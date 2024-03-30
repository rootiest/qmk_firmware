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
#include "quantum.h"

/* ##################################################################### */
/* ########################### Layer Locking ########################### */
/* ##################################################################### */
// This function is called whenever the layer lock state changes.
void layer_lock_set_user(layer_state_t locked_layers) {
  // Do something like `set_led(is_layer_locked(NAV));`
}


/* ##################################################################### */
/* ######################## Leader key functions ####################### */
/* ##################################################################### */
// The leader key allows for sequences of keys to be pressed after the leader key
// to trigger different actions. This is useful for creating a large number of
// key combinations without having to assign each one to a separate key.
void leader_start_user(void) {
    // Do something when the leader key is first pressed
}

void leader_end_user(void) {
    // Do something when the leader key sequence ends
    if (leader_sequence_one_key(KC_F)) {
        // Leader, f => Types the below string
        SEND_STRING("QMK is awesome.");
    } else if (leader_sequence_two_keys(KC_D, KC_D)) {
        // Leader, d, d => Ctrl+A, Ctrl+C
        SEND_STRING(SS_LCTL("a") SS_LCTL("c"));
    } else if (leader_sequence_three_keys(KC_D, KC_D, KC_S)) {
        // Leader, d, d, s => Types the below string
        SEND_STRING("https://start.duckduckgo.com\n");
    } else if (leader_sequence_two_keys(KC_R, KC_S)) {
        // Leader, r, s => Types the below string
        SEND_STRING("https://search.rootiest.dev\n");
    } else if (leader_sequence_two_keys(KC_A, KC_S)) {
        // Leader, a, s => GUI+S
        tap_code16(LGUI(KC_S));
    } else if (leader_sequence_three_keys(KC_D, KC_E, KC_G)) {
        // Leader, d, e, g => Types the degree symbol: °
        send_unicode_string("°");
    } else if (leader_sequence_three_keys(KC_D, KC_E, KC_C)) {
        // Leader, d, e, c => Types the degree Celsius symbol: ℃
        send_unicode_string("℃");
    } else if (leader_sequence_three_keys(KC_D, KC_E, KC_F)) {
        // Leader, d, e, f => Types the degree Fahrenheit symbol: ℉
        send_unicode_string("℉");
    } else if (leader_sequence_five_keys(KC_S, KC_M, KC_I, KC_L, KC_E)) {
        // Leader, s, m, i, l, e => Types the smile emoji: 🙂
        send_unicode_string("🙂");
    } else if (leader_sequence_five_keys(KC_F, KC_R, KC_O, KC_W, KC_N)) {
        // Leader, f, r, o, w, n => Types the frown emoji: 🙁
        send_unicode_string("🙁");
    } else if (leader_sequence_three_keys(KC_L, KC_O, KC_L)) {
        // Leader, l, o, l => Types the LOL emoji: 😂
        send_unicode_string("😂");
    } else if (leader_sequence_five_keys(KC_S, KC_H, KC_O, KC_C, KC_K)) {
        // Leader, s, h, o, c, k => Types the shock emoji: 😱
        send_unicode_string("😱");
    } else if (leader_sequence_three_keys(KC_S, KC_A, KC_D)) {
        // Leader, s, a, d => Types the sad emoji: 😢
        send_unicode_string("😢");
    } else if (leader_sequence_four_keys(KC_W, KC_I, KC_N, KC_K)) {
        // Leader, w, i, n, k => Types the wink emoji: 😉
        send_unicode_string("😉");
    } else if (leader_sequence_five_keys(KC_S, KC_H, KC_R, KC_U, KC_G)) {
        // Leader, s, h, r, u, g => Types the shrug emoji: 🤷
        send_unicode_string("🤷");
    } else if (leader_sequence_five_keys(KC_T, KC_H, KC_I, KC_N, KC_K)) {
        // Leader, t, h, i, n, k => Types the thinking emoji: 🤔
        send_unicode_string("🤔");
    } else if (leader_sequence_five_keys(KC_T, KC_H, KC_U, KC_M, KC_B)) {
        // Leader, t, h, u, m, b => Types the thumbs up emoji: 👍
        send_unicode_string("👍");
    } else if (leader_sequence_five_keys(KC_T, KC_H, KC_U, KC_M, KC_D)) {
        // Leader, t, h, u, m, d => Types the thumbs down emoji: 👎
        send_unicode_string("👎");
    } else if (leader_sequence_five_keys(KC_H, KC_E, KC_A, KC_R, KC_T)) {
        // Leader, h, e, a, r, t => Types the heart emoji: ❤️
        send_unicode_string("❤️");
    } else if (leader_sequence_four_keys(KC_S, KC_T, KC_A, KC_R)) {
        // Leader, s, t, a, r => Types the star emoji: ⭐
        send_unicode_string("⭐");
    } else if (leader_sequence_five_keys(KC_C, KC_H, KC_E, KC_C, KC_K)) {
        // Leader, c, h, e, c, k => Types the check emoji: ✅
        send_unicode_string("✅");
    } else if (leader_sequence_five_keys(KC_C, KC_R, KC_O, KC_S, KC_S)) {
        // Leader, c, r, o, s, s => Types the cross emoji: ❌
        send_unicode_string("❌");
    } else if (leader_sequence_four_keys(KC_H, KC_U, KC_N, KC_D)) {
        // Leader, h, u, n, d, r => Types the hundred emoji: 💯
        send_unicode_string("💯");
    } else if (leader_sequence_five_keys(KC_B, KC_U, KC_L, KC_L, KC_T)) {
        // Leader, b, u, l, l, t => Types the bullet emoji: •
        send_unicode_string("•");
    } else if(leader_sequence_four_keys(KC_W, KC_A, KC_R, KC_N)) {
        // Leader, w, a, r, n => Types the warning emoji: ⚠️
        send_unicode_string("⚠️");
    } else if (leader_sequence_four_keys(KC_F, KC_I, KC_R, KC_E)) {
        // Leader, f, i, r, e => Types the fire emoji: 🔥
        send_unicode_string("🔥");
    } else if (leader_sequence_four_keys(KC_P, KC_O, KC_O, KC_P)) {
        // Leader, p, o, o, p => Types the poop emoji: 💩
        send_unicode_string("💩");
    } else if (leader_sequence_four_keys(KC_C, KC_L, KC_A, KC_P)) {
        // Leader, c, l, a, p => Types the clap emoji: 👏
        send_unicode_string("👏");
    } else if (leader_sequence_four_keys(KC_B, KC_U, KC_L, KC_L)) {
        // Leader, b, u, l, l => Types the bullet emoji: •
        send_unicode_string("•");
    } else if (leader_sequence_four_keys(KC_D, KC_A, KC_S, KC_H)) {
        // Leader, d, a, s, h => Types the dash emoji: —
        send_unicode_string("—");
    } else if (leader_sequence_five_keys(KC_E, KC_L, KC_L, KC_I, KC_P)) {
        // Leader, e, l, l, i, p => Types the ellipsis emoji: …
        send_unicode_string("…");
    } else if (leader_sequence_five_keys(KC_C, KC_O, KC_P, KC_Y, KC_R)) {
        // Leader, c, o, p, y, r => Types the copyright symbol: ©
        send_unicode_string("©");
    } else if (leader_sequence_five_keys(KC_R, KC_E, KC_G, KC_I, KC_S)) {
        // Leader, r, e, g, i, s => Types the registered symbol: ®
        send_unicode_string("®");
    } else if (leader_sequence_five_keys(KC_T, KC_R, KC_A, KC_D, KC_E)) {
        // Leader, t, r, a, d, e => Types the trademark symbol: ™
        send_unicode_string("™");
    } else if (leader_sequence_five_keys(KC_S, KC_E, KC_C, KC_T, KC_N)) {
        // Leader, s, e, c, t, n => Types the section symbol: §
        send_unicode_string("§");
    } else if (leader_sequence_four_keys(KC_P, KC_A, KC_R, KC_A)) {
        // Leader, p, a, r, a => Types the paragraph symbol: ¶
        send_unicode_string("¶");
    } else if (leader_sequence_four_keys(KC_E, KC_U, KC_R, KC_O)) {
        // Leader, e, u, r, o => Types the euro symbol
        send_unicode_string("€");
    } else if (leader_sequence_four_keys(KC_W, KC_I, KC_N, KC_U)) {
        // Leader, w, i, n, u => Activate the Windows Unicode input mode
        set_unicode_input_mode(UNICODE_MODE_WINDOWS);
    } else if (leader_sequence_four_keys(KC_M, KC_A, KC_C, KC_U)) {
        // Leader, m, a, c, u => Activate the MacOS Unicode input mode
        set_unicode_input_mode(UNICODE_MODE_MACOS);
    } else if (leader_sequence_four_keys(KC_L, KC_I, KC_N, KC_U)) {
        // Leader, l, i, n, u => Activate the Linux Unicode input mode
        set_unicode_input_mode(UNICODE_MODE_LINUX);
    } else if (leader_sequence_three_keys(KC_L, KC_A, KC_R)) {
        // Leader, l, a, r => Types a left arrow emoji: ←
        send_unicode_string("←");
    } else if (leader_sequence_three_keys(KC_R, KC_A, KC_R)) {
        // Leader, r, a, r => Types a right arrow emoji: →
        send_unicode_string("→");
    } else if (leader_sequence_three_keys(KC_U, KC_A, KC_R)) {
        // Leader, u, a, r => Types an up arrow emoji: ↑
        send_unicode_string("↑");
    } else if (leader_sequence_three_keys(KC_D, KC_A, KC_R)) {
        // Leader, d, a, r => Types a down arrow emoji: ↓
        send_unicode_string("↓");
    } else if (leader_sequence_four_keys(KC_U, KC_L, KC_A, KC_R)) {
        // Leader, u, l, a, r => Types an up-left arrow emoji: ↖
        send_unicode_string("↖");
    } else if (leader_sequence_four_keys(KC_U, KC_R, KC_A, KC_R)) {
        // Leader, u, r, a, r => Types an up-right arrow emoji: ↗
        send_unicode_string("↗");
    } else if (leader_sequence_four_keys(KC_D, KC_L, KC_A, KC_R)) {
        // Leader, d, l, a, r => Types a down-left arrow emoji: ↙
        send_unicode_string("↙");
    } else if (leader_sequence_four_keys(KC_D, KC_R, KC_A, KC_R)) {
        // Leader, d, r, a, r => Types a down-right arrow emoji: ↘
        send_unicode_string("↘");
    } else if(leader_sequence_five_keys(KC_N, KC_W, KC_O, KC_R, KC_D)) {
        // Leader, n, w, o, r, d => Skips to next word
        SEND_STRING(
            SS_LCTL(
                SS_TAP(X_RGHT)));
    } else if(leader_sequence_five_keys(KC_P, KC_W, KC_O, KC_R, KC_D)) {
        // Leader, p, w, o, r, d => Skips to previous word
        SEND_STRING(
            SS_LCTL(
                SS_TAP(X_LEFT)));
    }
}

/* ##################################################################### */
/* ####################### RGB MATRIX INDICATORS ####################### */
/* ##################################################################### */
// This function is called to set the RGB matrix indicators.
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Layer indicators
    switch(get_highest_layer(layer_state|default_layer_state)) {
    case 7:
#ifdef FN_LAYER_LED_INDICATOR
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_BLUE); // Set all LEDs to blue
        }
#endif
        // WASD Arrow keys
#ifdef WASD_ARROW_KEYS
        RGB_MATRIX_INDICATOR_SET_COLOR(W_KEY_INDEX, 255, 75, 75);
        RGB_MATRIX_INDICATOR_SET_COLOR(A_KEY_INDEX, 255, 75, 75);
        RGB_MATRIX_INDICATOR_SET_COLOR(S_KEY_INDEX, 255, 75, 75);
        RGB_MATRIX_INDICATOR_SET_COLOR(D_KEY_INDEX, 255, 75, 75);
#endif
        // Number Keys
#ifdef BASIC_NUMBER_ROW
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_1_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_2_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_3_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_4_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_5_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_6_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_7_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_8_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_9_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_0_INDEX, 75, 75, 255);
#endif
        // Pink Fn7 key indicator
#ifdef FN7_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(FN7_INDEX, 255, 75, 75);
#endif
        break;
    case 1:
#ifdef FN_LAYER_LED_INDICATOR
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_PURPLE); // Set all LEDs to purple
        }
#endif
        // Inverted F keys
#ifdef BASIC_FKEY_ROW
        RGB_MATRIX_INDICATOR_SET_COLOR(F5_KEY_INDEX, 255, 75, 75);
        RGB_MATRIX_INDICATOR_SET_COLOR(F6_KEY_INDEX, 255, 75, 75);
        RGB_MATRIX_INDICATOR_SET_COLOR(F10_KEY_INDEX, 255, 75, 75);
        RGB_MATRIX_INDICATOR_SET_COLOR(F11_KEY_INDEX, 255, 75, 75);
        RGB_MATRIX_INDICATOR_SET_COLOR(F12_KEY_INDEX, 255, 75, 75);
#endif
        // Wireless keys
#ifdef KEYCHRON_WIRELESS
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_1_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_2_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_3_INDEX, 75, 75, 255);
        RGB_MATRIX_INDICATOR_SET_COLOR(KEY_4_INDEX, 255, 255, 75);
#endif
        // Escape key
        RGB_MATRIX_INDICATOR_SET_COLOR(ESC_KEY_INDEX, 0, 255, 125);
        // Pink Fn key indicators
#ifdef LFN_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(LFN_INDEX, 255, 75, 75);
#endif
#ifdef RFN_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(RFN_INDEX, 255, 75, 75);
#endif
        break;
    case 4:
#ifdef BASE_LAYER_LED_INDICATOR
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_WHITE); // Set all LEDs to white
        }
#endif
        break;
    case 2:
#ifdef BASE_LAYER_LED_INDICATOR
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_WHITE); // Set all LEDs to white
        }
#endif
        break;
    case 0:
#ifdef BASE_LAYER_LED_INDICATOR
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_WHITE); // Set all LEDs to white
        }
#endif
        break;
    default:
        break;
    }
    // Pink LED indicators for Shift keys IF:
    if(get_mods() & MOD_MASK_SHIFT || // Shift is held or
        get_oneshot_mods() & MOD_MASK_SHIFT || // Shift is oneshot or
        is_caps_word_on() || // Caps Word is active or
        host_keyboard_led_state().caps_lock ) { // Caps Lock is on
#ifdef LSHIFT_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(LSHIFT_INDEX, 255, 75, 75);
#endif
#ifdef RSHIFT_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(RSHIFT_INDEX, 255, 75, 75);
#endif
    }
    // Pink Alt indicators
    if(get_mods() & MOD_MASK_ALT || get_oneshot_mods() & MOD_MASK_ALT) {
#ifdef LALT_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(LALT_INDEX, 255, 75, 75);
#endif
#ifdef RALT_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(RALT_INDEX, 255, 75, 75);
#endif
    }
    // Pink Ctrl indicators
    if(get_mods() & MOD_MASK_CTRL || get_oneshot_mods() & MOD_MASK_CTRL) {
#ifdef LCTRL_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(LCTRL_INDEX, 255, 75, 75);
#endif
#ifdef RCTRL_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(RCTRL_INDEX, 255, 75, 75);
#endif
    }
    // Pink GUI indicators
    if(get_mods() & MOD_MASK_GUI || get_oneshot_mods() & MOD_MASK_GUI) {
#ifdef LGUI_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(LGUI_INDEX, 255, 75, 75);
#endif
#ifdef RGUI_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(RGUI_INDEX, 255, 75, 75);
#endif
    }
    // Pink Hyper indicators: Ctrl+Alt+Shift+GUI
    if(get_mods() & MOD_MASK_CSAG || get_oneshot_mods() & MOD_MASK_CSAG) {
#ifdef LHYPER_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(LHYPER_INDEX, 255, 75, 75);
#endif
#ifdef RHYPER_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(RHYPER_INDEX, 255, 75, 75);
#endif
    }
    // Pink Meh indicators: Ctrl+Alt+Shift
    if(get_mods() & MOD_MASK_CSA || get_oneshot_mods() & MOD_MASK_CSA) {
#ifdef LMEH_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(LMEH_INDEX, 255, 75, 75);
#endif
#ifdef RMEH_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(RMEH_INDEX, 255, 75, 75);
#endif
    }
    // Pink CAG indicators: Ctrl+Alt+GUI
    if(get_mods() & MOD_MASK_CAG || get_oneshot_mods() & MOD_MASK_CAG) {
#ifdef LCAG_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(LCAG_INDEX, 255, 75, 75);
#endif
#ifdef RCAG_INDEX
        RGB_MATRIX_INDICATOR_SET_COLOR(RCAG_INDEX, 255, 75, 75);
#endif
    }
    return false;
}
