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

#include "quantum.h"
#include "keychron_task.h"
#include "keychron_common.h"
#ifdef FACTORY_TEST_ENABLE
#    include "factory_test.h"
#endif
#ifdef LK_WIRELESS_ENABLE
#    include "lkbt51.h"
#    include "wireless.h"
#    include "keychron_wireless_common.h"
#    include "battery.h"
#endif

bool dip_switch_update_kb(uint8_t index, bool active) {
    if (index == 0) {
        default_layer_set(1UL << (active ? 0 : 2));
    }
    dip_switch_update_user(index, active);

    return true;
}

#ifdef LK_WIRELESS_ENABLE
// Re-apply the saved QMK RGB mode every time wireless connects.
// During transport changes, rgb_matrix_init() reads the correct mode from
// EEPROM, but something in the BT/2.4G reconnect sequence can reset it
// before the display settles.  This hook fires after connection is fully
// established, ensuring the Launcher-configured mode persists.
void wireless_enter_connected_kb(uint8_t host_idx) {
#    if defined(RGB_MATRIX_ENABLE) && defined(KEYCHRON_RGB_ENABLE)
    extern void eeconfig_init_custom_rgb(void);
    eeconfig_init_custom_rgb();

    // Re-read the QMK RGB mode from EEPROM and apply if it drifted.
    rgb_config_t saved_rgb;
    eeprom_read_block(&saved_rgb, EECONFIG_RGB_MATRIX, sizeof(saved_rgb));
    if (saved_rgb.mode && saved_rgb.mode != rgb_matrix_get_mode()) {
        rgb_matrix_mode_noeeprom(saved_rgb.mode);
    }
#    endif
}
#endif

void keyboard_post_init_kb(void) {
#ifdef LK_WIRELESS_ENABLE
    palSetLineMode(P2P4_MODE_SELECT_PIN, PAL_MODE_INPUT);
    palSetLineMode(BT_MODE_SELECT_PIN, PAL_MODE_INPUT);

    lkbt51_init(false);
    wireless_init();
#endif

#ifdef ENCODER_ENABLE
    encoder_cb_init();
#endif

#if defined(RGB_MATRIX_ENABLE) && defined(KEYCHRON_RGB_ENABLE)
    // Load Keychron custom RGB data (effect list, regions, per-key colours)
    // from EEPROM into RAM.  Without this call the arrays are zero-initialised
    // and Launcher settings are lost on every power cycle or transport change.
    extern void eeconfig_init_custom_rgb(void);
    eeconfig_init_custom_rgb();
#endif

    keyboard_post_init_user();
}

#ifdef LK_WIRELESS_ENABLE
bool lpm_is_kb_idle(void) {
    return !factory_reset_indicating();
}
#endif
