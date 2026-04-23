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
#include "keychron_raw_hid.h"
#include "hid_protocol.h"
#ifdef LK_WIRELESS_ENABLE
#    include "battery.h"
#    include "transport.h"
#endif

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
    BSP_DEL,                  // Tap=Backspace, Shift+Tap=Delete
};

#define CAPS_MOD MT(MOD_LCTL, KC_ESC)

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

// Battery reporting state (wireless mode only).
// Pushes the current percentage every BAT_REPORT_INTERVAL_MS ms.
// Suppresses redundant sends when the level hasn't changed.
#ifdef LK_WIRELESS_ENABLE
#    define BAT_REPORT_INTERVAL_MS 300000U // 5 minutes
static uint32_t g_bat_timer     = 0;
static bool     g_bat_sent_once = false;
static uint8_t  g_last_sent_bat = HID_BATT_UNAVAILABLE;
#endif

// Send the current battery percentage to the host.
// Only fires when get_transport() & TRANSPORT_WIRELESS; no-op otherwise.
// Suppresses sends when the level matches the last value sent.
#ifdef LK_WIRELESS_ENABLE
static void hid_send_battery(void) {
    if (!(get_transport() & TRANSPORT_WIRELESS)) return;
    uint8_t level = battery_get_percentage();
    if (level == g_last_sent_bat) return;
    g_last_sent_bat                       = level;
    uint8_t data[HID_PACKET_SIZE]         = {0};
    data[HID_OFF_CMD]                     = HID_CMD_BATTERY;
    data[HID_OFF_SRC]                     = HID_DEV_Q5MAX;
    data[HID_OFF_FLAGS]                   = 0;
    data[HID_PAYLOAD(HID_BATT_OFF_LEVEL)] = level;
    raw_hid_send(data, HID_PACKET_SIZE);
}
#endif

// Send the current layer state to the host / bridge application.
static void hid_send_layer_sync(uint8_t layer, uint8_t locked_mask) {
    uint8_t data[HID_PACKET_SIZE]           = {0};
    data[HID_OFF_CMD]                       = HID_CMD_LAYER_SYNC;
    data[HID_OFF_SRC]                       = HID_DEV_Q5MAX;
    data[HID_OFF_FLAGS]                     = 0;
    data[HID_PAYLOAD(HID_LAYER_OFF_ACTIVE)] = layer;
    data[HID_PAYLOAD(HID_LAYER_OFF_LOCKED)] = locked_mask;
    raw_hid_send(data, HID_PACKET_SIZE);
}

// Handle a Raw HID packet for our custom command range (0x40-0x7E).
// Overrides the weak kc_raw_hid_rx_kb() hook in keychron_raw_hid.c, which is
// called by kc_raw_hid_rx() for any command ID not handled by Keychron's own
// protocol.  Must call raw_hid_send() directly for any reply.
bool kc_raw_hid_rx_kb(uint8_t *data, uint8_t length) {
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
                uint8_t resp[HID_PACKET_SIZE]           = {0};
                resp[HID_OFF_CMD]                       = HID_CMD_LAYER_SYNC;
                resp[HID_OFF_SRC]                       = HID_DEV_Q5MAX;
                resp[HID_OFF_FLAGS]                     = HID_FLAG_RESPONSE;
                resp[HID_PAYLOAD(HID_LAYER_OFF_ACTIVE)] = get_highest_layer(layer_state);
                resp[HID_PAYLOAD(HID_LAYER_OFF_LOCKED)] = (uint8_t)locked_layers;
                raw_hid_send(resp, HID_PACKET_SIZE);
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

        case HID_CMD_BATTERY: {
            // Host is querying the current battery level.
            // Reply with the current percentage when in wireless mode, or
            // HID_BATT_UNAVAILABLE when wired (USB transport / not meaningful).
            uint8_t resp[HID_PACKET_SIZE] = {0};
            resp[HID_OFF_CMD]             = HID_CMD_BATTERY;
            resp[HID_OFF_SRC]             = HID_DEV_Q5MAX;
            resp[HID_OFF_FLAGS]           = HID_FLAG_RESPONSE;
#ifdef LK_WIRELESS_ENABLE
            resp[HID_PAYLOAD(HID_BATT_OFF_LEVEL)] = (get_transport() & TRANSPORT_WIRELESS) ? battery_get_percentage() : HID_BATT_UNAVAILABLE;
#else
            resp[HID_PAYLOAD(HID_BATT_OFF_LEVEL)] = HID_BATT_UNAVAILABLE;
#endif
            raw_hid_send(resp, HID_PACKET_SIZE);
            break;
        }

        default:
            break;
    }

    return true; // packet was fully handled by us
}

// CAPS_MOD state: tap=ESC, hold=Ctrl, Shift+tap=CapsLock, Alt+tap=CapsWord, GUI+tap=Autocorrect
// (Refactored to use MT(MOD_LCTL, KC_ESC) with custom tap logic)

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
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,        QK_CLEAR_EEPROM,  _______,    _______),
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
// COMM + DOT + SLSH → LCK_BASE: emergency fallback to base layer.
// COMBO_ONLY_FROM_LAYER 0 (config.h) ensures these keycodes are always
// resolved from BASE so the combo fires regardless of the active layer.
const uint16_t PROGMEM fallback_combo[] = {KC_COMM, KC_DOT, KC_SLSH, COMBO_END};
combo_t                key_combos[]     = {
    COMBO(fallback_combo, LCK_BASE),
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

#ifdef DIP_SWITCH_ENABLE
// dip_switch_update_user is claimed by factory_test.c; use the weak
// dip_switch_update_keymap hook added in q5_max.c instead.

// True while the Win-side dip switch is active.  The underlying RGB effect
// keeps running unchanged; rgb_matrix_indicators_advanced_user() paints over
// all LEDs with white each frame so neither mode nor EEPROM state is touched.
// Transport changes (which call rgb_matrix_init()) are therefore irrelevant.
static bool dip_win_active = false;

void dip_switch_update_keymap(uint8_t index, bool active) {
    if (index == 0) {
        dip_win_active = active;
    }
}
#endif

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
            // Custom tap logic: only intercept if it's a TAP AND a modifier is held.
            // If it's a pure hold (Ctrl) or a pure tap (Esc), return true to let
            // the MT() core handle it.
            if (record->tap.count > 0 && record->event.pressed) {
                uint8_t mods = get_mods();
                if (mods & (MOD_MASK_GUI | MOD_MASK_ALT)) {
                    // Neutralize the modifier hold so releasing GUI/Alt doesn't
                    // trigger an OS "tap" action (like opening the Start menu).
                    tap_code(DUMMY_MOD_NEUTRALIZER_KEYCODE);

                    if (mods & MOD_MASK_GUI) {
                        autocorrect_toggle();
                    } else {
                        caps_word_toggle();
                    }
                    return false; // suppress default Esc tap
                } else if (mods & MOD_MASK_SHIFT) {
                    tap_code(KC_CAPS);
                    return false; // suppress default Esc tap
                }
            }
            return true; // let core handle Esc tap or Ctrl hold

        case LCK_FN1:
        case LCK_FN2:
        case LCK_FN3:
        case LCK_FN4:
        case LCK_CTL:
        case LCK_BASE:
            if (record->event.pressed) {
                uint8_t target;
                switch (keycode) {
                    case LCK_FN1:
                        target = FN1;
                        break;
                    case LCK_FN2:
                        target = FN2;
                        break;
                    case LCK_FN3:
                        target = FN3;
                        break;
                    case LCK_FN4:
                        target = FN4;
                        break;
                    case LCK_CTL:
                        target = KEEB_CTL;
                        break;
                    default:
                        target = BASE;
                        break;
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

        case BSP_DEL:
            if (record->event.pressed) {
                uint8_t mods = get_mods();
                if (mods & MOD_MASK_SHIFT) {
                    del_mods(MOD_MASK_SHIFT);
                    tap_code(KC_DEL);
                    set_mods(mods);
                } else {
                    tap_code(KC_BSPC);
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
    if (alt_tab_active && timer_elapsed(alt_tab_timer) > ALT_TAB_TIMEOUT) {
        unregister_code(KC_LALT);
        alt_tab_active = false;
    }
    chord_scan();

#ifdef LK_WIRELESS_ENABLE
    // Push battery level to host every BAT_REPORT_INTERVAL_MS when wireless.
    // First call fires immediately (g_bat_sent_once == false) so the host gets
    // a reading as soon as the keyboard connects over USB in wireless mode.
    if (!g_bat_sent_once || timer_elapsed32(g_bat_timer) >= BAT_REPORT_INTERVAL_MS) {
        g_bat_sent_once = true;
        g_bat_timer     = timer_read32();
        hid_send_battery();
    }
#endif
}

// RGB Matrix Indicators --------------------------------------------------
// ESC key (LED index 0) shows which layer is active at a glance.
// BASE stays dark; each FN/control layer gets a distinct colour.
#if defined(RGB_MATRIX_ENABLE)
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
#    ifdef DIP_SWITCH_ENABLE
    // Win-side override: paint all LEDs white so the user gets a clean white
    // backlight regardless of which RGB effect is active.  The effect keeps
    // ticking internally and resumes the moment the switch returns to Mac side.
    // Layer and status indicators painted in the rest of this function appear
    // on top of the white fill, so they continue to work normally.
    if (dip_win_active) {
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, 255, 255, 255);
        }
    }
#    endif

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
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 0, 200, 0); // green: Caps Word active
    } else if (!autocorrect_is_enabled()) {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 150, 0, 255); // purple: Autocorrect disabled
    } else if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 255, 255, 255); // white: normal Caps Lock on
    } else {
        RGB_MATRIX_INDICATOR_SET_COLOR(55, 0, 0, 0); // off
    }

    return false;
}
#endif // RGB_MATRIX_ENABLE

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TD(TD_HOME_END):
            return 175;
        default:
            return TAPPING_TERM;
    }
}

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Home, twice for End
    [TD_HOME_END] = ACTION_TAP_DANCE_DOUBLE(KC_HOME, KC_END),
};
