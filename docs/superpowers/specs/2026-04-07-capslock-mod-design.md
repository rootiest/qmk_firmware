# Caps Lock Mod — Design Spec

**Date:** 2026-04-07  
**Keyboard:** Keychron Q5 Max (ANSI Encoder)  
**Keymap:** `keyboards/keychron/q5_max/ansi_encoder/keymaps/via/`

---

## Summary

Replace the plain `KC_CAPS` key with a smart `CAPS_MOD` custom keycode that provides
tap-vs-hold behavior and modifier-aware tap actions. Enable Caps Word and Autocorrect
features with RGB indicators on the Caps Lock key (LED 55).

---

## Behavior

| Action | Result |
|---|---|
| Tap | Escape |
| Hold (past tapping term) | Left Ctrl (held until release) |
| Shift + Tap | Toggle normal Caps Lock |
| Alt + Tap | Toggle Caps Word |
| GUI + Tap | Toggle Autocorrect |

**Priority for modifier-aware taps:** GUI → Alt → Shift → default (ESC).  
Modifiers are checked via `get_mods()` at key release time (they remain held by the user).

---

## Implementation

### New custom keycode

```c
CAPS_MOD  // added to enum custom_keycodes, after existing entries
```

Replaces every `KC_CAPS` in all layers of `keymap.c`.

### State variables

```c
static bool     caps_mod_held            = false;
static bool     caps_mod_ctrl_registered = false;
static uint16_t caps_mod_timer           = 0;
```

### `process_record_user` logic

**On press:**
```c
caps_mod_held  = true;
caps_mod_timer = timer_read();
```

**On release:**
```c
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
```

### `matrix_scan_user` addition

```c
if (caps_mod_held && !caps_mod_ctrl_registered
    && timer_elapsed(caps_mod_timer) > TAPPING_TERM) {
    caps_mod_ctrl_registered = true;
    register_code(KC_LCTL);
}
```

---

## RGB Indicators (LED 55 — Caps Lock physical position)

Added inside `rgb_matrix_indicators_advanced_user`, checked in priority order:

| State | Color |
|---|---|
| Caps Word on | Green `(0, 200, 0)` |
| Autocorrect on | Purple `(150, 0, 255)` |
| Normal Caps Lock on | White `(255, 255, 255)` |
| All off | Dark `(0, 0, 0)` |

Caps Word takes visual priority over host Caps Lock because both can technically be
active simultaneously.

---

## Feature Enablement

### `rules.mk` additions
```makefile
CAPS_WORD_ENABLE = yes
AUTOCORRECT_ENABLE = yes
```

### `config.h` addition
```c
#define AUTOCORRECT_MIN_LENGTH 4
```

`autocorrect_data.h` is already present in the keymap folder. QMK picks it up
automatically when `AUTOCORRECT_ENABLE = yes` — no manual `#include` needed.

---

## Files Changed

| File | Change |
|---|---|
| `keymap.c` | Add `CAPS_MOD` to enum; add state vars; add press/release logic in `process_record_user`; add scan logic in `matrix_scan_user`; add LED 55 indicators in `rgb_matrix_indicators_advanced_user`; replace all `KC_CAPS` with `CAPS_MOD` |
| `rules.mk` | Add `CAPS_WORD_ENABLE = yes` and `AUTOCORRECT_ENABLE = yes` |
| `config.h` | Add `AUTOCORRECT_MIN_LENGTH 4` |
