#!/usr/bin/env python3
"""Generate cheatsheet.html from chord_unicode.c, keymap.c, autocorrect_dictionary.txt and info.json.

Run from anywhere:  python3 gen_cheatsheet.py
The output is a single self-contained HTML page (layer maps, key tricks, autocorrect and chord lookup).
"""

import json
import re
from pathlib import Path

HERE = Path(__file__).resolve().parent
OUT = HERE / "cheatsheet.html"

# ---------------------------------------------------------------------------
# Chord table
# ---------------------------------------------------------------------------


def parse_chords():
    src = (HERE / "chord_unicode.c").read_text()
    table = src[src.index("chord_table[] = {"):src.index("// clang-format on")]
    entries, cat = [], "Other"
    for line in table.splitlines():
        m = re.match(r"\s*// ---- (.+?) -+\s*$", line)
        if m:
            cat = m.group(1).replace("Emoji : ", "Emoji · ")
            continue
        m = re.match(r'\s*\{"([^"]+)",\s*"([^"]+)"\},\s*(?://\s*(.*))?', line)
        if m:
            seq, out, comment = m.group(1), m.group(2), (m.group(3) or "")
            name = re.sub(r"\s*U\+[0-9A-Fa-f]+.*$", "", comment).strip()
            entries.append({"seq": seq, "out": out, "cat": cat, "name": name})
    return entries


def simulate(seq, entries):
    """Replay process_chord()/chord_check() for one typed sequence.

    Returns (status, fired_at): 'ok' fires on the last letter, 'wait' needs the
    CHORD_DISAMBIG_MS pause (or Enter), 'early' fires before the sequence ends.
    """
    for k in range(1, len(seq) + 1):
        buf = seq[:k]
        exact = next((e for e in entries if e["seq"] == buf), None)
        if exact is None:
            continue
        conflict = any(e["seq"].startswith(buf) and len(e["seq"]) > k and e["out"] != exact["out"] for e in entries)
        if not conflict:
            return ("ok" if k == len(seq) else "early"), buf
        if k == len(seq):
            return "wait", buf
    return "ok", seq


def build_chords():
    entries = parse_chords()
    groups = {}
    for e in entries:
        g = groups.setdefault(e["out"], {"out": e["out"], "cat": e["cat"], "name": "", "seqs": []})
        if e["name"] and "alias" not in e["name"] and not g["name"]:
            g["name"] = re.sub(r"\s*\(.*\)$", "", e["name"])
        status, at = simulate(e["seq"], entries)
        g["seqs"].append({"s": e["seq"], "st": status, "at": at})
    for g in groups.values():
        g["cp"] = ["U+%04X" % ord(c) for c in g["out"]]
        g["seqs"].sort(key=lambda s: (s["st"] == "early", len(s["s"])))
    return list(groups.values())


# ---------------------------------------------------------------------------
# Keymap
# ---------------------------------------------------------------------------

LAYER_INFO = {
    "BASE": ("Base", "#8a94a0", "ESC LED off"),
    "FN1": ("Fn1", "#0080ff", "ESC LED blue"),
    "FN2": ("Fn2", "#00dc50", "ESC LED green"),
    "FN3": ("Fn3", "#ff7800", "ESC LED orange"),
    "FN4": ("Fn4", "#b400ff", "ESC LED purple"),
    "KEEB_CTL": ("Control", "#ff2020", "ESC LED red"),
}

SHIFTED = {
    "KC_GRV": ("~", "`"), "KC_1": ("!", "1"), "KC_2": ("@", "2"), "KC_3": ("#", "3"), "KC_4": ("$", "4"),
    "KC_5": ("%", "5"), "KC_6": ("^", "6"), "KC_7": ("&", "7"), "KC_8": ("*", "8"), "KC_9": ("(", "9"),
    "KC_0": (")", "0"), "KC_MINS": ("_", "-"), "KC_EQL": ("+", "="), "KC_LBRC": ("{", "["),
    "KC_RBRC": ("}", "]"), "KC_BSLS": ("|", "\\"), "KC_SCLN": (":", ";"), "KC_QUOT": ('"', "'"),
    "KC_COMM": ("<", ","), "KC_DOT": (">", "."), "KC_SLSH": ("?", "/"),
}

# keycode: (label, category, description)
LABELS = {
    "KC_ESC": ("Esc", "mod", "Escape"), "KC_TAB": ("Tab", "mod", "Tab"), "KC_BSPC": ("Backspace", "mod", "Backspace"),
    "KC_ENT": ("Enter", "mod", "Enter"), "KC_SPC": ("", "mod", "Space"), "KC_DEL": ("Del", "mod", "Delete"),
    "KC_LSFT": ("Shift", "mod", "Left Shift"), "KC_RSFT": ("Shift", "mod", "Right Shift"),
    "KC_LCTL": ("Ctrl", "mod", "Left Control"), "KC_RCTL": ("Ctrl", "mod", "Right Control"),
    "KC_LGUI": ("Super", "mod", "Left GUI / Super"), "KC_LALT": ("Alt", "mod", "Left Alt"),
    "KC_PSCR": ("PrtSc", "mod", "Print Screen"), "KC_CALC": ("Calc", "media", "Calculator"),
    "KC_FIND": ("Find", "media", "Find"), "KC_PGUP": ("PgUp", "mod", "Page Up"), "KC_PGDN": ("PgDn", "mod", "Page Down"),
    "KC_HOME": ("Home", "mod", "Home"), "KC_END": ("End", "mod", "End"),
    "KC_UP": ("↑", "mod", "Up"), "KC_DOWN": ("↓", "mod", "Down"), "KC_LEFT": ("←", "mod", "Left"), "KC_RGHT": ("→", "mod", "Right"),
    "KC_NUM": ("Num", "mod", "Num Lock"), "KC_PSLS": ("/", "", "Keypad /"), "KC_PAST": ("*", "", "Keypad *"),
    "KC_PMNS": ("−", "", "Keypad −"), "KC_PPLS": ("+", "", "Keypad +"), "KC_PDOT": (".", "", "Keypad ."),
    "KC_PENT": ("Enter", "mod", "Keypad Enter"),
    "KC_MPLY": ("⏯", "media", "Play / pause"), "KC_MPRV": ("⏮", "media", "Previous track"),
    "KC_MNXT": ("⏭", "media", "Next track"), "KC_MUTE": ("Mute", "media", "Mute"),
    "KC_VOLD": ("Vol −", "media", "Volume down"), "KC_VOLU": ("Vol +", "media", "Volume up"),
    "KC_MRWD": ("Rewind", "media", "Rewind"), "KC_MFFD": ("Fast fwd", "media", "Fast forward"),
    "KC_BRID": ("Bright −", "media", "Screen brightness down"), "KC_BRIU": ("Bright +", "media", "Screen brightness up"),
    "KC_MCTRL": ("Mission", "media", "Mission Control (macOS)"), "KC_LNPAD": ("Launch", "media", "Launchpad (macOS)"),
    "KC_TASK": ("Tasks", "media", "Task view"), "KC_FILE": ("Files", "media", "File explorer"),
    "KC_SLEP": ("Sleep", "sys", "System sleep"), "KC_PWR": ("Power", "sys", "System power"),
    "RGB_TOG": ("LED ⏻", "rgb", "Backlight on/off"), "RGB_MOD": ("Effect →", "rgb", "Next backlight effect"),
    "RGB_RMOD": ("Effect ←", "rgb", "Previous backlight effect"), "RGB_VAI": ("LED +", "rgb", "Backlight brighter"),
    "RGB_VAD": ("LED −", "rgb", "Backlight dimmer"), "RGB_HUI": ("Hue +", "rgb", "Backlight hue up"),
    "RGB_HUD": ("Hue −", "rgb", "Backlight hue down"), "RGB_SAI": ("Sat +", "rgb", "Backlight saturation up"),
    "RGB_SAD": ("Sat −", "rgb", "Backlight saturation down"), "RGB_SPI": ("Speed +", "rgb", "Effect speed up"),
    "RGB_SPD": ("Speed −", "rgb", "Effect speed down"),
    "BT_HST1": ("BT 1", "sys", "Bluetooth host 1"), "BT_HST2": ("BT 2", "sys", "Bluetooth host 2"),
    "BT_HST3": ("BT 3", "sys", "Bluetooth host 3"), "P2P4G": ("2.4 GHz", "sys", "2.4 GHz dongle"),
    "BAT_LVL": ("Battery", "sys", "Show battery level on LEDs"), "NK_TOGG": ("NKRO", "sys", "Toggle N-key rollover"),
    "QK_CLEAR_EEPROM": ("Clear EE", "danger", "Clear EEPROM — wipes all saved settings"),
    "KC_MS_WH_UP": ("Wheel ↑", "mouse", "Scroll up"), "KC_MS_WH_DOWN": ("Wheel ↓", "mouse", "Scroll down"),
    "KC_MS_UP": ("Ms ↑", "mouse", "Mouse up"), "KC_MS_DOWN": ("Ms ↓", "mouse", "Mouse down"),
    "KC_MS_LEFT": ("Ms ←", "mouse", "Mouse left"), "KC_MS_RIGHT": ("Ms →", "mouse", "Mouse right"),
    "KC_MS_BTN1": ("Click L", "mouse", "Left click"), "KC_MS_BTN2": ("Click R", "mouse", "Right click"),
    "KC_MS_BTN3": ("Click M", "mouse", "Middle click"),
    "CAPS_MOD": ("Esc ⁄ Ctrl", "custom", "Tap Esc · hold Ctrl · Shift+tap Caps Lock · Alt+tap Caps Word · Super+tap toggle Autocorrect"),
    "TD(TD_HOME_END)": ("Home ⁄ End", "custom", "Tap Home · double-tap End"),
    "CHORD_KEY": ("Chord", "chord", "Chord entry — type a sequence to output a symbol"),
    "LCK_BASE": ("Unlock", "custom", "Clear layer locks, return to Base"),
    "ALT_TAB_FWD": ("Alt-Tab →", "custom", "Cycle windows forward"), "ALT_TAB_BWD": ("Alt-Tab ←", "custom", "Cycle windows back"),
}
LAYER_OPS = {
    "TT": ("", "Tap-toggle {n}: hold for momentary, tap 3× to toggle"),
    "TG": ("⇄ ", "Toggle {n} on/off"),
    "TO": ("→ ", "Switch to {n} (clears others)"),
    "OSL": ("¹ ", "One-shot {n}: applies to the next key only"),
    "MO": ("", "Hold for {n}"),
}


def describe(kc):
    if kc == "_______":
        return {"t": 1}
    m = re.fullmatch(r"(TT|TG|TO|OSL|MO)\((\w+)\)", kc)
    if m:
        op, layer = m.groups()
        name = LAYER_INFO[layer][0]
        prefix, desc = LAYER_OPS[op]
        return {"l": prefix + name, "c": "layer", "to": layer, "d": desc.format(n=name)}
    m = re.fullmatch(r"LCK_(FN\d|CTL)", kc)
    if m:
        layer = "KEEB_CTL" if m.group(1) == "CTL" else m.group(1)
        name = LAYER_INFO[layer][0]
        return {"l": "🔒 " + name, "c": "layer", "to": layer, "d": f"Lock {name} on (press again to unlock)"}
    if kc in SHIFTED:
        top, bottom = SHIFTED[kc]
        return {"l": bottom, "s": top, "c": "", "d": kc}
    if kc in LABELS:
        label, cat, desc = LABELS[kc]
        return {"l": label, "c": cat, "d": desc}
    m = re.fullmatch(r"KC_([A-Z])", kc)
    if m:
        return {"l": m.group(1), "c": "", "d": kc}
    m = re.fullmatch(r"KC_(\d)", kc)
    if m:
        return {"l": m.group(1), "c": "", "d": kc}
    m = re.fullmatch(r"KC_P(\d)", kc)
    if m:
        return {"l": m.group(1), "c": "", "d": "Keypad " + m.group(1)}
    m = re.fullmatch(r"KC_F(\d+)", kc)
    if m:
        return {"l": "F" + m.group(1), "c": "", "d": "F" + m.group(1)}
    return {"l": kc.replace("KC_", ""), "c": "", "d": kc}


def split_args(body):
    args, depth, cur = [], 0, ""
    for ch in body:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            args.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        args.append(cur.strip())
    return args


def build_layers():
    src = (HERE / "keymap.c").read_text()
    info = json.loads((HERE / "../../info.json").read_text())
    geom = info["layouts"]["LAYOUT_ansi_101"]["layout"]
    layers = []
    for m in re.finditer(r"\[(\w+)\]\s*=\s*LAYOUT_ansi_101\(", src):
        i, depth = m.end(), 1
        while depth:
            depth += {"(": 1, ")": -1}.get(src[i], 0)
            i += 1
        codes = split_args(src[m.end():i - 1])
        assert len(codes) == len(geom), f"{m.group(1)}: {len(codes)} keycodes vs {len(geom)} positions"
        keys = []
        for kc, g in zip(codes, geom):
            k = describe(kc)
            k.update(kc=kc, x=g["x"], y=g["y"], w=g.get("w", 1), h=g.get("h", 1))
            keys.append(k)
        name = m.group(1)
        layers.append({"id": name, "name": LAYER_INFO[name][0], "led": LAYER_INFO[name][1], "ledNote": LAYER_INFO[name][2], "keys": keys})

    enc = dict((m.group(1), [describe(m.group(2))["l"], describe(m.group(3))["l"]])
               for m in re.finditer(r"\[(\w+)\]\s*=\s*\{ENCODER_CCW_CW\((\w+),\s*(\w+)\)\}", src))
    for layer in layers:
        layer["enc"] = enc.get(layer["id"])
        # Every key on another layer that leads here.
        layer["via"] = [{"from": other["name"], "l": k["l"], "d": k["d"]}
                        for other in layers if other is not layer
                        for k in other["keys"] if k.get("to") == layer["id"]]
    return layers


def main():
    header = (HERE / "chord_unicode.h").read_text()
    consts = {n: int(re.search(rf"#define {n} (\d+)", header).group(1))
              for n in ("CHORD_MAX_LEN", "CHORD_TIMEOUT_MS", "CHORD_DISAMBIG_MS")}
    keymap = (HERE / "keymap.c").read_text()
    config = (HERE / "config.h").read_text() + (HERE / "../../config.h").read_text()
    keys = {
        "TAPPING_TERM": int(re.search(r"#define TAPPING_TERM (\d+)", config).group(1)),
        "TAPPING_TOGGLE": int(re.search(r"#define TAPPING_TOGGLE (\d+)", config).group(1)),
        "ALT_TAB_TIMEOUT": int(re.search(r"#define ALT_TAB_TIMEOUT (\d+)", keymap).group(1)),
        "HOME_END_MS": int(re.search(r"case TD\(TD_HOME_END\):\s*return (\d+)", keymap).group(1)),
    }
    autocorrect = re.findall(r"^(\S+)\s*->\s*(.+?)\s*$", (HERE / "autocorrect_dictionary.txt").read_text(), re.M)
    data = {"chords": build_chords(), "layers": build_layers(), "consts": consts, "keys": keys, "autocorrect": autocorrect}
    payload = json.dumps(data, ensure_ascii=False, separators=(",", ":")).replace("</", "<\\/")
    template = (HERE / "cheatsheet.template.html").read_text().split("\n", 1)[1]  # drop the template note
    OUT.write_text(template.replace("/*DATA*/null", payload))
    print(f"wrote {OUT.name}: {len(data['chords'])} symbols, {len(data['layers'])} layers, {len(autocorrect)} autocorrections, {keys}")


if __name__ == "__main__":
    main()
