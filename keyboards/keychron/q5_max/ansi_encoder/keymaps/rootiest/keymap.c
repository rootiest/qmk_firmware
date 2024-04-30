/**
 * Copyright (C) 2024 Chris Laprade (chris@rootiest.com)
 *
 * This file is part of rootiest.
 *
 * rootiest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * rootiest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with rootiest.  If not, see <https://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H // QMK Keyboard library
#include "keychron_common.h" // Keychron common definitions
#include "features/select_word.h" // Select word feature
#include "features/mouse_turbo_click.h" // Mouse turbo click feature
#include "features/layer_lock.h" // Layer lock feature
#include "features/achordion.h" // Achordion feature
#include "features/sentence_case.h" // Sentence case feature
#include "rootiest.h" // Rootiest customizations

    /* ============================================================================================================ */
    /* ============================================================================================================ */
    /* ================================================            ================================================ */
    /* ================================================   KEYMAP   ================================================ */
    /* ================================================            ================================================ */
    /* ============================================================================================================ */
    /* ============================================================================================================ */
    //              The keymap defines the layout of the keyboard. The layout is defined as a
    //              two-dimensional array where each row represents a row of keys on the keyboard.
    //              The first dimension of the array represents the layer, and the second dimension
    //              represents the rows of keys on the keyboard. The keys are represented by their
    //                        keycodes, which are defined in the QMK Keyboard library.

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAIN_BASE] = LAYOUT_ansi_101(
        KC_ESC,             KC_BRID,  KC_BRIU,  KC_MCTRL, KC_MYCM, KC_F5,  KC_F6,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_F10,    KC_F11,  KC_F12,            KC_DEL,   KC_PSCR,   KC_WWW_SEARCH ,  QK_LEAD,     KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            TD(TD_PG_HOME),  TD(TD_NUM_CLEAR),   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            TD(TD_PG_END),    KC_P7,    KC_P8,    KC_P9,
        TD(TD_ESC_CAPS),  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,     TD(TD_HOME_END),  KC_P4,    KC_P5,    KC_P6,     KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,                      KC_P1,    KC_P2,    KC_P3,
        SC_LCPO,  KC_LGUI, KC_LALT,                    KC_SPC,                 KC_RALT, MO(MAIN_FN), SC_RCPC,  KC_LEFT,  KC_DOWN,  KC_RGHT,                                   LT(SPECIAL_FN, KC_P0),  LT(PREFIX_C_K, KC_PDOT),   KC_PENT),
    [MAIN_FN] = LAYOUT_ansi_101(
        _______,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    RGB_VAD,    RGB_VAI,    KC_F7,    KC_F8,    KC_F9,    KC_MUTE,     KC_VOLD,   KC_VOLU,      A(KC_F4), _______, _______, ALT4KEY,    A(KC_F4),
        HEATMAP,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  KC_CALC,  _______,  _______,    _______,
        RGB_MOD,  RGB_TOG,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______, SS_PERSONAL, SS_ADDRESS,
        _______,  C(KC_A), C(KC_S),  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,             _______,  SS_NICK,  SS_WEB,   SS_GITHUB,    _______,
        _______,            C(KC_Z),  C(KC_X),  C(KC_C),  C(KC_V),  BAT_LVL,  NK_TOGG,  TO(MUSIK),  _______,  AC_TOGG,  _______,              _______,  _______,            SS_NAME,  SS_EMAIL,  SS_PHONE,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  TD(TD_OPEN),  _______,  TD(TD_CLOSE),            _______,  _______,    _______),
    [PLAIN_BASE] = LAYOUT_ansi_101(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    RGB_VAI,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,             KC_DEL,   _______,  _______,  _______,    KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,  KC_P4,    KC_P5,    KC_P6,      KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(PLAIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,            KC_P0,    KC_PDOT,    KC_PENT),
    [PLAIN_FN] = LAYOUT_ansi_101(
        _______,            KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,    KC_VOLD,  KC_VOLU,            _______,  _______,  _______,  _______,    RGB_TOG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______, KC_CLEAR, _______,  _______,    _______,
        RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,
        _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            KC_END,   _______,  _______,  _______,    _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,            _______,  _______,    _______),
    [EXT_BASE] = LAYOUT_ansi_101(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,             KC_DEL,   _______,  _______,  _______,    KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,  KC_P4,    KC_P5,    KC_P6,      KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(PREFIX_C_K), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,            KC_P0,    KC_PDOT,    KC_PENT),
    [PREFIX_C_K] = LAYOUT_ansi_101(
        _______,            _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,   _______,             _______,  _______,  _______,  _______,    KC_BSPC,
        _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,    _______,
        _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______, _______, _______,
        _______,  _______, _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,             _______,  _______,  _______,   _______,    _______,
        _______,            _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,            _______,  _______,    _______),
    [MUSIK] = LAYOUT_ansi_101(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,             KC_DEL,   _______,  _______,  _______,    KC_MUTE,
        KC_GRV,   KC_1,     MI_C3,    MI_D3,    MI_E3,    MI_F3,    MI_G3,    MI_A3,    MI_B3,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     MI_C2,    MI_D2,    MI_E2,    MI_F2,    MI_G2,    MI_A2,    MI_B2,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     MI_C1,    MI_D1,    MI_E1,    MI_F1,    MI_G1,    MI_A1,    MI_B1,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,  KC_P4,    KC_P5,    KC_P6,      KC_PPLS,
        KC_LSFT,            MI_C,     MI_D,     MI_E,     MI_F,     MI_G,     MI_A,     MI_B,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  MI_OCTU,              KC_P1,    KC_P2,    KC_P3,
        MI_SUST,  MI_MOD,  MI_SOFT,                                MI_AOFF,                                 MI_SOFT,  TO(MUSIK), MI_SUST,  MI_TRSD,  MI_OCTD,  MI_TRSU,            KC_P0,    KC_PDOT,    KC_PENT),
    [SPECIAL_FN] = LAYOUT_ansi_101(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,             KC_DEL,   _______,  _______,  _______,    KC_MUTE,
        KC_TILD,  KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,  KC_CIRC,  KC_KP_7,  KC_KP_8,  KC_KP_9,  KC_ASTR,  KC_PMNS,    KC_EQL,   KC_BSPC,            _______,  _______,   _______,  _______,    _______,
        KC_TAB,   KC_HOME,  KC_UP,    KC_END,   KC_DEL,   KC_ESC,   KC_PGUP,  KC_KP_4,  KC_KP_5,  KC_KP_6,  KC_PPLS,  KC_LPRN,    KC_RPRN,  KC_BSLS,            _______,  _______,    _______,    _______,
        TD(TD_ESC_CAPS), KC_LEFT, KC_DOWN, KC_RIGHT, KC_BSPC, KC_NUM, KC_PGDN, KC_KP_1, KC_KP_2,  KC_KP_3,  KC_ENT,   KC_QUOT,              KC_ENT,             _______,  _______,    _______,    _______,      _______,
        KC_LSFT,            KC_UNDO,  KC_CUT,   KC_COPY, KC_PASTE,  KC_UNDS,  KC_COLN,  KC_KP_0,  DBLZERO,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              _______,    _______,    _______,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(PLAIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,            _______,    _______,    _______),
};  /* ============================================================================================================ */
    /* ============================================================================================================ */
    /* ============================================================================================================ */
    /* ============================================================================================================ */
    /* ============================================================================================================ */
    /* ============================================================================================================ */
    /* ============================================================================================================ */


/* ##################################################################### */
/* ########################### Mouse jiggler ########################### */
/* ##################################################################### */
// Jiggles the mouse every 5 seconds to prevent the computer from going to sleep.
// https://www.reddit.com/r/olkb/comments/t4imri/comment/hz2w67i/?context=3
bool mouse_jiggler_enabled = false;
static uint16_t mouse_jiggler_timer;

bool has_mouse_report_changed(report_mouse_t* new_report, report_mouse_t* old_report) {
    // Only report every 5 seconds.
    if (mouse_jiggler_enabled && timer_elapsed(mouse_jiggler_timer) > 5000) {
        mouse_jiggler_timer = timer_read();
        return mouse_jiggler_enabled;
    }
    return memcmp(new_report, old_report, sizeof(report_mouse_t));
}
void mouse_jiggle_toggle(void) {
    mouse_jiggler_timer = timer_read();
    mouse_jiggler_enabled = ! mouse_jiggler_enabled;
}

// Autosaver (automatically saves documents every 30 seconds)
// NOTE: This function is not currently operational.
// The intended function is to send Ctrl+S every 30 seconds to
// save the current document continuously.
bool autosave_enabled = false;

void autosave_toggle(void) {
    autosave_enabled = ! autosave_enabled;
}

/* ##################################################################### */
/* ####################### Tap dancing functions ####################### */
/* ##################################################################### */
// Tap dance functions allow you to perform different actions depending on the
// number of times you tap or hold the key. Here we define the tap dance functions
// that are used in the keymap.
void dance_open_brackets(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1: // (
            register_code(KC_LSFT);
            tap_code(KC_9);
            unregister_code(KC_LSFT);
            break;
        case 2: // [
            tap_code(KC_LBRC);
            break;
        case 3: // {
            register_code(KC_LSFT);
            tap_code(KC_LBRC);
            unregister_code(KC_LSFT);
            break;
        case 4: // <
            register_code(KC_LSFT);
            tap_code(KC_COMMA);
            unregister_code(KC_LSFT);
            break;
    }
}

void dance_close_brackets(tap_dance_state_t *state, void *user_data) {
    switch (state->count) {
        case 1: // )
            register_code(KC_LSFT);
            tap_code(KC_0);
            unregister_code(KC_LSFT);
            break;
        case 2: // ]
            tap_code(KC_RBRC);
            break;
        case 3: // }
            register_code(KC_LSFT);
            tap_code(KC_RBRC);
            unregister_code(KC_LSFT);
            break;
        case 4: // >
            register_code(KC_LSFT);
            tap_code(KC_DOT);
            unregister_code(KC_LSFT);
            break;
    }
}

void sticky_shift_finished(tap_dance_state_t *state, void *user_data) {
    if (state->count >= 5) { // Caps Lock
        unregister_code(KC_LSFT);
        tap_code(KC_CAPS);
        reset_tap_dance(state);
    } else { // Shift
        register_code(KC_LSFT);
    }
}

void sticky_shift_reset(tap_dance_state_t *state, void *user_data) {
    unregister_code(KC_LSFT); // Release shift
}

void sticky_shift_tap(tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        register_code(KC_LSFT); // Hold shift
    }
}

void sticky_shift_release(tap_dance_state_t *state, void *user_data) {
    unregister_code(KC_LSFT); // Release shift
}

/* ##################################################################### */
/* ####################### Tap Dance definitions ####################### */
/* ##################################################################### */
// Tap Dance keys allow you to perform different actions depending on the
// number of times you tap or hold the key. Here we define the tap dance keys.

tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_ESC_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS),
    // Tap once for Page Up, twice for Home
    [TD_PG_HOME] = ACTION_TAP_DANCE_DOUBLE(KC_PGUP, KC_HOME),
    // Tap once for Page Down, twice for End
    [TD_PG_END] = ACTION_TAP_DANCE_DOUBLE(KC_PGDN, KC_END),
    // Tap once for Home, twice for Page Up
    [TD_HOME_PG] = ACTION_TAP_DANCE_DOUBLE(KC_HOME, KC_PGUP),
    // Tap once for End, twice for Page Down
    [TD_END_PG] = ACTION_TAP_DANCE_DOUBLE(KC_END, KC_PGDN),
    // Tap once for Home, twice for End
    [TD_HOME_END] = ACTION_TAP_DANCE_DOUBLE(KC_HOME, KC_END),
    // Tap once for (, twice for [, thrice for {, four times for <
    [TD_OPEN] = ACTION_TAP_DANCE_FN(dance_open_brackets),
    // Tap once for ), twice for ], thrice for }, four times for >
    [TD_CLOSE] = ACTION_TAP_DANCE_FN(dance_close_brackets),
    // Tap once for (, twice for )
    [TD_P_OPEN_CLOSE] = ACTION_TAP_DANCE_DOUBLE(KC_LPRN, KC_RPRN),
    // Tap once for [, twice for ]
    [TD_B_OPEN_CLOSE] = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_RBRC),
    // Tap once for {, twice for }
    [TD_C_OPEN_CLOSE] = ACTION_TAP_DANCE_DOUBLE(KC_LCBR, KC_RCBR),
    // Tap five times for Caps Lock
    [TD_STICKY_SHIFT] = ACTION_TAP_DANCE_FN_ADVANCED_WITH_RELEASE(sticky_shift_tap, sticky_shift_release, sticky_shift_finished, sticky_shift_reset),
    // Tap once for Num Lock, twice for Calculator
    [TD_NUM_CALC] = ACTION_TAP_DANCE_DOUBLE(KC_NUM, KC_CALC),
    // Tap once for Num Lock, twice for Clear
    [TD_NUM_CLEAR] = ACTION_TAP_DANCE_DOUBLE(KC_NUM, KC_CLEAR),
};

// Key overrides
const key_override_t next_track_override =
    ko_make_with_layers_negmods_and_options(
        MOD_MASK_CTRL,       // Trigger modifiers: ctrl
        KC_MPLY,             // Trigger key: play/pause
        KC_MNXT,             // Replacement key
        ~0,                  // Activate on all layers
        MOD_MASK_SA,         // Do not activate when shift or alt are pressed
        ko_option_no_reregister_trigger); // Specifies that the play key is not registered again after lifting ctrl

// This is the same as the previous override, but with the play/pause key replaced with the previous track key
const key_override_t prev_track_override = ko_make_with_layers_negmods_and_options(MOD_MASK_CS, KC_MPLY,
                                            KC_MPRV, ~0, MOD_MASK_ALT, ko_option_no_reregister_trigger);

// This is the same as the previous override, but with the play/pause key replaced with the volume up key
const key_override_t vol_up_override = ko_make_with_layers_negmods_and_options(MOD_MASK_ALT, KC_MPLY,
                                            KC_VOLU, ~0, MOD_MASK_CS, ko_option_no_reregister_trigger);

// This is the same as the previous override, but with the play/pause key replaced with the volume down key
const key_override_t vol_down_override = ko_make_with_layers_negmods_and_options(MOD_MASK_SA, KC_MPLY,
                                            KC_VOLD, ~0, MOD_MASK_CTRL, ko_option_no_reregister_trigger);

// This is the same as the previous override, but with the play/pause key replaced with the brightness up key
const key_override_t brightness_up_override = ko_make_with_layers_negmods_and_options(MOD_MASK_CA, KC_MPLY,
                                            KC_BRIU, ~0, MOD_MASK_SHIFT, ko_option_no_reregister_trigger);

// This is the same as the previous override, but with the play/pause key replaced with the brightness down key
const key_override_t brightness_down_override = ko_make_basic(MOD_MASK_CSA, KC_MPLY, KC_BRID);

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &next_track_override, // Next track override
    &prev_track_override, // Previous track override
    &vol_up_override,     // Volume up override
    &vol_down_override,   // Volume down override
    &brightness_up_override,   // Brightness up override
    &brightness_down_override, // Brightness down override
    NULL
};

// Helper for implementing tap vs. long-press keys. Given a tap-hold
// key event, replaces the hold function with `long_press_keycode`.
static bool process_tap_or_long_press_key(
    keyrecord_t* record, uint16_t long_press_keycode) {
    if (record->tap.count == 0) {  // Key is being held.
        if (record->event.pressed) {
            tap_code16(long_press_keycode); // Send long-press keycode.
        }
        return false;  // Skip default handling.
    }
    return true;  // Continue default handling.
}

/* ##################################################################### */
/* ######################## Idle timeout handler ####################### */
/* ##################################################################### */
// This function is called when the keyboard has been idle for a certain
// amount of time. The idle timeout is defined by IDLE_TIMEOUT_MS.
static uint32_t idle_callback(uint32_t trigger_time, void* cb_arg) {
    // If execution reaches here, the keyboard has gone idle.
    return 0;
}

/* ##################################################################### */
/* ########################## Encoder mappping ######################### */
/* ##################################################################### */
// The encoder map defines the behavior of the encoders. It maps the
// encoders to the keycodes that are sent when the encoder is turned.
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume control
    [MAIN_FN]   = {ENCODER_CCW_CW(KC_WH_U, KC_WH_D)}, // Scroll wheel
    [PLAIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume control
    [PLAIN_FN]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)}, // RGB brightness control
    [EXT_BASE]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume control
    [PREFIX_C_K]  = {ENCODER_CCW_CW(C(KC_LEFT), C(KC_RIGHT))}, // Move cursor by word
    [MUSIK]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume control
    [SPECIAL_FN]  = {ENCODER_CCW_CW(KC_WH_L, KC_WH_R)}, // Horizontal scroll wheel
};
#endif // ENCODER_MAP_ENABLE

bool is_alt_4_active = false;

/* ##################################################################### */
/* ######################## Process Record User ######################## */
/* ##################################################################### */
// Actions performed when a key is pressed or released
// Here we define the bulk of the custom keycodes and features performed
// by a key press or release.
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {

    // On every key event, start or extend the deferred execution to call
    // `idle_callback()` after IDLE_TIMEOUT_MS.
    static deferred_token idle_token = INVALID_DEFERRED_TOKEN;
    if (!extend_deferred_exec(idle_token, IDLE_TIMEOUT_MS)) {
        idle_token = defer_exec(IDLE_TIMEOUT_MS, idle_callback, NULL);
    }

    // process feature callbacks
    if (!process_select_word(keycode, record, SELWORD)) { return false; } // Select word
    if (!process_mouse_turbo_click(keycode, record, TURBO)) { return false; } // Mouse turbo click
    if (!process_layer_lock(keycode, record, LLOCK)) { return false; } // Layer lock
    if (!process_achordion(keycode, record)) { return false; } // Achordion
    if (!process_sentence_case(keycode, record)) { return false; } // Sentence case

    // Alt4Key overrides
    if (!record->event.pressed) {  // On key up event.
        static uint8_t count = 0;
        if (keycode == ALT4KEY) {
            count = 5;  // Tapping ALT4KEY sets count = 4.
            is_alt_4_active = true;
            return true;
        }
        // While count > 0, hold Alt on next key.
        if (count > 0) {
            if (get_mods() == (MOD_BIT(KC_RALT))) {
                --count;
            } else {
                register_code(KC_RALT);
                --count;
            }
        } else {
            if (is_alt_4_active) {
                is_alt_4_active = false;
                clear_mods();
                return false;
            }
        }
    }

    // Get current mod and one-shot mod states.
    const uint8_t mods = get_mods();
    const uint8_t oneshot_mods = get_oneshot_mods();

    // Process Prefix Layer keycode
    if (IS_LAYER_ON(PREFIX_C_K) && record->event.pressed) {
        tap_code16(PREFIX_KEY);  // Tap the prefix key combo.
    }

    // process custom keycodes
    switch (keycode) {
        case SS_EMAIL: // Sends email address.
            if (record->event.pressed) {
                SEND_STRING("chris@rootiest.com");
            }
            return false;
        case SS_NAME: // Sends full name.
            if (record->event.pressed) {
                SEND_STRING("Chris Laprade");
            }
            return false;
        case SS_NICK: // Sends nickname.
            if (record->event.pressed) {
                SEND_STRING("rootiest");
            }
            return false;
        case SS_PHONE: // Sends phone number.
            if (record->event.pressed) {
                SEND_STRING("+1 413-233-4602");
            }
            return false;
        case SS_GITHUB: // Sends GitHub profile.
            if (record->event.pressed) {
                SEND_STRING("https://github.com/rootiest");
            }
            return false;
        case SS_WEB: // Sends rootiest website.
            if (record->event.pressed) {
                SEND_STRING("https://rootiest.com");
            }
            return false;
        case SS_PERSONAL: // Sends personal email address.
            if (record->event.pressed) {
                SEND_STRING("boogie7601@gmail.com");
            }
            return false;
        case SS_ADDRESS: // Sends personal address.
            if (record->event.pressed) {
                SEND_STRING("43 Pickering Rd Apt F\nRochester, NH 03839\nUnited States");
            }
            return false;
        case JIGGLE: // Toggles mouse jiggler.
            if (record->event.pressed) {
                mouse_jiggle_toggle();
            }
            return false;
        case AUTOSAVE: // Toggles autosaver.
            if (record->event.pressed) {
                autosave_enabled = true;
            } else {
                autosave_enabled = false;
            }
            return false;
        case UPDIR:  // Types ../ to go up a directory on the shell.
            if (record->event.pressed) {
                SEND_STRING("../");
            }
            return false;
        case EQ3X:  // Types triple equal ===
            if (record->event.pressed) {
                SEND_STRING("===");
            }
            return false;
        case DBLCLK:  // Double click the left mouse button.
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_BTN1) SS_DELAY(50) SS_TAP(X_BTN1));
            }
            return false;
        case SELLINE:  // Selects the current line.
            if (record->event.pressed) {
                SEND_STRING(SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_END)));
                // Mac users, use:
                // SEND_STRING(SS_LCTL("a" SS_LSFT("e")));
            }
            return false;
        case SRCHSEL:  // Searches the current selection in a new tab.
            if (record->event.pressed) {
                // Mac users, change LCTL to LGUI.
                SEND_STRING(SS_LCTL("ct") SS_DELAY(100) SS_LCTL("v") SS_TAP(X_ENTER));
            }
            return false;
        case JOINLN:  // Join lines like Vim's `J` command.
            if (record->event.pressed) {
                SEND_STRING( // Go to the end of the line and tap delete.
                    SS_TAP(X_END) SS_TAP(X_DEL)
                    // In case this has joined two words together, insert one space.
                    SS_TAP(X_SPC)
                    SS_LCTL(
                        // Go to the beginning of the next word.
                        SS_TAP(X_RGHT) SS_TAP(X_LEFT)
                        // Select back to the end of the previous word. This should select
                        // all spaces and tabs between the joined lines from indentation
                        // or trailing whitespace, including the space inserted earlier.
                        SS_LSFT(SS_TAP(X_LEFT) SS_TAP(X_RGHT)))
                        // Replace the selection with a single space.
                    SS_TAP(X_SPC));
            }
            return false;
        case NEXTSEN:  // Next sentence macro.
            if (record->event.pressed) {
                SEND_STRING(". ");
                add_oneshot_mods(MOD_BIT(KC_LSFT));  // Set one-shot mod for shift.
            }
            return false;
        case NEXTWORD:  // Next word macro.
            if (record->event.pressed) {
                SEND_STRING(
                    SS_LCTL(
                        SS_TAP(X_RGHT)));
            }
            return false;
        case PREVWORD:  // Previous word macro.
            if (record->event.pressed) {
                SEND_STRING(
                    SS_LCTL(
                        SS_TAP(X_LEFT)));
            }
            return false;
        case US_FLAG:  // Types US flag emoji 🇺🇸.
            if (record->event.pressed) {
                send_unicode_string("🇺🇸");
                // Or if you prefer ASCII-only source files, use escape codes to
                // write the UTF-8 encoding of U+1F1FA, U+1F1F8:
                // send_unicode_string("\xf0\x9f\x87\xba\xf0\x9f\x87\xb8");
            }
            return false;
        case DBLZERO: // Types double zero.
            if (record->event.pressed) {
                tap_code(KC_0);
                tap_code(KC_0);
            }
            return false;
        case HEATMAP: // Enables heatmap mode.
            if (record->event.pressed) {
                rgb_matrix_mode(RGB_MATRIX_TYPING_HEATMAP);
            }
            return false;
        case COMM_COPY:  // Comma on tap, Ctrl+C on long press.
            return process_tap_or_long_press_key(record, C(KC_C));

        case DOT_PASTE:  // Dot on tap, Ctrl+V on long press.
            return process_tap_or_long_press_key(record, C(KC_V));

        case MPLY_MNXT:  // Play/pause on tap, next song on long press.
            return process_tap_or_long_press_key(record, KC_MNXT);
        case ARROW:  // Arrow macro, types -> or =>.
            if (record->event.pressed) {
                if ((mods | oneshot_mods) & MOD_MASK_SHIFT) {  // Is shift held?
                    // Temporarily delete shift.
                    del_oneshot_mods(MOD_MASK_SHIFT);
                    unregister_mods(MOD_MASK_SHIFT);
                    SEND_STRING("=>");
                    register_mods(mods);            // Restore mods.
                } else {
                    SEND_STRING("->");
                }
            }
            return false;
        case DEGREE:  // Degree symbol macro: °
            if (record->event.pressed) {
                send_unicode_string("°");
            }
            return false;
        case DEGREE_C:  // Degree Celsius symbol macro: ℃
            if (record->event.pressed) {
                send_unicode_string("℃");
            }
            return false;
        case DEGREE_F:  // Degree Fahrenheit symbol macro: ℉
            if (record->event.pressed) {
                send_unicode_string("℉");
            }
            return false;
        case SMILE:  // Smile emoji macro: 🙂
            if (record->event.pressed) {
                send_unicode_string("🙂");
            }
            return false;
        case FROWN:  // Frown emoji macro: 🙁
            if (record->event.pressed) {
                send_unicode_string("🙁");
            }
            return false;
        case LOL:  // LOL emoji macro: 😂
            if (record->event.pressed) {
                send_unicode_string("😂");
            }
            return false;
        case SHOCK:  // Shock emoji macro: 😱
            if (record->event.pressed) {
                send_unicode_string("😱");
            }
            return false;
        case SAD:  // Sad emoji macro: 😢
            if (record->event.pressed) {
                send_unicode_string("😢");
            }
            return false;
        case WINK:  // Wink emoji macro: 😉
            if (record->event.pressed) {
                send_unicode_string("😉");
            }
            return false;
        case SHRUG:  // Shrug emoji macro: 🤷
            if (record->event.pressed) {
                send_unicode_string("🤷");
            }
            return false;
        case THINK:  // Thinking emoji macro: 🤔
            if (record->event.pressed) {
                send_unicode_string("🤔");
            }
            return false;
        case THUMBS_UP:  // Thumbs up emoji macro: 👍
            if (record->event.pressed) {
                send_unicode_string("👍");
            }
            return false;
        case THUMBS_DOWN:  // Thumbs down emoji macro: 👎
            if (record->event.pressed) {
                send_unicode_string("👎");
            }
            return false;
        case HEART:  // Heart emoji macro: ❤️
            if (record->event.pressed) {
                send_unicode_string("❤️");
            }
            return false;
        case STAR:  // Star emoji macro: ⭐
            if (record->event.pressed) {
                send_unicode_string("⭐");
            }
            return false;
        case CHECK:  // Check emoji macro: ✅
            if (record->event.pressed) {
                send_unicode_string("✅");
            }
            return false;
        case CROSS:  // Cross emoji macro: ❌
            if (record->event.pressed) {
                send_unicode_string("❌");
            }
            return false;
        case HUNDRED:  // Hundred emoji macro: 💯
            if (record->event.pressed) {
                send_unicode_string("💯");
            }
            return false;
        case WARNING:  // Warning emoji macro: ⚠️
            if (record->event.pressed) {
                send_unicode_string("⚠️");
            }
            return false;
        case FIRE:  // Fire emoji macro: 🔥
            if (record->event.pressed) {
                send_unicode_string("🔥");
            }
            return false;
        case POOP:  // Poop emoji macro: 💩
            if (record->event.pressed) {
                send_unicode_string("💩");
            }
            return false;
        case CLAP:  // Clap emoji macro: 👏
            if (record->event.pressed) {
                send_unicode_string("👏");
            }
            return false;
        case BULLET:  // Bullet emoji macro: •
            if (record->event.pressed) {
                send_unicode_string("•");
            }
            return false;
        case DASH:  // Dash emoji macro: —
            if (record->event.pressed) {
                send_unicode_string("—");
            }
            return false;
        case ELLIPSIS:  // Ellipsis emoji macro: …
            if (record->event.pressed) {
                send_unicode_string("…");
            }
            return false;
        case COPYRIGHT:  // Copyright symbol macro: ©
            if (record->event.pressed) {
                send_unicode_string("©");
            }
            return false;
        case REGISTERED:  // Registered symbol macro: ®
            if (record->event.pressed) {
                send_unicode_string("®");
            }
            return false;
        case TRADEMARK:  // Trademark symbol macro: ™
            if (record->event.pressed) {
                send_unicode_string("™");
            }
            return false;
        case SECTION:  // Section symbol macro: §
            if (record->event.pressed) {
                send_unicode_string("§");
            }
            return false;
        case PARAGRAPH:  // Paragraph symbol macro: ¶
            if (record->event.pressed) {
                send_unicode_string("¶");
            }
            return false;
        case EURO:  // Euro symbol macro: €
            if (record->event.pressed) {
                send_unicode_string("€");
            }
            return false;
/*  Backspace or delete based on shift key state override. */
#ifdef USE_BSPC_DEL_OVERRIDE
        case KC_BSPC: { // Backspace or Delete based on shift key state.
            static uint16_t registered_key = KC_NO;
            if (record->event.pressed) {  // On key press.
                const uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
                uint8_t shift_mods = (mods | get_oneshot_mods()) & MOD_MASK_SHIFT;
#else
                uint8_t shift_mods = mods & MOD_MASK_SHIFT;
#endif  // NO_ACTION_ONESHOT
                if (shift_mods) {  // At least one shift key is held.
                    registered_key = KC_DEL;
                    // If one shift is held, clear it from the mods. But if both
                    // shifts are held, leave as is to send Shift + Del.
                    if (shift_mods != MOD_MASK_SHIFT) {
#ifndef NO_ACTION_ONESHOT
                        del_oneshot_mods(MOD_MASK_SHIFT);
#endif  // NO_ACTION_ONESHOT
                        unregister_mods(MOD_MASK_SHIFT);
                    }
                } else {
                    registered_key = KC_BSPC;
                }

                register_code(registered_key);
                set_mods(mods);
            } else {  // On key release.
                unregister_code(registered_key);
            }
        }
        return false;
#endif
    }

    // Call the common keychron process record function
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }
    return true;
}

/* ##################################################################### */
/* ########################### Layer Locking ########################### */
/* ##################################################################### */
// This function is called whenever the layer lock state changes.
void layer_lock_set_user(layer_state_t locked_layers) {
  // Do something like `set_led(is_layer_locked(NAV));`
}

/* ##################################################################### */
/* ######################## Matrix Scan function ####################### */
/* ##################################################################### */
// Actions performed each cycle of the matrix scan
void matrix_scan_user(void) {
    select_word_task(); // Select word task
    layer_lock_task(); // Layer lock task
    achordion_task(); // Achordion task
    sentence_case_task(); // Sentence case task
    // Other tasks...
}
