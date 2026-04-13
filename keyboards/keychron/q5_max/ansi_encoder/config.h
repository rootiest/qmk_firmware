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

#pragma once

#ifdef RGB_MATRIX_ENABLE
/* RGB Matrix driver configuration */
#    define RGB_MATRIX_LED_COUNT 101
#    define DRIVER_COUNT 2
#    define DRIVER_CS_PINS {B8, B9}

/* Set LED driver current */
#    define SNLED27351_CURRENT_TUNE {0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C}

/* Set to infinit, which is use in USB mode by default */
#    define RGB_MATRIX_TIMEOUT RGB_MATRIX_TIMEOUT_INFINITE
/* Allow shutdown of led driver to save power */
#    define RGB_MATRIX_DRIVER_SHUTDOWN_ENABLE
/* Turn off backlight on low brightness to save power */
#    define RGB_MATRIX_BRIGHTNESS_TURN_OFF_VAL 32

/* Indications */
#    define NUM_LOCK_INDEX 32
#    define CAPS_LOCK_INDEX 55
#    define LOW_BAT_IND_INDEX {92}

#    define RGB_MATRIX_KEYPRESSES
#    define RGB_MATRIX_FRAMEBUFFER_EFFECTS

/* Default lighting effect */
#    define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_TYPING_HEATMAP

#endif

// Pin VIA keymap storage to a fixed EEPROM address.  By default VIA places its
// magic/keymap block immediately after EECONFIG_KB_DATA_SIZE, so any growth in
// the Keychron custom-RGB EEPROM region shifts the keymap silently and corrupts
// the stored layout (observed as layer 0 keys reverting to KC_TRNS on boot).
// 544 is past the current Keychron data region and leaves headroom for further
// EEPROM additions without requiring another VIA reset.
#define VIA_EEPROM_MAGIC_ADDR 544

/* Number of layers */
#define DYNAMIC_KEYMAP_LAYER_COUNT 6

/* Number of taps to toggle layer with TT */
#define TAPPING_TOGGLE 3
