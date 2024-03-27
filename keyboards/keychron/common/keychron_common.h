/* Copyright 2023 @ Keychron (https://www.keychron.com)
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

#include "stdint.h"

// clang-format off
enum {
    KC_LOPTN = QK_KB_0,
    KC_ROPTN,
    KC_LCMMD,
    KC_RCMMD,
    KC_MCTRL,
    KC_LNPAD,
    KC_TASK_VIEW,
    KC_FILE_EXPLORER,
    KC_SCREEN_SHOT,
    KC_CORTANA,
    KC_SIRI,
#ifdef LK_WIRELESS_ENABLE
    BT_HST1,
    BT_HST2,
    BT_HST3,
    P2P4G,
    BAT_LVL,
#else
    BT_HST1 = _______,
    BT_HST2 = _______,
    BT_HST3 = _______,
    P2P4G   = _______,
    BAT_LVL = _______,
#endif
#ifdef DANANLOG_MATRIX
    PROF1,
    PROF2,
    PROF3,
#else
    PROF1 = _______,
    PROF2 = _______,
    PROF3 = _______,
#endif
    // Custom Keycodes
    SS_EMAIL, // Send email address
    SS_NAME,     // Send full name
    SS_NICK,     // Send nickname
    SS_PHONE,    // Send phone number
    SS_GITHUB,   // Send GitHub profile
    SS_WEB,      // Send rootiest website
    SS_PERSONAL, // Send personal email address
    SS_ADDRESS,  // Send personal address
    SELWORD,     // Select word
    TURBO,       // Mouse turbo click
    LLOCK,       // Layer lock
    JIGGLE,      // Mouse jiggler
    AUTOSAVE,    // Autosaver
    UPDIR,       // Go up a directory
    EQ3X,        // Types triple equal ===
    DBLCLK,      // Double click the left mouse button
    SELLINE,     // Selects the current line
    SRCHSEL,     // Searches the current selection in a new tab
    JOINLN,      // Join lines like Vim's `J` command
    NEXTSEN,     // Next sentence macro
    US_FLAG,     // Types US flag emoji 🇺🇸
    DBLZERO,     // Types double zero
    HEATMAP,     // Enables heatmap mode
    ARROW,       // Types arrow: -> or =>
    DEGREE,      // Types degree symbol: °
    DEGREE_C,    // Types Celsius degree symbol: °C
    DEGREE_F,    // Types Fahrenheit degree symbol: °F
    SMILE,       // Types smile emoji: 🙂
    FROWN,       // Types frown emoji: 🙁
    LOL,         // Types LOL emoji: 😂
    SHOCK,       // Types shock emoji: 😱
    SAD,         // Types sad emoji: 😢
    WINK,        // Types wink emoji: 😉
    SHRUG,       // Types shrug emoji: 🤷
    THINK,       // Types thinking emoji: 🤔
    THUMBS_UP,   // Types thumbs up emoji: 👍
    THUMBS_DOWN, // Types thumbs down emoji: 👎
    HEART,       // Types heart emoji: ❤️
    STAR,        // Types star emoji: ⭐
    CHECK,       // Types check mark emoji: ✅
    CROSS,       // Types cross mark emoji: ❌
    HUNDRED,     // Types hundred emoji: 💯
    WARNING,     // Types warning emoji: ⚠️
    FIRE,        // Types fire emoji: 🔥
    CLAP,        // Types clap emoji: 👏
    POOP,        // Types poop emoji: 💩
    BULLET,      // Types bullet point: •
    DASH,        // Types dash: —
    ELLIPSIS,    // Types ellipsis: …
    COPYRIGHT,   // Types copyright symbol: ©
    REGISTERED,  // Types registered symbol: ®
    TRADEMARK,   // Types trademark symbol: ™
    SECTION,     // Types section symbol: §
    PARAGRAPH,   // Types paragraph symbol: ¶
    EURO,        // Types euro symbol: €
    NEW_SAFE_RANGE,
};

#define KC_TASK KC_TASK_VIEW
#define KC_FILE KC_FILE_EXPLORER
#define KC_SNAP KC_SCREEN_SHOT
#define KC_CTANA KC_CORTANA

typedef struct PACKED {
    uint8_t len;
    uint8_t keycode[3];
} key_combination_t;

bool process_record_keychron_common(uint16_t keycode, keyrecord_t *record);
void keychron_common_task(void);

#ifdef ENCODER_ENABLE
void encoder_cb_init(void);
#endif

