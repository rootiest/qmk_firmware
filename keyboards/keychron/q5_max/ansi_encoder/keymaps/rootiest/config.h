// Copyright (C) 2024 Chris Laprade (chris@rootiest.com)
//
// This file is part of rootiest.
//
// rootiest is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// rootiest is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with rootiest.  If not, see <https://www.gnu.org/licenses/>.



#pragma once

/* Encoder Configuration */
#define ENCODER_DEFAULT_POS 0x3
#define ENCODER_MAP_KEY_DELAY 2

#ifdef LK_WIRELESS_ENABLE
/* Hardware configuration */
#    define P2P4_MODE_SELECT_PIN A10
#    define BT_MODE_SELECT_PIN A9

#    define LKBT51_RESET_PIN C4
#    define LKBT51_INT_INPUT_PIN B1
#    define BLUETOOTH_INT_OUTPUT_PIN A4

#    define USB_POWER_SENSE_PIN B0
#    define USB_POWER_CONNECTED_LEVEL 0

#    define BAT_CHARGING_PIN B13
#    define BAT_CHARGING_LEVEL 0

#    define BT_HOST_DEVICES_COUNT 3

#    if defined(RGB_MATRIX_ENABLE) || defined(LED_MATRIX_ENABLE)

#        define LED_DRIVER_SHUTDOWN_PIN B7

#        define BT_HOST_LED_MATRIX_LIST \
            { 18, 19, 20 }

#        define P2P4G_HOST_LED_MATRIX_LIST \
            { 21 }

#        define BAT_LEVEL_LED_LIST \
            { 18, 19, 20, 21, 22, 23, 24, 25, 26, 27 }

#        define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_TYPING_HEATMAP

/* Backlit disable timeout when keyboard is disconnected(unit: second) */
#        define DISCONNECTED_BACKLIGHT_DISABLE_TIMEOUT 40

/* Backlit disable timeout when keyboard is connected(unit: second) */
#        define CONNECTED_BACKLIGHT_DISABLE_TIMEOUT 600

/* Reinit LED driver on tranport changed */
#        define REINIT_LED_DRIVER 1

#    endif

/* Keep USB connection in blueooth mode */
#    define KEEP_USB_CONNECTION_IN_WIRELESS_MODE

/* Enable bluetooth NKRO */
#    define WIRELESS_NKRO_ENABLE

/* Raw hid command for factory test and bluetooth DFU */
#    define RAW_HID_CMD 0xAA ... 0xAB
#else
/* Raw hid command for factory test */
#    define RAW_HID_CMD 0xAB
#endif

/* Factory test keys */
#define FN_KEY_1 MO(1) // Factory test key 1
#define FN_KEY_2 MO(3) // Factory test key 2

// Matrix scan rate
#define MATRIX_IO_DELAY 10 // 10ms delay for matrix IO.

// Define dynamic keymap layer count for VIA
#define DYNAMIC_KEYMAP_LAYER_COUNT 8 // 8 dynamic keymap layers. (VIA)

// Use default Rootiest Layers
#define USE_ROOTIEST_LAYERS // Use default Rootiest layers.

// Tri-Layer Identifiers
#define TRI_LAYER_LOWER_LAYER 1
#define TRI_LAYER_RAISE_LAYER 7
#define TRI_LAYER_ADJUST_LAYER 5

// Define secure idle timeout
#define SECURE_IDLE_TIMEOUT 0 // Disable secure idle timeout.

// Define select word timeout
#define SELECT_WORD_TIMEOUT 2000 // When idle, clear state after 2 seconds.

// Click every 50 ms for 20 clicks per second.
#define MOUSE_TURBO_CLICK_PERIOD 50 // 50 ms mouse turbo click period.
#define MOUSE_TURBO_CLICK_KEY KC_MS_BTN1 // Mouse button 1.

// Define layer lock idle timeout
#define LAYER_LOCK_IDLE_TIMEOUT 60000 // Turn off after 60 seconds.

// Define sentence case timeout
#define SENTENCE_CASE_TIMEOUT 2000 // Reset state after 2 seconds.

// Leader key configuration
#define LEADER_NO_TIMEOUT // Disable initial timeout.
#define LEADER_PER_KEY_TIMING // Enable per-key timing.
#define LEADER_TIMEOUT 500 // 500ms timeout per key after leader key.

// Define idle timeout
#define IDLE_TIMEOUT_MS 5000  // Idle timeout in milliseconds.

// Define Prefix Key
#define PREFIX_KEY C(KC_K) // Prefix key is Ctrl+K.

// Define Trigger Keys
#define COMM_COPY LT(0, KC_COMM) // Comma key: Tap for comma, hold for copy
#define DOT_PASTE LT(0, KC_DOT) // Dot key: Tap for dot, hold for paste
#define MPLY_MNXT LT(0, KC_MPLY) // Media play key: Tap for play, hold for next track

// Supported Unicode modes
#define UNICODE_SELECTED_MODES UNICODE_MODE_LINUX, UNICODE_MODE_WINDOWS //, UNICODE_MODE_MACOS

// VSCode Command Key combination
#define VSCODE_COMMAND_KEY C(S(KC_P)) // Ctrl+Shift+P

// Backspace/Delete Key Override
//#define USE_BSPC_DEL_OVERRIDE // Enables Shift+Backspace for Delete

// RGB Matrix Indicators
#if defined(RGB_MATRIX_ENABLE) || defined(LED_MATRIX_ENABLE)
// Modifier Keys
#   define LSHIFT_INDEX 72
#   define RSHIFT_INDEX 83
#   define LCTRL_INDEX 89
#   define RCTRL_INDEX 95
#   define LALT_INDEX 91
#   define RALT_INDEX 93
#   define LGUI_INDEX 90
#   define RFN_INDEX 94
#   define FN7_INDEX 99
// WASD Keys
#   define WASD_ARROW_KEYS // Enables WASD key highlights
#   define W_KEY_INDEX 38
#   define A_KEY_INDEX 56
#   define S_KEY_INDEX 57
#   define D_KEY_INDEX 58
// Basic Number Row
#   define BASIC_NUMBER_ROW // Enables basic number row highlights
#   define KEY_1_INDEX 18
#   define KEY_2_INDEX 19
#   define KEY_3_INDEX 20
#   define KEY_4_INDEX 21
#   define KEY_5_INDEX 22
#   define KEY_6_INDEX 23
#   define KEY_7_INDEX 24
#   define KEY_8_INDEX 25
#   define KEY_9_INDEX 26
#   define KEY_0_INDEX 27
// Basic Function Row
#   define BASIC_FKEY_ROW // Enables basic function row highlights
#   define F1_KEY_INDEX 1
#   define F2_KEY_INDEX 2
#   define F3_KEY_INDEX 3
#   define F4_KEY_INDEX 4
#   define F5_KEY_INDEX 5
#   define F6_KEY_INDEX 6
#   define F7_KEY_INDEX 7
#   define F8_KEY_INDEX 8
#   define F9_KEY_INDEX 9
#   define F10_KEY_INDEX 10
#   define F11_KEY_INDEX 11
#   define F12_KEY_INDEX 12
// Escape Key
#   define ESC_KEY_INDEX 0
// Additional configurator toggles
#   define KEYCHRON_WIRELESS // Enables wireless userspace config
#   define FN_LAYER_LED_INDICATOR // Enables Function layers LED indicators
//#   define BASE_LAYER_LED_INDICATOR // Enables Base layers LED indicators
#endif
