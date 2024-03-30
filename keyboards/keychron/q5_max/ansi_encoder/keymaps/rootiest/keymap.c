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
#include "os_detection.h" // OS detection
#include "features/select_word.h" // Select word feature
#include "features/mouse_turbo_click.h" // Mouse turbo click feature
#include "features/layer_lock.h" // Layer lock feature
#include "features/achordion.h" // Achordion feature
#include "features/sentence_case.h" // Sentence case feature
#include "features/vscode_macros.h" // VSCode macros

/* ##################################################################### */
/* ######################### Layer Definitions ######################### */
/* ##################################################################### */
enum layers {
    MAIN_BASE,  // Main layer
    MAIN_FN,    // Main function layer
    PLAIN_BASE, // Plain layer
    PLAIN_FN,   // Plain function layer
    EXT_BASE,    // Extended base layer
    PREFIX_C_K, // Prefix Ctrl + K
    LAYER_6,    // Layer 6
    SPECIAL_FN, // Special function layer
};

/* ##################################################################### */
/* ##################### Custom keycode definitions #################### */
/* ##################################################################### */
enum custom_keycodes{
    // Tap Dance
    TD_ESC_CAPS,     // Tap once for Escape, twice for Caps Lock
    TD_PG_HOME,      // Tap once for Page Up, twice for Home
    TD_PG_END,       // Tap once for Page Down, twice for End
    TD_HOME_PG,      // Tap once for Home, twice for Page Up
    TD_END_PG,       // Tap once for End, twice for Page Down
    TD_HOME_END,     // Tap once for Home, twice for End
    TD_OPEN,         // Tap once for (, twice for [, thrice for {, four times for <
    TD_CLOSE,        // Tap once for ), twice for ], thrice for }, four times for >
    TD_P_OPEN_CLOSE, // Tap once for (, twice for )
    TD_B_OPEN_CLOSE, // Tap once for [, twice for ]
    TD_C_OPEN_CLOSE, // Tap once for {, twice for }
    TD_STICKY_SHIFT, // Tap five times for Caps Lock
    TD_NUM_CALC,     // Tap once for Num Lock, twice for Calculator
    TD_NUM_CLEAR,    // Tap once for Num Lock, twice for Clear
    // Rootiest Custom Keycodes
#ifdef VIA_ENABLE // If you are using VIA
    SS_EMAIL = QK_USER_0, // Send email address
#else // If you are not using VIA
    SS_EMAIL = SAFE_RANGE,    // Send email address
#endif
    SS_NAME,     // Send full name
    SS_NICK,     // Send nickname
    SS_PHONE,    // Send phone number
    SS_GITHUB,   // Send GitHub profile
    SS_WEB,      // Send rootiest website
    SS_PERSONAL, // Send personal email address
    SS_ADDRESS,  // Send personal address
    SELWORD,     // Select word
    NEXTWORD,    // Move to the next word
    PREVWORD,    // Move to the previous word
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
    VSC_CMD = VSCODE_COMMAND_KEY, // VSCode command key
};

/* ##################################################################### */
/* #################### Define Unicode Lookup Table #################### */
/* ##################################################################### */
const ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE(
    UCIS_SYM("rofl", 0x1F923),                 // 🤣
    UCIS_SYM("ukr", 0x1F1FA, 0x1F1E6),         // 🇺🇦
    UCIS_SYM("look", 0x0CA0, 0x005F, 0x0CA0),  // ಠ_ಠ
    // Commonly-used Unicode symbols
    UCIS_SYM("degree_f", 0x2109),              // ℉
    UCIS_SYM("degree_c", 0x2103),              // ℃
    UCIS_SYM("degree", 0x00B0),                // °
    UCIS_SYM("arrow", 0x2192),                 // →
    // Common Unicode emojis
    UCIS_SYM("us_flag", 0x1F1FA, 0x1F1F8),     // 🇺🇸
    UCIS_SYM("smile", 0x1F642),                // 🙂
    UCIS_SYM("frown", 0x1F641),                // 🙁
    UCIS_SYM("lol", 0x1F602),                  // 😂
    UCIS_SYM("shock", 0x1F631),                // 😱
    UCIS_SYM("sad", 0x1F622),                  // 😢
    UCIS_SYM("wink", 0x1F609),                 // 😉
    UCIS_SYM("shrug", 0x1F937),                // 🤷
    UCIS_SYM("think", 0x1F914),                // 🤔
    UCIS_SYM("thumbs_up", 0x1F44D),            // 👍
    UCIS_SYM("thumbs_down", 0x1F44E),          // 👎
    UCIS_SYM("heart", 0x2764),                 // ❤️
    UCIS_SYM("star", 0x2B50),                  // ⭐
    UCIS_SYM("check", 0x2705),                 // ✅
    UCIS_SYM("cross", 0x274C),                 // ❌
    UCIS_SYM("hundred", 0x1F4AF),              // 💯
    UCIS_SYM("warning", 0x26A0),               // ⚠️
    UCIS_SYM("fire", 0x1F525),                 // 🔥
    UCIS_SYM("clap", 0x1F44F),                 // 👏
    UCIS_SYM("poop", 0x1F4A9),                 // 💩
    // More Unicode symbols
    UCIS_SYM("bullet", 0x2022),                // •
    UCIS_SYM("dash", 0x2014),                  // —
    UCIS_SYM("ellipsis", 0x2026),              // …
    UCIS_SYM("copyright", 0x00A9),             // ©
    UCIS_SYM("registered", 0x00AE),            // ®
    UCIS_SYM("trademark", 0x2122),             // ™
    UCIS_SYM("section", 0x00A7),               // §
    UCIS_SYM("paragraph", 0x00B6),             // ¶
    // Money and currency symbols
    UCIS_SYM("euro", 0x20AC),                  // €
    UCIS_SYM("pound", 0x00A3),                 // £
    UCIS_SYM("yen", 0x00A5),                   // ¥
    UCIS_SYM("dollar", 0x0024),                // $
    UCIS_SYM("cent", 0x00A2),                  // ¢
    UCIS_SYM("won", 0x20A9),                   // ₩
    UCIS_SYM("rupee", 0x20B9),                 // ₹
    UCIS_SYM("ruble", 0x20BD),                 // ₽
    UCIS_SYM("lira", 0x20BA),                  // ₺
    UCIS_SYM("bitcoin", 0x20BF),               // ₿
    UCIS_SYM("ethereum", 0x20B3),              // ₳
    UCIS_SYM("litecoin", 0x20B6),              // ₶
    UCIS_SYM("dash", 0x20A7),                  // ₧
    UCIS_SYM("peso", 0x20B1),                  // ₱
    UCIS_SYM("dong", 0x20AB),                  // ₫
    UCIS_SYM("hryvnia", 0x20B4),               // ₴
    UCIS_SYM("naira", 0x20A6),                 // ₦
    UCIS_SYM("rial", 0x20A0),                  // ₠
    UCIS_SYM("baht", 0x20B8),                  // ฿
    UCIS_SYM("lari", 0x20BE),                  // ₾
    UCIS_SYM("peseta", 0x20A7),                // ₧
    UCIS_SYM("krona", 0x20A1),                 // ₡
    UCIS_SYM("franc", 0x20A3),                 // ₣
    UCIS_SYM("florin", 0x20A2),                // ₢
    UCIS_SYM("guarani", 0x20B2),               // ₲
    UCIS_SYM("kip", 0x20AD),                   // ₭
    UCIS_SYM("manat", 0x20BC),                 // ₼
    UCIS_SYM("ngultrum", 0x20B8),              // ₸
    UCIS_SYM("pula", 0x20B1),                  // ₱
    UCIS_SYM("rufiyaa", 0x20A8),               // ₨
    UCIS_SYM("taka", 0x20A6),                  // ₦
    UCIS_SYM("tugrik", 0x20AE),                // ₮
    UCIS_SYM("vatu", 0x20B0),                  // ₰
    UCIS_SYM("watt", 0x20A7),                  // ₧
    UCIS_SYM("afghani", 0x20AF),               // ₯
    UCIS_SYM("cedi", 0x20B5),                  // ₵
    UCIS_SYM("dalasi", 0x20A0),                // ₠
    UCIS_SYM("denar", 0x20AF),                 // ₯
    UCIS_SYM("dinar", 0x20AA),                 // ₪
    UCIS_SYM("dirham", 0x20AED),               // د.إ
    UCIS_SYM("dobra", 0x20B3),                  // ₳
    // Extended Unicode emojis
    UCIS_SYM("hearteyes", 0x1F60D),            // 😍
    UCIS_SYM("kiss", 0x1F618),                 // 😘
    UCIS_SYM("tongue", 0x1F61B),               // 😛
    UCIS_SYM("sunglasses", 0x1F60E),           // 😎
    UCIS_SYM("smirk", 0x1F60F),                // 😏
    UCIS_SYM("sweat", 0x1F605),                // 😅
    UCIS_SYM("laugh", 0x1F606),                // 😆
    UCIS_SYM("cry", 0x1F62D),                  // 😭
    UCIS_SYM("scream", 0x1F631),               // 😱
    UCIS_SYM("angry", 0x1F620),                // 😠
    UCIS_SYM("confused", 0x1F615),             // 😕
    UCIS_SYM("worried", 0x1F61F),              // 😟
    UCIS_SYM("disappointed", 0x1F61E),         // 😞
    UCIS_SYM("sleepy", 0x1F634),               // 😴
    UCIS_SYM("sick", 0x1F637),                 // 😷
    UCIS_SYM("mask", 0x1F637),                 // 😷
    UCIS_SYM("cat", 0x1F638),                  // 😸
    UCIS_SYM("dog", 0x1F415),                  // 🐕
    UCIS_SYM("panda", 0x1F43C),                // 🐼
    UCIS_SYM("bear", 0x1F43B),                 // 🐻
    UCIS_SYM("pig", 0x1F437),                  // 🐷
    UCIS_SYM("cow", 0x1F42E),                  // 🐮
    UCIS_SYM("chicken", 0x1F414),              // 🐔
    UCIS_SYM("frog", 0x1F438),                 // 🐸
    UCIS_SYM("bee", 0x1F41D),                  // 🐝
    UCIS_SYM("octopus", 0x1F419),              // 🐙
    UCIS_SYM("snail", 0x1F40C),                // 🐌
    UCIS_SYM("bug", 0x1F41B),                  // 🐛
    UCIS_SYM("ant", 0x1F41C),                  // 🐜
    UCIS_SYM("spider", 0x1F577),               // 🕷
    UCIS_SYM("scorpion", 0x1F982),             // 🦂
    UCIS_SYM("crab", 0x1F980),                 // 🦀
    UCIS_SYM("snake", 0x1F40D),                // 🐍
    UCIS_SYM("turtle", 0x1F422),               // 🐢
    UCIS_SYM("lizard", 0x1F98E),               // 🦎
    UCIS_SYM("dolphin", 0x1F42C),              // 🐬
    UCIS_SYM("shark", 0x1F988),                // 🦈
    UCIS_SYM("fish", 0x1F41F),                 // 🐟
    UCIS_SYM("whale", 0x1F433),                // 🐳
    UCIS_SYM("crocodile", 0x1F40A),            // 🐊
    UCIS_SYM("dragon", 0x1F409),               // 🐉
    UCIS_SYM("unicorn", 0x1F984),              // 🦄
    UCIS_SYM("horse", 0x1F434),                // 🐴
    UCIS_SYM("camel", 0x1F42A),                // 🐪
    UCIS_SYM("elephant", 0x1F418),             // 🐘
    UCIS_SYM("rhino", 0x1F98F),                // 🦏
    UCIS_SYM("hippo", 0x1F99B),                // 🦛
    UCIS_SYM("lion", 0x1F981),                 // 🦁
    UCIS_SYM("tiger", 0x1F42F),                // 🐯
    UCIS_SYM("leopard", 0x1F406),              // 🐆
    UCIS_SYM("parrrot", 0x1F99C),              // 🦜
    UCIS_SYM("penguin", 0x1F427),              // 🐧
    UCIS_SYM("owl", 0x1F989),                  // 🦉
    UCIS_SYM("dove", 0x1F54A),                 // 🕊
    UCIS_SYM("eagle", 0x1F985),                // 🦅
    UCIS_SYM("duck", 0x1F986),                 // 🦆
    UCIS_SYM("swan", 0x1F9A2),                 // 🦢
    UCIS_SYM("flamingo", 0x1F9A9),             // 🦩
    UCIS_SYM("peacock", 0x1F99A),              // 🦚
    UCIS_SYM("rooster", 0x1F413),              // 🐓
    UCIS_SYM("turkey", 0x1F983),               // 🦃
    UCIS_SYM("bat", 0x1F987),                  // 🦇
    UCIS_SYM("rat", 0x1F400),                  // 🐀
    UCIS_SYM("mouse", 0x1F401),                // 🐁
    UCIS_SYM("hamster", 0x1F439),              // 🐹
    UCIS_SYM("rabbit", 0x1F430),               // 🐰
    UCIS_SYM("chipmunk", 0x1F43F),             // 🐿
    UCIS_SYM("hedgehog", 0x1F994),             // 🦔
    UCIS_SYM("paw", 0x1F43E),                 // 🐾
    UCIS_SYM("paw_prints", 0x1F43E),           // 🐾
    UCIS_SYM("monkey", 0x1F435),               // 🐵
    UCIS_SYM("gorilla", 0x1F98D),              // 🦍
    UCIS_SYM("orangutan", 0x1F9A7),            // 🦧
    UCIS_SYM("dog2", 0x1F415),                 // 🐕
    UCIS_SYM("guide_dog", 0x1F9AE),            // 🦮
    UCIS_SYM("service_dog", 0x1F415),          // 🐕
    UCIS_SYM("poodle", 0x1F429),               // 🐩
    UCIS_SYM("wolf", 0x1F43A),                 // 🐺
    UCIS_SYM("fox", 0x1F98A),                  // 🦊
    UCIS_SYM("raccoon", 0x1F99D),              // 🦝
    UCIS_SYM("badger", 0x1F9A1),               // 🦡
    UCIS_SYM("bear2", 0x1F43B),                // 🐻
    UCIS_SYM("koala", 0x1F428),                // 🐨
    UCIS_SYM("panda_face", 0x1F43C),           // 🐼
    UCIS_SYM("sloth", 0x1F9A5),                // 🦥
    UCIS_SYM("otter", 0x1F9A6),                // 🦦
    UCIS_SYM("skunk", 0x1F9A8),                // 🦨
    UCIS_SYM("kangaroo", 0x1F998),             // 🦘
    UCIS_SYM("elephant2", 0x1F418),            // 🐘
    UCIS_SYM("flower", 0x1F33A),               // 🌺
    UCIS_SYM("rose", 0x1F339),                 // 🌹
    UCIS_SYM("sunflower", 0x1F33B),            // 🌻
    UCIS_SYM("tulip", 0x1F337),                // 🌷
    UCIS_SYM("daisy", 0x1F33E),                // 🌾
    UCIS_SYM("hibiscus", 0x1F33C),             // 🌼
    UCIS_SYM("cherry", 0x1F352),               // 🍒
    UCIS_SYM("strawberry", 0x1F353),           // 🍓
    UCIS_SYM("banana", 0x1F34C),               // 🍌
    UCIS_SYM("apple", 0x1F34E),                // 🍎
    UCIS_SYM("pear", 0x1F350),                 // 🍐
    UCIS_SYM("peach", 0x1F351),                // 🍑
    UCIS_SYM("grapes", 0x1F347),               // 🍇
    UCIS_SYM("melon", 0x1F348),                // 🍈
    UCIS_SYM("watermelon", 0x1F349),           // 🍉
    UCIS_SYM("lemon", 0x1F34B),                // 🍋
    UCIS_SYM("pineapple", 0x1F34D),            // 🍍
    UCIS_SYM("kiwi", 0x1F95D),                 // 🥝
    UCIS_SYM("avocado", 0x1F951),              // 🥑
    UCIS_SYM("tomato", 0x1F345),               // 🍅
    UCIS_SYM("eggplant", 0x1F346),             // 🍆
    UCIS_SYM("potato", 0x1F954),               // 🥔
    UCIS_SYM("carrot", 0x1F955),               // 🥕
    UCIS_SYM("corn", 0x1F33D),                 // 🌽
    UCIS_SYM("cucumber", 0x1F952),             // 🥒
    UCIS_SYM("pepper", 0x1F336),               // 🌶
    UCIS_SYM("mushroom", 0x1F344),             // 🍄
    UCIS_SYM("peanut", 0x1F95C),               // 🥜
    UCIS_SYM("bread", 0x1F35E),                // 🍞
    UCIS_SYM("croissant", 0x1F950),            // 🥐
    UCIS_SYM("baguette", 0x1F956),             // 🥖
    UCIS_SYM("pancakes", 0x1F95E),             // 🥞
    UCIS_SYM("cheese", 0x1F9C0),               // 🧀
    UCIS_SYM("bacon", 0x1F953),                // 🥓
    UCIS_SYM("egg", 0x1F95A),                  // 🥚
    UCIS_SYM("fried_egg", 0x1F373),            // 🍳
    UCIS_SYM("pancake", 0x1F95E),              // 🥞
    UCIS_SYM("butter", 0x1F9C8),               // 🧈
    UCIS_SYM("honey", 0x1F36F),                // 🍯
    UCIS_SYM("milk", 0x1F95B),                 // 🥛
    UCIS_SYM("coffee", 0x2615),                // ☕
    UCIS_SYM("tea", 0x1F375),                  // 🍵
    UCIS_SYM("sake", 0x1F376),                 // 🍶
    UCIS_SYM("beer", 0x1F37A),                 // 🍺
    UCIS_SYM("wine", 0x1F377),                 // 🍷
    UCIS_SYM("cocktail", 0x1F378),             // 🍸
    UCIS_SYM("tropical_drink", 0x1F379),       // 🍹
    UCIS_SYM("champagne", 0x1F37E),            // 🍾
    UCIS_SYM("whiskey", 0x1F943),              // 🥃
    UCIS_SYM("icecream", 0x1F368),             // 🍨
    UCIS_SYM("donut", 0x1F369),                // 🍩
    UCIS_SYM("cookie", 0x1F36A),               // 🍪
    UCIS_SYM("chocolate", 0x1F36B),            // 🍫
    UCIS_SYM("candy", 0x1F36C),                // 🍬
    UCIS_SYM("lollipop", 0x1F36D),             // 🍭
    UCIS_SYM("custard", 0x1F36E),              // 🍮
    UCIS_SYM("popcorn", 0x1F37F),              // 🍿
    UCIS_SYM("airplane", 0x2708),              // ✈️
    UCIS_SYM("helicopter", 0x1F681),           // 🚁
    UCIS_SYM("rocket", 0x1F680),               // 🚀
    UCIS_SYM("satellite", 0x1F6F0),            // 🛰
    UCIS_SYM("car", 0x1F697),                  // 🚗
    UCIS_SYM("taxi", 0x1F695),                 // 🚕
    UCIS_SYM("bus", 0x1F68C),                  // 🚌
    UCIS_SYM("truck", 0x1F69A),                // 🚚
    UCIS_SYM("tractor", 0x1F69C),              // 🚜
    UCIS_SYM("bike", 0x1F6B2),                 // 🚲
    UCIS_SYM("motorcycle", 0x1F3CD),           // 🏍
    UCIS_SYM("train", 0x1F682),                // 🚂
    UCIS_SYM("tram", 0x1F68A),                 // 🚊
    UCIS_SYM("metro", 0x1F687),                // 🚇
    UCIS_SYM("light_rail", 0x1F688),           // 🚈
    UCIS_SYM("station", 0x1F689),              // 🚉
    UCIS_SYM("tram_stop", 0x1F68F),            // 🚏
    UCIS_SYM("bus_stop", 0x1F68D),             // 🚍
    UCIS_SYM("taxi_stand", 0x1F696),           // 🚖
    UCIS_SYM("car_park", 0x1F697),             // 🚗
    UCIS_SYM("bed", 0x1F6CC),                  // 🛌
    UCIS_SYM("couch", 0x1F6CB),                // 🛋
    UCIS_SYM("chair", 0x1FA91),                // 🪑
    UCIS_SYM("toilet", 0x1F6BD),               // 🚽
    UCIS_SYM("shower", 0x1F6BF),               // 🚿
    UCIS_SYM("bathtub", 0x1F6C1),              // 🛁
    UCIS_SYM("potable_water", 0x1F6B0),        // 🚰
    UCIS_SYM("shopping_cart", 0x1F6D2),        // 🛒
    UCIS_SYM("luggage", 0x1F9F3),              // 🧳
    UCIS_SYM("bellhop", 0x1F6CE),              // 🛎
    UCIS_SYM("key", 0x1F511),                  // 🔑
    UCIS_SYM("lock", 0x1F512),                 // 🔒
    UCIS_SYM("unlock", 0x1F513),               // 🔓
    UCIS_SYM("bell", 0x1F514),                 // 🔔
    UCIS_SYM("no_bell", 0x1F515),              // 🔕
    UCIS_SYM("no_entry", 0x26D4),              // ⛔
    UCIS_SYM("no_smoking", 0x1F6AD),           // 🚭
    UCIS_SYM("no_littering", 0x1F6AF),         // 🚯
    UCIS_SYM("no_bikes", 0x1F6B3),             // 🚳
    UCIS_SYM("no_skateboards", 0x1F6A7),       // 🚧
    UCIS_SYM("no_pedestrians", 0x1F6B7),       // 🚷
    UCIS_SYM("no_children", 0x1F6B8),          // 🚸
    UCIS_SYM("no_mobiles", 0x1F4F5),           // 📵
    UCIS_SYM("water", 0x1F4A7),                // 💧
    UCIS_SYM("fire_extinguisher", 0x1F9EF),    // 🧯
    UCIS_SYM("fire_alarm", 0x1F6A8),           // 🚨
    UCIS_SYM("first_aid", 0x1F3E5),            // 🏥
    UCIS_SYM("crossed_fingers", 0x1F91E),      // 🤞
    UCIS_SYM("handshake", 0x1F91D),            // 🤝
    UCIS_SYM("sun", 0x2600),                   // ☀️
    UCIS_SYM("moon", 0x1F319),                 // 🌙
    UCIS_SYM("star", 0x2B50),                  // ⭐
    UCIS_SYM("cloud", 0x2601),                 // ☁️
    UCIS_SYM("rain", 0x1F327),                 // 🌧
    UCIS_SYM("snow", 0x1F328),                 // 🌨
    UCIS_SYM("thunder", 0x26A1),               // ⚡
    UCIS_SYM("tornado", 0x1F32A),              // 🌪
    UCIS_SYM("fog", 0x1F32B),                  // 🌫
    UCIS_SYM("wind", 0x1F32C),                 // 🌬
    UCIS_SYM("umbrella", 0x2602),              // ☂️
    UCIS_SYM("thermometer", 0x1F321),          // 🌡
    UCIS_SYM("droplet", 0x1F4A7),              // 💧
    UCIS_SYM("wave", 0x1F30A),                 // 🌊
    UCIS_SYM("volcano", 0x1F30B),              // 🌋
    UCIS_SYM("earthquake", 0x1F30C),           // 🌌
    UCIS_SYM("hurricane", 0x1F300),            // 🌀
    UCIS_SYM("compass", 0x1F9ED),              // 🧭
    UCIS_SYM("map", 0x1F5FA),                  // 🗺
    UCIS_SYM("mountain", 0x26F0),              // ⛰
    UCIS_SYM("camping", 0x1F3D5),              // 🏕
    UCIS_SYM("beach", 0x1F3D6),                // 🏖
    UCIS_SYM("desert", 0x1F3DC),               // 🏜
    UCIS_SYM("island", 0x1F3DD),               // 🏝
    UCIS_SYM("park", 0x1F3DE),                 // 🏞
    UCIS_SYM("stadium", 0x1F3DF),              // 🏟
    UCIS_SYM("city", 0x1F3D9),                 // 🏙
    UCIS_SYM("house", 0x1F3E0),                // 🏠
    UCIS_SYM("apartment", 0x1F3E1),            // 🏡
    UCIS_SYM("office", 0x1F3E2),               // 🏢
    UCIS_SYM("post_office", 0x1F3E3),         // 🏣
    UCIS_SYM("party", 0x1F389),                // 🎉
    UCIS_SYM("birthday", 0x1F382),             // 🎂
    UCIS_SYM("christmas", 0x1F384),            // 🎄
    UCIS_SYM("fireworks", 0x1F386),            // 🎆
    UCIS_SYM("balloon", 0x1F388),              // 🎈
    UCIS_SYM("gift", 0x1F381),                 // 🎁
    UCIS_SYM("ticket", 0x1F3AB),               // 🎫
    UCIS_SYM("trophy", 0x1F3C6),               // 🏆
    UCIS_SYM("medal", 0x1F396),                // 🎖
    UCIS_SYM("rosette", 0x1F3F5),              // 🏵
    UCIS_SYM("ribbon", 0x1F397),               // 🎗
    UCIS_SYM("badge", 0x1F4DB),                // 📛
    UCIS_SYM("flag", 0x1F3F4),                 // 🏴
    UCIS_SYM("banner", 0x1F3F7),               // 🏷
    UCIS_SYM("confetti", 0x1F38A),             // 🎊
    UCIS_SYM("pinata", 0x1FA85),               // 🪅
    UCIS_SYM("pin", 0x1F4CC),                  // 📌
    UCIS_SYM("paperclip", 0x1F4CE),            // 📎
    UCIS_SYM("scissors", 0x2702),              // ✂️
    UCIS_SYM("pen", 0x1F58A),                  // 🖊
    UCIS_SYM("pencil", 0x270F),                // ✏️
    UCIS_SYM("paintbrush", 0x1F58C),           // 🖌
    UCIS_SYM("crayon", 0x1F58D),               // 🖍
    UCIS_SYM("ruler", 0x1F4CF),                // 📏
    UCIS_SYM("notebook", 0x1F4D3),             // 📓
    UCIS_SYM("ledger", 0x1F4D2),               // 📒
    UCIS_SYM("calendar", 0x1F4C5),             // 📅
    UCIS_SYM("clipboard", 0x1F4CB),            // 📋
    UCIS_SYM("file", 0x1F4C1),                 // 📁
    UCIS_SYM("folder", 0x1F4C2),               // 📂
    UCIS_SYM("open_file", 0x1F4C4),            // 📄
    UCIS_SYM("trash", 0x1F5D1),                // 🗑
    UCIS_SYM("lock_with_pen", 0x1F50F),        // 🔏
    UCIS_SYM("lock_with_key", 0x1F510),        // 🔐
    UCIS_SYM("lightbulb", 0x1F4A1),            // 💡
    UCIS_SYM("flashlight", 0x1F526),           // 🔦
    UCIS_SYM("candle", 0x1F56F),               // 🕯
    UCIS_SYM("crown", 0x1F451),                // 👑
    UCIS_SYM("ring", 0x1F48D),                 // 💍
    UCIS_SYM("gem", 0x1F48E),                  // 💎
    UCIS_SYM("money", 0x1F4B0),                // 💰
    UCIS_SYM("credit_card", 0x1F4B3),          // 💳
    UCIS_SYM("receipt", 0x1F9FE),              // 🧾
    UCIS_SYM("chart", 0x1F4C8),                // 📈
    UCIS_SYM("graph", 0x1F4C9),                // 📉
    UCIS_SYM("bar_chart", 0x1F4CA),            // 📊
    UCIS_SYM("pie", 0x1F967),                  // 🥧
    UCIS_SYM("leaf", 0x1F343),                 // 🍃
    UCIS_SYM("tree", 0x1F333),                 // 🌳
    UCIS_SYM("palm_tree", 0x1F334),            // 🌴
    UCIS_SYM("cactus", 0x1F335),               // 🌵
    UCIS_SYM("skull", 0x1F480),                // 💀
    UCIS_SYM("skull_crossbones", 0x2620),      // ☠️
    UCIS_SYM("ghost", 0x1F47B),                // 👻
    UCIS_SYM("alien", 0x1F47D),                // 👽
    UCIS_SYM("robot", 0x1F916),                // 🤖
    UCIS_SYM("zombie", 0x1F9DF),               // 🧟
    UCIS_SYM("santa", 0x1F385),                // 🎅
    UCIS_SYM("elf", 0x1F9DD),                  // 🧝
    UCIS_SYM("vampire", 0x1F9DB),              // 🧛
    UCIS_SYM("wizard", 0x1F9D9),              // 🧙
    UCIS_SYM("mermaid", 0x1F9DC),              // 🧜
    UCIS_SYM("fairy", 0x1F9DA),                // 🧚
    UCIS_SYM("angel", 0x1F47C),                // 👼
    UCIS_SYM("devil", 0x1F47F),                // 👿
    UCIS_SYM("clown", 0x1F921),                // 🤡
    UCIS_SYM("jester", 0x1F9B9),               // 🦹
    UCIS_SYM("mage", 0x1F9D9),                 // 🧙
    UCIS_SYM("ninja", 0x1F977),                // 🥷
    UCIS_SYM("princess", 0x1F478),             // 👸
    UCIS_SYM("prince", 0x1F934),               // 🤴
    UCIS_SYM("bride", 0x1F470),                // 👰
    UCIS_SYM("groom", 0x1F935),                // 🤵
    UCIS_SYM("baby", 0x1F476),                 // 👶
    UCIS_SYM("child", 0x1F9D2),                // 🧒
    UCIS_SYM("left_arrow", 0x2B05),            // ⬅️
    UCIS_SYM("right_arrow", 0x27A1),           // ➡️
    UCIS_SYM("up_arrow", 0x2B06),              // ⬆️
    UCIS_SYM("down_arrow", 0x2B07),            // ⬇️
    UCIS_SYM("up_left_arrow", 0x2196),         // ↖️
    UCIS_SYM("up_right_arrow", 0x2197),        // ↗️
    UCIS_SYM("down_right_arrow", 0x2198),      // ↘️
    UCIS_SYM("down_left_arrow", 0x2199)        // ↙️
);


/* ##################################################################### */
/* ########################### Define Combos ########################### */
/* ##################################################################### */
// Combos are performed by pressing multiple keys simultaneously.
// This is different than tap dance keys, which are performed by tapping keys in sequence.
const uint16_t PROGMEM combo_undo[] = {KC_Z, KC_X, COMBO_END}; // Undo: Z + X
const uint16_t PROGMEM combo_cut[] = {KC_Z, KC_C, COMBO_END}; // Cut: Z + C
const uint16_t PROGMEM combo_copy[] = {KC_X, KC_C, COMBO_END}; // Copy: X + C
const uint16_t PROGMEM combo_paste[] = {KC_C, KC_V, COMBO_END}; // Paste: C + V
const uint16_t PROGMEM combo_select_all[] = {KC_Z, KC_V, COMBO_END}; // Select all: Z + A
const uint16_t PROGMEM combo_shift_lock[] = {KC_LSFT, KC_RSFT, COMBO_END}; // Shift lock: Left Shift + Right Shift
const uint16_t PROGMEM combo_select_word[] = {KC_LEFT_BRACKET, KC_RIGHT_BRACKET, COMBO_END}; // Select word: [ + ]
const uint16_t PROGMEM combo_select_line[] = {KC_SEMICOLON, KC_QUOTE, COMBO_END}; // Select the current line: ; + '
const uint16_t PROGMEM combo_join_lines[] = {KC_COMMA, KC_DOT, COMBO_END}; // Join lines like Vim's `J` command: , + .
const uint16_t PROGMEM combo_autocorrect[] = {KC_A, KC_C, COMBO_END}; // Autocorrect toggle: A + C
const uint16_t PROGMEM combo_search_selection[] = {KC_F, KC_S, COMBO_END}; // Search selection: F + S
const uint16_t PROGMEM combo_up_dir[] = {KC_DOT, KC_SLASH, COMBO_END}; // Go up a directory: . + /

combo_t key_combos[] = {
    COMBO(combo_copy, C(KC_C)),             // Copy: Ctrl + C
    COMBO(combo_paste, C(KC_V)),            // Paste: Ctrl + V
    COMBO(combo_undo, C(KC_Z)),             // Undo: Ctrl + Z
    COMBO(combo_cut, C(KC_X)),              // Cut: Ctrl + X
    COMBO(combo_select_all, C(KC_A)),       // Select all: Ctrl + A
    COMBO(combo_shift_lock, KC_CAPS),       // Shift lock: Caps Lock
    COMBO(combo_autocorrect, AC_TOGG),      // Autocorrect toggle: Alt + C
    COMBO(combo_select_word, SELWORD),      // Select word
    COMBO(combo_select_line, SELLINE),      // Select the current line
    COMBO(combo_join_lines, JOINLN),        // Join lines like Vim's `J` command
    COMBO(combo_search_selection, SRCHSEL), // Search selection
    COMBO(combo_up_dir, UPDIR),             // Go up a directory: ../
};

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
        _______,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    RGB_VAD,    RGB_VAI,    KC_F7,    KC_F8,    KC_F9,    KC_MUTE,     KC_VOLD,   KC_VOLU,             A(KC_F4),  _______,  _______,  _______,    A(KC_F4),
        HEATMAP,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  KC_CALC,  _______,  _______,    _______,
        RGB_MOD,  RGB_TOG,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______, SS_PERSONAL, SS_ADDRESS,
        _______,  C(KC_A), C(KC_S),  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,             _______,  SS_NICK,  SS_WEB,   SS_GITHUB,    _______,
        _______,            C(KC_Z),  C(KC_X),  C(KC_C),  C(KC_V),  BAT_LVL,  NK_TOGG,  _______,  _______,  AC_TOGG,  _______,              _______,  _______,            SS_NAME,  SS_EMAIL,  SS_PHONE,
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
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______, DF(MAIN_BASE), _______,  _______,    _______,
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
    [LAYER_6] = LAYOUT_ansi_101(
        KC_ESC,             KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,   KC_F12,             KC_DEL,   _______,  _______,  _______,    KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,    KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,  KC_P4,    KC_P5,    KC_P6,      KC_PPLS,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(PLAIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,            KC_P0,    KC_PDOT,    KC_PENT),
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
/* ########################### Layer Locking ########################### */
/* ##################################################################### */
// This function is called whenever the layer lock state changes.
void layer_lock_set_user(layer_state_t locked_layers) {
  // Do something like `set_led(is_layer_locked(NAV));`
}

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
/* ####################### Encoder RGB Animation ####################### */
/* ##################################################################### */
// This function is called to set the RGB animation for the encoders.
uint32_t encoder_animation_pattern(uint32_t trigger_time, void *cb_arg) {
    if (cb_arg == NULL) {
        // Display frame1 for 100ms
        rgb_matrix_set_color(16, 255, 75, 75);
        rgb_matrix_set_color(34, 255, 75, 75);
        rgb_matrix_set_color(35, 255, 75, 75);
        wait_ms(100);
        // Display frame2 for 100ms
        rgb_matrix_set_color(15, 255, 0, 0);
        rgb_matrix_set_color(33, 255, 0, 0);
        rgb_matrix_set_color(52, 255, 0, 0);
        rgb_matrix_set_color(53, 255, 0, 0);
        rgb_matrix_set_color(54, 255, 0, 0);
        wait_ms(100);
        // Clear frame1
        rgb_matrix_set_color(16, 0, 0, 0);
        rgb_matrix_set_color(34, 0, 0, 0);
        rgb_matrix_set_color(35, 0, 0, 0);
        wait_ms(100);
        // Clear frame2
        rgb_matrix_set_color(15, 0, 0, 0);
        rgb_matrix_set_color(33, 0, 0, 0);
        rgb_matrix_set_color(52, 0, 0, 0);
        rgb_matrix_set_color(53, 0, 0, 0);
    } else {
        // Display frame2 for 100ms
        rgb_matrix_set_color(15, 255, 0, 0);
        rgb_matrix_set_color(33, 255, 0, 0);
        rgb_matrix_set_color(52, 255, 0, 0);
        rgb_matrix_set_color(53, 255, 0, 0);
        rgb_matrix_set_color(54, 255, 0, 0);
        wait_ms(100);
        // Display frame1 for 100ms
        rgb_matrix_set_color(16, 255, 75, 75);
        rgb_matrix_set_color(34, 255, 75, 75);
        rgb_matrix_set_color(35, 255, 75, 75);
        wait_ms(100);
        // Clear frame2
        rgb_matrix_set_color(15, 0, 0, 0);
        rgb_matrix_set_color(33, 0, 0, 0);
        rgb_matrix_set_color(52, 0, 0, 0);
        rgb_matrix_set_color(53, 0, 0, 0);
        wait_ms(100);
        // Clear frame1
        rgb_matrix_set_color(16, 0, 0, 0);
        rgb_matrix_set_color(34, 0, 0, 0);
        rgb_matrix_set_color(35, 0, 0, 0);
    }
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
    [MAIN_FN]   = {ENCODER_CCW_CW(KC_WH_D, KC_WH_U)}, // Scroll wheel
    [PLAIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume control
    [PLAIN_FN]   = {ENCODER_CCW_CW(RGB_VAD, RGB_VAI)}, // RGB brightness control
    [EXT_BASE]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume control
    [PREFIX_C_K]  = {ENCODER_CCW_CW(C(KC_LEFT), C(KC_RIGHT))}, // Move cursor by word
    [LAYER_6]  = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume control
    [SPECIAL_FN]  = {ENCODER_CCW_CW(KC_WH_L, KC_WH_R)}, // Horizontal scroll wheel
};
#endif // ENCODER_MAP_ENABLE

/* ##################################################################### */
/* ######################## Process Record User ######################## */
/* ##################################################################### */
// Actions performed when a key is pressed or released
// Here we define the bulk of the custom keycodes and features performed
// by a key press or release.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {

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
        case KC_MUTE:  // Mute audio
            if (record->event.pressed) {
                tap_code16(KC_MUTE);
                defer_exec(1, encoder_animation_pattern, NULL);
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
/* ####################### OS detection function ####################### */
/* ##################################################################### */
// This function is called when the keyboard is initialized
// and when the keyboard is connected to a new host.
// It is used to set the Unicode input mode based on the detected OS.
bool process_detected_host_os_user(os_variant_t detected_os) {
    switch (detected_os) {
        case OS_MACOS: // macOS
            //set_unicode_input_mode(UNICODE_MODE_LINUX);
            set_unicode_input_mode(UNICODE_MODE_MACOS);
            tap_code16(QK_MAGIC_SWAP_LCTL_LGUI);
            break;
        case OS_IOS: // iOS
            //set_unicode_input_mode(UNICODE_MODE_LINUX);
            set_unicode_input_mode(UNICODE_MODE_MACOS);
            tap_code16(QK_MAGIC_SWAP_LCTL_LGUI);
            break;
        case OS_WINDOWS: // Windows
            set_unicode_input_mode(UNICODE_MODE_WINDOWS);
            // set_unicode_input_mode(UNICODE_MODE_WINC); // WinCompose mode
            tap_code16(QK_MAGIC_UNSWAP_LCTL_LGUI);
            break;
        case OS_LINUX: // Linux
            set_unicode_input_mode(UNICODE_MODE_LINUX);
            tap_code16(QK_MAGIC_UNSWAP_LCTL_LGUI);
            break;
        case OS_UNSURE: // Unknown OS (or failed to detect)
            set_unicode_input_mode(UNICODE_MODE_LINUX);
            tap_code16(QK_MAGIC_UNSWAP_LCTL_LGUI);
            break;
    }
    return true;
}

/* ##################################################################### */
/* ####################### RGB MATRIX INDICATORS ####################### */
/* ##################################################################### */
// This function is called to set the RGB matrix indicators.
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Layer indicators
    switch(get_highest_layer(layer_state|default_layer_state)) {
    case SPECIAL_FN:
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
    case MAIN_FN:
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
    case EXT_BASE:
#ifdef BASE_LAYER_LED_INDICATOR
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_WHITE); // Set all LEDs to white
        }
#endif
        break;
    case PLAIN_BASE:
#ifdef BASE_LAYER_LED_INDICATOR
        for (uint8_t i = led_min; i < led_max; i++) {
            rgb_matrix_set_color(i, RGB_WHITE); // Set all LEDs to white
        }
#endif
        break;
    case MAIN_BASE:
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
