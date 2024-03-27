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
