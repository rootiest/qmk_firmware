/* Copyright 2024 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "chord_unicode.h"
#include "raw_hid.h"
#include "hid_protocol.h"

// Tap Dance declarations
enum {
    TD_HOME_END,
};

// Custom keycodes
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

// Declare layers early so the HID functions below can reference KEEB_CTL.
enum layers {
    BASE,
    FN1,
    FN2,
    FN3,
    FN4,
    KEEB_CTL,
};

// Alt-Tab cycling state
static bool     alt_tab_active = false;
static uint16_t alt_tab_timer  = 0;
#define ALT_TAB_TIMEOUT 750 // ms to hold Alt after last encoder tick

// Layer-lock state: bitmask of layers that should stay active even after
// momentary (TT/MO) keys are released.
static layer_state_t locked_layers = 0;

// ---------------------------------------------------------------------------
// Raw HID state
// ---------------------------------------------------------------------------

// Anti-loop guard: set to true while applying a layer change that arrived
// over HID so that layer_state_set_user() does not echo it back.
static bool g_hid_recv_active = false;

// Last highest-layer value we sent via HID.  0xFF = never sent.
// Avoids redundant sends when layer_state_set_user is called multiple times
// with the same effective top layer.
static uint8_t g_last_sent_layer = 0xFF;

// Latest host-reported values (stored for future RGB indicator use).
static uint8_t g_hid_volume     = 0;
static uint8_t g_hid_brightness = 0;

// Send the current layer state to the host / bridge application.
static void hid_send_layer_sync(uint8_t layer, uint8_t locked_mask) {
    uint8_t data[RAW_EPSIZE] = {0};
    data[HID_OFF_CMD]                           = HID_CMD_LAYER_SYNC;
    data[HID_OFF_SRC]                           = HID_DEV_Q5MAX;
    data[HID_OFF_FLAGS]                         = 0;
    data[HID_PAYLOAD(HID_LAYER_OFF_ACTIVE)]     = layer;
    data[HID_PAYLOAD(HID_LAYER_OFF_LOCKED)]     = locked_mask;
    raw_hid_send(data, RAW_EPSIZE);
}

// Handle a Raw HID packet for our custom command range (0x40-0x7E).
// Called from via_command_kb(); must call raw_hid_send() for any reply.
bool via_command_kb(uint8_t *data, uint8_t length) {
    uint8_t cmd = data[HID_OFF_CMD];

    // Only intercept our custom command range; let VIA handle everything else.
    if (cmd < 0x40u || cmd > 0x7Eu) {
        return false;
    }

    uint8_t flags = data[HID_OFF_FLAGS];

    switch (cmd) {
        case HID_CMD_LAYER_SYNC: {
            if (flags & HID_FLAG_QUERY) {
                // Host requests current state — reply without changing anything.
                uint8_t resp[RAW_EPSIZE] = {0};
                resp[HID_OFF_CMD]                       = HID_CMD_LAYER_SYNC;
                resp[HID_OFF_SRC]                       = HID_DEV_Q5MAX;
                resp[HID_OFF_FLAGS]                     = HID_FLAG_RESPONSE;
                resp[HID_PAYLOAD(HID_LAYER_OFF_ACTIVE)] = get_highest_layer(layer_state);
                resp[HID_PAYLOAD(HID_LAYER_OFF_LOCKED)] = (uint8_t)locked_layers;
                raw_hid_send(resp, RAW_EPSIZE);
            } else {
                // Host or peer keyboard is pushing a new active layer.
                uint8_t new_layer  = data[HID_PAYLOAD(HID_LAYER_OFF_ACTIVE)];
                uint8_t new_locked = data[HID_PAYLOAD(HID_LAYER_OFF_LOCKED)];
                if (new_layer <= KEEB_CTL) {
                    // Set the guard BEFORE calling layer_move() so that the
                    // resulting layer_state_set_user() call does not echo the
                    // change back to the host, preventing an infinite loop.
                    g_hid_recv_active = true;
                    locked_layers     = new_locked;
                    layer_move(new_layer);
                    g_hid_recv_active = false;
                }
            }
            break;
        }

        case HID_CMD_VOLUME: {
            // Store the host-reported volume (0-100).
            g_hid_volume = data[HID_PAYLOAD(HID_VOLUME_OFF_LEVEL)];
            // TODO: drive an RGB volume-bar indicator here in a future commit.
            break;
        }

        case HID_CMD_BRIGHTNESS: {
            // Store the host-reported screen brightness (0-100).
            g_hid_brightness = data[HID_PAYLOAD(HID_BRITE_OFF_LEVEL)];
            // TODO: drive an RGB brightness indicator here in a future commit.
            break;
        }

        case HID_CMD_ACTIVE_APP: {
            // Reserved — no action yet.  Payload is a null-terminated UTF-8
            // application name (up to HID_APP_NAME_MAX bytes).
            // TODO: implement active-app handling once the host side is ready.
            break;
        }

        default:
            break;
    }

    return true;  // packet was fully handled by us
}

// CAPS_MOD state: tap=ESC, hold=Ctrl, Shift+tap=CapsLock, Alt+tap=CapsWord, GUI+tap=Autocorrect
static bool     caps_mod_held            = false;
static bool     caps_mod_ctrl_registered = false;
static uint16_t caps_mod_timer           = 0;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_ansi_101(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,             KC_DEL,   KC_PSCR,  KC_CALC,  KC_FIND,    KC_MPLY,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        CAPS_MOD, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             TD(TD_HOME_END),  KC_P4,    KC_P5,    KC_P6,      KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 TT(FN2), TT(FN1), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,            KC_P0,    KC_PDOT,    KC_PENT),

    [FN1] = LAYOUT_ansi_101(
        KC_SLEP,            KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,    KC_VOLD,  KC_VOLU,            KC_DEL,   KC_PSCR,  KC_CALC,  KC_FIND,    KC_MUTE,
        KC_GRV,  LCK_FN1, LCK_FN2,  LCK_FN3,  LCK_FN4, LCK_CTL,  KC_6,   KC_7,     KC_8,     KC_9,    LCK_BASE,  KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        CAPS_MOD, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_END,   KC_P4,    KC_P5,    KC_P6,      KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LGUI,  CHORD_KEY,                             KC_SPC,                            TT(FN3), TG(FN1), OSL(KEEB_CTL),  KC_HOME,  KC_DOWN,  KC_END,                 KC_P0,    KC_PDOT,    KC_PENT),

    [FN2] = LAYOUT_ansi_101(
        KC_PWR,             KC_F13,   KC_F14,   KC_F15,   KC_F16,   KC_F17,   KC_F18,   KC_F19,   KC_F20,   KC_F21,   KC_F22,     KC_F23,   KC_F24,             KC_DEL,   KC_PSCR,  KC_CALC,  KC_FIND,    KC_MPLY,
        KC_GRV,  LCK_FN1, LCK_FN2,  LCK_FN3,  LCK_FN4, LCK_CTL,  KC_6,   KC_7,     KC_8,     KC_9,    LCK_BASE,  KC_MINS,    KC_EQL,   KC_BSPC,            KC_MS_WH_UP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,           KC_MS_WH_DOWN, KC_P7,    KC_P8,    KC_P9,
        CAPS_MOD, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_MS_BTN3,   KC_P4,    KC_P5,    KC_P6,      KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_MS_UP,               KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LGUI,  KC_LALT,                               KC_SPC,                                 TG(FN2), TT(FN4), KC_RCTL,  KC_MS_LEFT,  KC_MS_DOWN,  KC_MS_RIGHT, KC_MS_BTN1,    KC_MS_BTN2,    KC_PENT),

    [FN3] = LAYOUT_ansi_101(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,    KC_VOLD,  KC_VOLU,            KC_DEL,   KC_PSCR,  KC_CALC,  KC_FIND,     KC_MPLY,
        KC_GRV,  LCK_FN1, LCK_FN2,  LCK_FN3,  LCK_FN4, LCK_CTL,  KC_6,   KC_7,     KC_8,     KC_9,    LCK_BASE,  KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        CAPS_MOD, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             TD(TD_HOME_END),  KC_P4,    KC_P5,    KC_P6,      KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LGUI,  KC_LALT,                               KC_SPC,                                 TG(FN3), TT(FN4), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,       KC_P0,    KC_PDOT,    KC_PENT),

    [FN4] = LAYOUT_ansi_101(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,    KC_VOLD,  KC_VOLU,            KC_DEL,   KC_PSCR,  KC_CALC,  KC_FIND,    KC_MPLY,
        KC_GRV,  LCK_FN1, LCK_FN2,  LCK_FN3,  LCK_FN4, LCK_CTL,  KC_6,   KC_7,     KC_8,     KC_9,    LCK_BASE,  KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        CAPS_MOD, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             TD(TD_HOME_END),  KC_P4,    KC_P5,    KC_P6,      KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LGUI,  KC_LALT,                               KC_SPC,                                 TO(BASE), TG(FN4), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,      KC_P0,    KC_PDOT,    KC_PENT),

    [KEEB_CTL] = LAYOUT_ansi_101(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,    KC_VOLD,  KC_VOLU,            _______,  _______,  _______,  _______,    RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______, LCK_BASE,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            KC_END,   _______,  _______,  _______,    _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,            _______,  _______,    _______),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [BASE]     = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [FN1]      = {ENCODER_CCW_CW(KC_MRWD, KC_MFFD)},
    [FN2]      = {ENCODER_CCW_CW(ALT_TAB_BWD, ALT_TAB_FWD)},
    [FN3]      = {ENCODER_CCW_CW(KC_MRWD, KC_MFFD)},
    [FN4]      = {ENCODER_CCW_CW(KC_MRWD, KC_MFFD)},
    [KEEB_CTL] = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)},
};
#endif // ENCODER_MAP_ENABLE

// clang-format on

// Combos -----------------------------------------------------------------
// COMM + DOT + SLSH → TO(BASE): emergency fallback to base layer.
// COMBO_ONLY_FROM_LAYER 0 (config.h) ensures these keycodes are always
// resolved from BASE so the combo fires regardless of the active layer.
const uint16_t PROGMEM fallback_combo[] = {KC_COMM, KC_DOT, KC_SLSH, COMBO_END};
combo_t                key_combos[]     = {
    COMBO(fallback_combo, TO(BASE)),
};

// Re-assert locked layers whenever QMK modifies layer state (e.g. TT release).
// Also notifies the host application of the new active layer via Raw HID,
// unless the change was itself triggered by an incoming HID packet (anti-loop).
layer_state_t layer_state_set_user(layer_state_t state) {
    state |= locked_layers;

    if (!g_hid_recv_active) {
        uint8_t top = get_highest_layer(state);
        if (top != g_last_sent_layer) {
            g_last_sent_layer = top;
            hid_send_layer_sync(top, (uint8_t)locked_layers);
        }
    }

    return state;
}

void keyboard_post_init_user(void) {
    chord_init();
    // Use the Linux unicode input method (Ctrl+Shift+U → hex → Enter).
    set_unicode_input_mode(UNICODE_MODE_LINUX);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }

    // Chord key: activate/deactivate chord unicode mode.
    if (keycode == CHORD_KEY) {
        if (record->event.pressed) {
            chord_activate();
        } else {
            chord_key_released();
        }
        return false;
    }

    // While chord mode is active, let it consume the key event.
    if (!process_chord(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case CAPS_MOD:
            if (record->event.pressed) {
                caps_mod_held  = true;
                caps_mod_timer = timer_read();
                // If a real modifier is held, send a dummy key so the OS sees
                // modifier+key rather than a bare modifier hold/tap. Without
                // this, the OS never receives any keycode while the modifier is
                // down and treats the eventual modifier release as a tap (e.g.
                // GUI opening the app menu). KC_F24 is harmless and universally
                // ignored by applications.
                if (get_mods() & (MOD_MASK_GUI | MOD_MASK_ALT | MOD_MASK_SHIFT)) {
                    register_code(KC_F24);
                    unregister_code(KC_F24);
                }
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
                        tap_code(KC_CAPS); // Shift still held → host sees Shift+CapsLock (toggles on most OSes)
                    } else {
                        tap_code(KC_ESC);
                    }
                }
                caps_mod_held = false; // cleared in both hold and tap paths
            }
            return false;

        case LCK_FN1:
        case LCK_FN2:
        case LCK_FN3:
        case LCK_FN4:
        case LCK_CTL:
        case LCK_BASE:
            if (record->event.pressed) {
                uint8_t target;
                switch (keycode) {
                    case LCK_FN1:  target = FN1;       break;
                    case LCK_FN2:  target = FN2;       break;
                    case LCK_FN3:  target = FN3;       break;
                    case LCK_FN4:  target = FN4;       break;
                    case LCK_CTL:  target = KEEB_CTL;  break;
                    default:       target = BASE;      break;
                }
                if (target != BASE && (locked_layers & (1UL << target))) {
                    // Already locked on this layer — unlock and return to BASE.
                    locked_layers = 0;
                    layer_move(BASE);
                } else {
                    // Lock the target layer (clears any other lock first).
                    locked_layers = 0;
                    layer_move(target);
                    if (target != BASE) {
                        locked_layers = (1UL << target);
                    }
                }
            }
            return false;

        case ALT_TAB_FWD:
            if (record->event.pressed) {
                if (!alt_tab_active) {
                    alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                tap_code(KC_TAB);
            }
            return false;
        case ALT_TAB_BWD:
            if (record->event.pressed) {
                if (!alt_tab_active) {
                    alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                register_code(KC_LSFT);
                tap_code(KC_TAB);
                unregister_code(KC_LSFT);
            }
            return false;
    }
    return true;
}

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

// RGB Matrix Indicators --------------------------------------------------
// ESC key (LED index 0) shows which layer is active at a glance.
// BASE stays dark; each FN/control layer gets a distinct colour.
#if defined(RGB_MATRIX_ENABLE)
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    switch (get_highest_layer(layer_state)) {
        case FN1:
            RGB_MATRIX_INDICATOR_SET_COLOR(0, 0, 128, 255); // blue
            break;
        case FN2:
            RGB_MATRIX_INDICATOR_SET_COLOR(0, 0, 220, 80); // green
            break;
        case FN3:
            RGB_MATRIX_INDICATOR_SET_COLOR(0, 255, 120, 0); // orange
            break;
        case FN4:
            RGB_MATRIX_INDICATOR_SET_COLOR(0, 180, 0, 255); // purple
            break;
        case KEEB_CTL:
            RGB_MATRIX_INDICATOR_SET_COLOR(0, 255, 0, 0); // red
            break;
        default: // BASE — keep ESC dark
            RGB_MATRIX_INDICATOR_SET_COLOR(0, 0, 0, 0);
            break;
    }

    // Caps Lock key (LED 55): shows CapsWord/Autocorrect/CapsLock state.
    if (is_caps_word_on()) {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 0, 200, 0);     // green: Caps Word active
    } else if (!autocorrect_is_enabled()) {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 150, 0, 255);   // purple: Autocorrect disabled
    } else if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 255, 255, 255); // white: normal Caps Lock on
    } else {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 0, 0, 0);       // off
    }

    return false;
}
#endif // RGB_MATRIX_ENABLE

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Home, twice for End
    [TD_HOME_END] = ACTION_TAP_DANCE_DOUBLE(KC_HOME, KC_END),
};
