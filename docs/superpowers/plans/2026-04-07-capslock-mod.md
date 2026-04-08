# Caps Lock Mod Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Replace the plain Caps Lock key with a smart `CAPS_MOD` key: tap=ESC, hold=Ctrl, Shift+tap=CapsLock, Alt+tap=CapsWord, GUI+tap=Autocorrect toggle, with RGB indicators on the Caps Lock key.

**Architecture:** Custom keycode `CAPS_MOD` with a timer-based tap/hold split. `process_record_user` starts the timer on press and dispatches actions on release. `matrix_scan_user` promotes a held key to Ctrl once `TAPPING_TERM` elapses. RGB indicators for CapsWord (green), Autocorrect (purple), and host CapsLock (white) are added to the existing `rgb_matrix_indicators_advanced_user` function.

**Tech Stack:** QMK firmware (C), RGB Matrix, Caps Word, Autocorrect — all built in to QMK.

**Spec:** `docs/superpowers/specs/2026-04-07-capslock-mod-design.md`

**Build command** (always activate venv first):
```bash
source .venv/bin/activate && qmk compile -kb keychron/q5_max/ansi_encoder -km via
```

---

## File Map

| File | Change |
|---|---|
| `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/rules.mk` | Add `CAPS_WORD_ENABLE` and `AUTOCORRECT_ENABLE` |
| `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/config.h` | No change needed (`AUTOCORRECT_MIN_LENGTH` already defined in `autocorrect_data.h`) |
| `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/autocorrect_data.h` | Commit to git (was untracked; pre-generated dictionary with `AUTOCORRECT_MIN_LENGTH 4` already inside) |
| `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/keymap.c` | Add `CAPS_MOD` to enum; add state vars; replace all `KC_CAPS`; add press/release + scan logic; add LED 55 indicators |

---

## Task 1: Enable Caps Word and Autocorrect features

**Files:**
- Modify: `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/rules.mk`
- Commit (untracked): `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/autocorrect_data.h`
- Note: `config.h` needs no change — `AUTOCORRECT_MIN_LENGTH 4` is already defined inside `autocorrect_data.h`

- [ ] **Step 1: Add feature flags to rules.mk**

Replace the entire file content with:

```makefile
VIA_ENABLE = yes
TAP_DANCE_ENABLE = yes
UNICODE_ENABLE = yes
COMBO_ENABLE = yes
CAPS_WORD_ENABLE = yes
AUTOCORRECT_ENABLE = yes
SRC += chord_unicode.c
```

- [ ] **Step 2: Verify autocorrect_data.h is present and has AUTOCORRECT_MIN_LENGTH**

Check that `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/autocorrect_data.h` exists and contains `#define AUTOCORRECT_MIN_LENGTH`. Do NOT add it to `config.h` — it is already defined in the pre-generated file.

- [ ] **Step 3: Compile to verify no breakage**

```bash
source .venv/bin/activate && qmk compile -kb keychron/q5_max/ansi_encoder -km via
```

Expected: build succeeds, `.bin` file produced. No errors.

- [ ] **Step 4: Commit**

```bash
git add keyboards/keychron/q5_max/ansi_encoder/keymaps/via/rules.mk \
        keyboards/keychron/q5_max/ansi_encoder/keymaps/via/config.h
git commit -m "feat(q5_max): enable CAPS_WORD and AUTOCORRECT features"
```

---

## Task 2: Add CAPS_MOD keycode, state variables, and replace KC_CAPS

**Files:**
- Modify: `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/keymap.c`

- [ ] **Step 1: Add CAPS_MOD to the custom keycodes enum**

Find this block (lines ~27–37):

```c
enum custom_keycodes {
    ALT_TAB_FWD = SAFE_RANGE, // Alt+Tab (forward)
    ALT_TAB_BWD,              // Alt+Shift+Tab (backward)
    CHORD_KEY,                // Fn1+LeftAlt → chord/unicode entry mode
    LCK_FN1,                  // Lock/unlock FN1
    LCK_FN2,                  // Lock/unlock FN2
    LCK_FN3,                  // Lock/unlock FN3
    LCK_FN4,                  // Lock/unlock FN4
    LCK_CTL,                  // Lock/unlock KEEB_CTL
    LCK_BASE,                 // Clear all locks and return to BASE
};
```

Replace with:

```c
enum custom_keycodes {
    ALT_TAB_FWD = SAFE_RANGE, // Alt+Tab (forward)
    ALT_TAB_BWD,              // Alt+Shift+Tab (backward)
    CHORD_KEY,                // Fn1+LeftAlt → chord/unicode entry mode
    LCK_FN1,                  // Lock/unlock FN1
    LCK_FN2,                  // Lock/unlock FN2
    LCK_FN3,                  // Lock/unlock FN3
    LCK_FN4,                  // Lock/unlock FN4
    LCK_CTL,                  // Lock/unlock KEEB_CTL
    LCK_BASE,                 // Clear all locks and return to BASE
    CAPS_MOD,                 // Tap=ESC, hold=Ctrl, Shift=CapsLock, Alt=CapsWord, GUI=Autocorrect
};
```

- [ ] **Step 2: Add CAPS_MOD state variables**

Find this block (lines ~44–46):

```c
// Layer-lock state: bitmask of layers that should stay active even after
// momentary (TT/MO) keys are released.
static layer_state_t locked_layers = 0;
```

Replace with:

```c
// Layer-lock state: bitmask of layers that should stay active even after
// momentary (TT/MO) keys are released.
static layer_state_t locked_layers = 0;

// CAPS_MOD state: tap=ESC, hold=Ctrl, Shift+tap=CapsLock, Alt+tap=CapsWord, GUI+tap=Autocorrect
static bool     caps_mod_held            = false;
static bool     caps_mod_ctrl_registered = false;
static uint16_t caps_mod_timer           = 0;
```

- [ ] **Step 3: Replace KC_CAPS with CAPS_MOD in all five layers**

In the keymap arrays, find every occurrence of `KC_CAPS` and replace with `CAPS_MOD`. There are five instances — one at the start of row 3 in each of: BASE, FN1, FN2, FN3, FN4. KEEB_CTL already uses `_______` for that position and stays unchanged.

Each row looks like:
```c
        KC_CAPS,  KC_A,     KC_S, ...
```
Change to:
```c
        CAPS_MOD, KC_A,     KC_S, ...
```

Do this for all five layers.

- [ ] **Step 4: Compile to verify**

```bash
source .venv/bin/activate && qmk compile -kb keychron/q5_max/ansi_encoder -km via
```

Expected: build succeeds. `CAPS_MOD` is defined but not yet handled — QMK will pass it through to `process_record_user` which returns `true` by default, so no errors.

- [ ] **Step 5: Commit**

```bash
git add keyboards/keychron/q5_max/ansi_encoder/keymaps/via/keymap.c
git commit -m "feat(q5_max): add CAPS_MOD keycode and replace KC_CAPS in all layers"
```

---

## Task 3: Implement tap/hold logic in process_record_user and matrix_scan_user

**Files:**
- Modify: `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/keymap.c`

- [ ] **Step 1: Add CAPS_MOD case to the switch in process_record_user**

Find the `switch (keycode)` block in `process_record_user`. It currently starts with `case LCK_FN1:`. Add the `CAPS_MOD` case **before** the `LCK_FN1` case:

```c
        case CAPS_MOD:
            if (record->event.pressed) {
                caps_mod_held  = true;
                caps_mod_timer = timer_read();
            } else {
                if (caps_mod_ctrl_registered) {
                    unregister_code(KC_LCTL);
                    caps_mod_ctrl_registered = false;
                } else {
                    uint8_t mods = get_mods();
                    if (mods & MOD_MASK_GUI) {
                        autocorrect_toggle();
                    } else if (mods & MOD_MASK_ALT) {
                        caps_word_toggle();
                    } else if (mods & MOD_MASK_SHIFT) {
                        tap_code(KC_CAPS);
                    } else {
                        tap_code(KC_ESC);
                    }
                }
                caps_mod_held = false;
            }
            return false;
```

- [ ] **Step 2: Add Ctrl promotion to matrix_scan_user**

Find `matrix_scan_user`:

```c
void matrix_scan_user(void) {
    if (alt_tab_active && timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT) {
        unregister_code(KC_LALT);
        alt_tab_active = false;
    }
    chord_scan();
}
```

Replace with:

```c
void matrix_scan_user(void) {
    if (caps_mod_held && !caps_mod_ctrl_registered
        && timer_elapsed(caps_mod_timer) > TAPPING_TERM) {
        caps_mod_ctrl_registered = true;
        register_code(KC_LCTL);
    }
    if (alt_tab_active && timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT) {
        unregister_code(KC_LALT);
        alt_tab_active = false;
    }
    chord_scan();
}
```

- [ ] **Step 3: Compile to verify**

```bash
source .venv/bin/activate && qmk compile -kb keychron/q5_max/ansi_encoder -km via
```

Expected: build succeeds, no errors or warnings about undefined functions. (`autocorrect_toggle`, `caps_word_toggle`, `get_mods`, `MOD_MASK_*` are all QMK builtins.)

- [ ] **Step 4: Commit**

```bash
git add keyboards/keychron/q5_max/ansi_encoder/keymaps/via/keymap.c
git commit -m "feat(q5_max): implement CAPS_MOD tap/hold logic (ESC/Ctrl/CapsLock/CapsWord/Autocorrect)"
```

---

## Task 4: Add RGB indicators for the Caps Lock key (LED 55)

**Files:**
- Modify: `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/keymap.c`

- [ ] **Step 1: Add LED 55 indicators to rgb_matrix_indicators_advanced_user**

Find `rgb_matrix_indicators_advanced_user`. It currently ends with:

```c
        default: // BASE — keep ESC dark
            RGB_MATRIX_INDICATOR_SET_COLOR(0, 0, 0, 0);
            break;
    }
    return false;
}
```

Replace that closing section with:

```c
        default: // BASE — keep ESC dark
            RGB_MATRIX_INDICATOR_SET_COLOR(0, 0, 0, 0);
            break;
    }

    // Caps Lock key (LED 55): shows CapsWord/Autocorrect/CapsLock state.
    if (is_caps_word_on()) {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 0, 200, 0);     // green: Caps Word active
    } else if (autocorrect_is_enabled()) {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 150, 0, 255);   // purple: Autocorrect active
    } else if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 255, 255, 255); // white: normal Caps Lock on
    } else {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 0, 0, 0);       // off
    }

    return false;
}
```

- [ ] **Step 2: Compile to verify**

```bash
source .venv/bin/activate && qmk compile -kb keychron/q5_max/ansi_encoder -km via
```

Expected: build succeeds. (`is_caps_word_on`, `autocorrect_is_enabled`, `host_keyboard_led_state` are all QMK builtins available when their features are enabled.)

- [ ] **Step 3: Commit**

```bash
git add keyboards/keychron/q5_max/ansi_encoder/keymaps/via/keymap.c
git commit -m "feat(q5_max): add RGB indicators for CapsWord/Autocorrect/CapsLock on LED 55"
```

---

## Self-Review Checklist

- [x] Tap=ESC → handled in `process_record_user` else branch, `tap_code(KC_ESC)`
- [x] Hold=Ctrl → `matrix_scan_user` promotes after `TAPPING_TERM`, released on key-up
- [x] Shift+tap=CapsLock → `MOD_MASK_SHIFT` branch, `tap_code(KC_CAPS)`
- [x] Alt+tap=CapsWord → `MOD_MASK_ALT` branch, `caps_word_toggle()`
- [x] GUI+tap=Autocorrect → `MOD_MASK_GUI` branch, `autocorrect_toggle()`
- [x] Green LED for CapsWord → `is_caps_word_on()` branch
- [x] Purple LED for Autocorrect → `autocorrect_is_enabled()` branch
- [x] White LED for CapsLock → `host_keyboard_led_state().caps_lock` branch
- [x] `CAPS_WORD_ENABLE = yes` in rules.mk
- [x] `AUTOCORRECT_ENABLE = yes` in rules.mk
- [x] `AUTOCORRECT_MIN_LENGTH 4` in config.h
- [x] All 5 `KC_CAPS` instances replaced (BASE, FN1, FN2, FN3, FN4)
- [x] KEEB_CTL left unchanged (`_______`)
- [x] LED index 55 confirmed from `ansi_encoder.c` matrix map (row 3, col 0)
