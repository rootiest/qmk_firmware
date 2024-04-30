#pragma once

#include "quantum.h"
#include "action.h"
#include "version.h"

/* ##################################################################### */
/* ######################### Layer Definitions ######################### */
/* ##################################################################### */
// This layer map is used on all the Rootiest keyboards.
// However, the layout of the keys may vary depending on the model.
#ifdef USE_ROOTIEST_LAYERS
enum layers {
    MAIN_BASE,  // Main layer: The default Rootiest keymap
    MAIN_FN,    // Main function layer
    PLAIN_BASE, // Plain layer: A more traditional keymap
    PLAIN_FN,   // Plain function layer
    EXT_BASE,   // Extended base layer: Secondary functions
    PREFIX_C_K, // Prefix Ctrl + K: VSCode chord prefix
    MUSIK,      // Layer 6: Custom layer
    SPECIAL_FN, // Special function layer: Tertiary functions
};
#endif

/* ##################################################################### */
/* ##################### Custom keycode definitions #################### */
/* ##################################################################### */
// These custom keycodes are available on all Rootiest keyboards.
#ifdef USE_ROOTIEST_KEYCODES
enum custom_keycodes {
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
#    ifdef VIA_ENABLE     // If you are using VIA
    SS_EMAIL = QK_USER_0, // Send email address
#    else                 // If you are not using VIA
    SS_EMAIL = SAFE_RANGE, // Send email address
#    endif
    SS_NAME,                      // Send full name
    SS_NICK,                      // Send nickname
    SS_PHONE,                     // Send phone number
    SS_GITHUB,                    // Send GitHub profile
    SS_WEB,                       // Send rootiest website
    SS_PERSONAL,                  // Send personal email address
    SS_ADDRESS,                   // Send personal address
    SELWORD,                      // Select word
    NEXTWORD,                     // Move to the next word
    PREVWORD,                     // Move to the previous word
    TURBO,                        // Mouse turbo click
    LLOCK,                        // Layer lock
    JIGGLE,                       // Mouse jiggler
    AUTOSAVE,                     // Autosaver
    UPDIR,                        // Go up a directory
    EQ3X,                         // Types triple equal ===
    DBLCLK,                       // Double click the left mouse button
    SELLINE,                      // Selects the current line
    SRCHSEL,                      // Searches the current selection in a new tab
    JOINLN,                       // Join lines like Vim's `J` command
    NEXTSEN,                      // Next sentence macro
    US_FLAG,                      // Types US flag emoji 🇺🇸
    DBLZERO,                      // Types double zero
    HEATMAP,                      // Enables heatmap mode
    ARROW,                        // Types arrow: -> or =>
    DEGREE,                       // Types degree symbol: °
    DEGREE_C,                     // Types Celsius degree symbol: °C
    DEGREE_F,                     // Types Fahrenheit degree symbol: °F
    SMILE,                        // Types smile emoji: 🙂
    FROWN,                        // Types frown emoji: 🙁
    LOL,                          // Types LOL emoji: 😂
    SHOCK,                        // Types shock emoji: 😱
    SAD,                          // Types sad emoji: 😢
    WINK,                         // Types wink emoji: 😉
    SHRUG,                        // Types shrug emoji: 🤷
    THINK,                        // Types thinking emoji: 🤔
    THUMBS_UP,                    // Types thumbs up emoji: 👍
    THUMBS_DOWN,                  // Types thumbs down emoji: 👎
    HEART,                        // Types heart emoji: ❤️
    STAR,                         // Types star emoji: ⭐
    CHECK,                        // Types check mark emoji: ✅
    CROSS,                        // Types cross mark emoji: ❌
    HUNDRED,                      // Types hundred emoji: 💯
    WARNING,                      // Types warning emoji: ⚠️
    FIRE,                         // Types fire emoji: 🔥
    CLAP,                         // Types clap emoji: 👏
    POOP,                         // Types poop emoji: 💩
    BULLET,                       // Types bullet point: •
    DASH,                         // Types dash: —
    ELLIPSIS,                     // Types ellipsis: …
    COPYRIGHT,                    // Types copyright symbol: ©
    REGISTERED,                   // Types registered symbol: ®
    TRADEMARK,                    // Types trademark symbol: ™
    SECTION,                      // Types section symbol: §
    PARAGRAPH,                    // Types paragraph symbol: ¶
    EURO,                         // Types euro symbol: €
    VSC_CMD = VSCODE_COMMAND_KEY, // VSCode command key
    ALT4KEY                       // Applies Alt modifier for next 4 keypresses
};
#endif

/* ##################################################################### */
/* #################### Define Unicode Lookup Table #################### */
/* ##################################################################### */
// This table is used to define Unicode symbols that can be easily typed.
#ifdef USE_ROOTIEST_UNICODE_TABLE
const ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE(UCIS_SYM("rofl", 0x1F923),                // 🤣
                                                     UCIS_SYM("ukr", 0x1F1FA, 0x1F1E6),        // 🇺🇦
                                                     UCIS_SYM("look", 0x0CA0, 0x005F, 0x0CA0), // ಠ_ಠ
                                                     // Commonly-used Unicode symbols
                                                     UCIS_SYM("degree_f", 0x2109), // ℉
                                                     UCIS_SYM("degree_c", 0x2103), // ℃
                                                     UCIS_SYM("degree", 0x00B0),   // °
                                                     UCIS_SYM("arrow", 0x2192),    // →
                                                     // Common Unicode emojis
                                                     UCIS_SYM("us_flag", 0x1F1FA, 0x1F1F8), // 🇺🇸
                                                     UCIS_SYM("smile", 0x1F642),            // 🙂
                                                     UCIS_SYM("frown", 0x1F641),            // 🙁
                                                     UCIS_SYM("lol", 0x1F602),              // 😂
                                                     UCIS_SYM("shock", 0x1F631),            // 😱
                                                     UCIS_SYM("sad", 0x1F622),              // 😢
                                                     UCIS_SYM("wink", 0x1F609),             // 😉
                                                     UCIS_SYM("shrug", 0x1F937),            // 🤷
                                                     UCIS_SYM("think", 0x1F914),            // 🤔
                                                     UCIS_SYM("thumbs_up", 0x1F44D),        // 👍
                                                     UCIS_SYM("thumbs_down", 0x1F44E),      // 👎
                                                     UCIS_SYM("heart", 0x2764),             // ❤️
                                                     UCIS_SYM("star", 0x2B50),              // ⭐
                                                     UCIS_SYM("check", 0x2705),             // ✅
                                                     UCIS_SYM("cross", 0x274C),             // ❌
                                                     UCIS_SYM("hundred", 0x1F4AF),          // 💯
                                                     UCIS_SYM("warning", 0x26A0),           // ⚠️
                                                     UCIS_SYM("fire", 0x1F525),             // 🔥
                                                     UCIS_SYM("clap", 0x1F44F),             // 👏
                                                     UCIS_SYM("poop", 0x1F4A9),             // 💩
                                                     // More Unicode symbols
                                                     UCIS_SYM("bullet", 0x2022),     // •
                                                     UCIS_SYM("dash", 0x2014),       // —
                                                     UCIS_SYM("ellipsis", 0x2026),   // …
                                                     UCIS_SYM("copyright", 0x00A9),  // ©
                                                     UCIS_SYM("registered", 0x00AE), // ®
                                                     UCIS_SYM("trademark", 0x2122),  // ™
                                                     UCIS_SYM("section", 0x00A7),    // §
                                                     UCIS_SYM("paragraph", 0x00B6),  // ¶
                                                     // Money and currency symbols
                                                     UCIS_SYM("euro", 0x20AC),     // €
                                                     UCIS_SYM("pound", 0x00A3),    // £
                                                     UCIS_SYM("yen", 0x00A5),      // ¥
                                                     UCIS_SYM("dollar", 0x0024),   // $
                                                     UCIS_SYM("cent", 0x00A2),     // ¢
                                                     UCIS_SYM("won", 0x20A9),      // ₩
                                                     UCIS_SYM("rupee", 0x20B9),    // ₹
                                                     UCIS_SYM("ruble", 0x20BD),    // ₽
                                                     UCIS_SYM("lira", 0x20BA),     // ₺
                                                     UCIS_SYM("bitcoin", 0x20BF),  // ₿
                                                     UCIS_SYM("ethereum", 0x20B3), // ₳
                                                     UCIS_SYM("litecoin", 0x20B6), // ₶
                                                     UCIS_SYM("dash", 0x20A7),     // ₧
                                                     UCIS_SYM("peso", 0x20B1),     // ₱
                                                     UCIS_SYM("dong", 0x20AB),     // ₫
                                                     UCIS_SYM("hryvnia", 0x20B4),  // ₴
                                                     UCIS_SYM("naira", 0x20A6),    // ₦
                                                     UCIS_SYM("rial", 0x20A0),     // ₠
                                                     UCIS_SYM("baht", 0x20B8),     // ฿
                                                     UCIS_SYM("lari", 0x20BE),     // ₾
                                                     UCIS_SYM("peseta", 0x20A7),   // ₧
                                                     UCIS_SYM("krona", 0x20A1),    // ₡
                                                     UCIS_SYM("franc", 0x20A3),    // ₣
                                                     UCIS_SYM("florin", 0x20A2),   // ₢
                                                     UCIS_SYM("guarani", 0x20B2),  // ₲
                                                     UCIS_SYM("kip", 0x20AD),      // ₭
                                                     UCIS_SYM("manat", 0x20BC),    // ₼
                                                     UCIS_SYM("ngultrum", 0x20B8), // ₸
                                                     UCIS_SYM("pula", 0x20B1),     // ₱
                                                     UCIS_SYM("rufiyaa", 0x20A8),  // ₨
                                                     UCIS_SYM("taka", 0x20A6),     // ₦
                                                     UCIS_SYM("tugrik", 0x20AE),   // ₮
                                                     UCIS_SYM("vatu", 0x20B0),     // ₰
                                                     UCIS_SYM("watt", 0x20A7),     // ₧
                                                     UCIS_SYM("afghani", 0x20AF),  // ₯
                                                     UCIS_SYM("cedi", 0x20B5),     // ₵
                                                     UCIS_SYM("dalasi", 0x20A0),   // ₠
                                                     UCIS_SYM("denar", 0x20AF),    // ₯
                                                     UCIS_SYM("dinar", 0x20AA),    // ₪
                                                     UCIS_SYM("dirham", 0x20AED),  // د.إ
                                                     UCIS_SYM("dobra", 0x20B3),    // ₳
                                                     // Extended Unicode emojis
                                                     UCIS_SYM("hearteyes", 0x1F60D),         // 😍
                                                     UCIS_SYM("kiss", 0x1F618),              // 😘
                                                     UCIS_SYM("tongue", 0x1F61B),            // 😛
                                                     UCIS_SYM("sunglasses", 0x1F60E),        // 😎
                                                     UCIS_SYM("smirk", 0x1F60F),             // 😏
                                                     UCIS_SYM("sweat", 0x1F605),             // 😅
                                                     UCIS_SYM("laugh", 0x1F606),             // 😆
                                                     UCIS_SYM("cry", 0x1F62D),               // 😭
                                                     UCIS_SYM("scream", 0x1F631),            // 😱
                                                     UCIS_SYM("angry", 0x1F620),             // 😠
                                                     UCIS_SYM("confused", 0x1F615),          // 😕
                                                     UCIS_SYM("worried", 0x1F61F),           // 😟
                                                     UCIS_SYM("disappointed", 0x1F61E),      // 😞
                                                     UCIS_SYM("sleepy", 0x1F634),            // 😴
                                                     UCIS_SYM("sick", 0x1F637),              // 😷
                                                     UCIS_SYM("mask", 0x1F637),              // 😷
                                                     UCIS_SYM("cat", 0x1F638),               // 😸
                                                     UCIS_SYM("dog", 0x1F415),               // 🐕
                                                     UCIS_SYM("panda", 0x1F43C),             // 🐼
                                                     UCIS_SYM("bear", 0x1F43B),              // 🐻
                                                     UCIS_SYM("pig", 0x1F437),               // 🐷
                                                     UCIS_SYM("cow", 0x1F42E),               // 🐮
                                                     UCIS_SYM("chicken", 0x1F414),           // 🐔
                                                     UCIS_SYM("frog", 0x1F438),              // 🐸
                                                     UCIS_SYM("bee", 0x1F41D),               // 🐝
                                                     UCIS_SYM("octopus", 0x1F419),           // 🐙
                                                     UCIS_SYM("snail", 0x1F40C),             // 🐌
                                                     UCIS_SYM("bug", 0x1F41B),               // 🐛
                                                     UCIS_SYM("ant", 0x1F41C),               // 🐜
                                                     UCIS_SYM("spider", 0x1F577),            // 🕷
                                                     UCIS_SYM("scorpion", 0x1F982),          // 🦂
                                                     UCIS_SYM("crab", 0x1F980),              // 🦀
                                                     UCIS_SYM("snake", 0x1F40D),             // 🐍
                                                     UCIS_SYM("turtle", 0x1F422),            // 🐢
                                                     UCIS_SYM("lizard", 0x1F98E),            // 🦎
                                                     UCIS_SYM("dolphin", 0x1F42C),           // 🐬
                                                     UCIS_SYM("shark", 0x1F988),             // 🦈
                                                     UCIS_SYM("fish", 0x1F41F),              // 🐟
                                                     UCIS_SYM("whale", 0x1F433),             // 🐳
                                                     UCIS_SYM("crocodile", 0x1F40A),         // 🐊
                                                     UCIS_SYM("dragon", 0x1F409),            // 🐉
                                                     UCIS_SYM("unicorn", 0x1F984),           // 🦄
                                                     UCIS_SYM("horse", 0x1F434),             // 🐴
                                                     UCIS_SYM("camel", 0x1F42A),             // 🐪
                                                     UCIS_SYM("elephant", 0x1F418),          // 🐘
                                                     UCIS_SYM("rhino", 0x1F98F),             // 🦏
                                                     UCIS_SYM("hippo", 0x1F99B),             // 🦛
                                                     UCIS_SYM("lion", 0x1F981),              // 🦁
                                                     UCIS_SYM("tiger", 0x1F42F),             // 🐯
                                                     UCIS_SYM("leopard", 0x1F406),           // 🐆
                                                     UCIS_SYM("parrrot", 0x1F99C),           // 🦜
                                                     UCIS_SYM("penguin", 0x1F427),           // 🐧
                                                     UCIS_SYM("owl", 0x1F989),               // 🦉
                                                     UCIS_SYM("dove", 0x1F54A),              // 🕊
                                                     UCIS_SYM("eagle", 0x1F985),             // 🦅
                                                     UCIS_SYM("duck", 0x1F986),              // 🦆
                                                     UCIS_SYM("swan", 0x1F9A2),              // 🦢
                                                     UCIS_SYM("flamingo", 0x1F9A9),          // 🦩
                                                     UCIS_SYM("peacock", 0x1F99A),           // 🦚
                                                     UCIS_SYM("rooster", 0x1F413),           // 🐓
                                                     UCIS_SYM("turkey", 0x1F983),            // 🦃
                                                     UCIS_SYM("bat", 0x1F987),               // 🦇
                                                     UCIS_SYM("rat", 0x1F400),               // 🐀
                                                     UCIS_SYM("mouse", 0x1F401),             // 🐁
                                                     UCIS_SYM("hamster", 0x1F439),           // 🐹
                                                     UCIS_SYM("rabbit", 0x1F430),            // 🐰
                                                     UCIS_SYM("chipmunk", 0x1F43F),          // 🐿
                                                     UCIS_SYM("hedgehog", 0x1F994),          // 🦔
                                                     UCIS_SYM("paw", 0x1F43E),               // 🐾
                                                     UCIS_SYM("paw_prints", 0x1F43E),        // 🐾
                                                     UCIS_SYM("monkey", 0x1F435),            // 🐵
                                                     UCIS_SYM("gorilla", 0x1F98D),           // 🦍
                                                     UCIS_SYM("orangutan", 0x1F9A7),         // 🦧
                                                     UCIS_SYM("dog2", 0x1F415),              // 🐕
                                                     UCIS_SYM("guide_dog", 0x1F9AE),         // 🦮
                                                     UCIS_SYM("service_dog", 0x1F415),       // 🐕
                                                     UCIS_SYM("poodle", 0x1F429),            // 🐩
                                                     UCIS_SYM("wolf", 0x1F43A),              // 🐺
                                                     UCIS_SYM("fox", 0x1F98A),               // 🦊
                                                     UCIS_SYM("raccoon", 0x1F99D),           // 🦝
                                                     UCIS_SYM("badger", 0x1F9A1),            // 🦡
                                                     UCIS_SYM("bear2", 0x1F43B),             // 🐻
                                                     UCIS_SYM("koala", 0x1F428),             // 🐨
                                                     UCIS_SYM("panda_face", 0x1F43C),        // 🐼
                                                     UCIS_SYM("sloth", 0x1F9A5),             // 🦥
                                                     UCIS_SYM("otter", 0x1F9A6),             // 🦦
                                                     UCIS_SYM("skunk", 0x1F9A8),             // 🦨
                                                     UCIS_SYM("kangaroo", 0x1F998),          // 🦘
                                                     UCIS_SYM("elephant2", 0x1F418),         // 🐘
                                                     UCIS_SYM("flower", 0x1F33A),            // 🌺
                                                     UCIS_SYM("rose", 0x1F339),              // 🌹
                                                     UCIS_SYM("sunflower", 0x1F33B),         // 🌻
                                                     UCIS_SYM("tulip", 0x1F337),             // 🌷
                                                     UCIS_SYM("daisy", 0x1F33E),             // 🌾
                                                     UCIS_SYM("hibiscus", 0x1F33C),          // 🌼
                                                     UCIS_SYM("cherry", 0x1F352),            // 🍒
                                                     UCIS_SYM("strawberry", 0x1F353),        // 🍓
                                                     UCIS_SYM("banana", 0x1F34C),            // 🍌
                                                     UCIS_SYM("apple", 0x1F34E),             // 🍎
                                                     UCIS_SYM("pear", 0x1F350),              // 🍐
                                                     UCIS_SYM("peach", 0x1F351),             // 🍑
                                                     UCIS_SYM("grapes", 0x1F347),            // 🍇
                                                     UCIS_SYM("melon", 0x1F348),             // 🍈
                                                     UCIS_SYM("watermelon", 0x1F349),        // 🍉
                                                     UCIS_SYM("lemon", 0x1F34B),             // 🍋
                                                     UCIS_SYM("pineapple", 0x1F34D),         // 🍍
                                                     UCIS_SYM("kiwi", 0x1F95D),              // 🥝
                                                     UCIS_SYM("avocado", 0x1F951),           // 🥑
                                                     UCIS_SYM("tomato", 0x1F345),            // 🍅
                                                     UCIS_SYM("eggplant", 0x1F346),          // 🍆
                                                     UCIS_SYM("potato", 0x1F954),            // 🥔
                                                     UCIS_SYM("carrot", 0x1F955),            // 🥕
                                                     UCIS_SYM("corn", 0x1F33D),              // 🌽
                                                     UCIS_SYM("cucumber", 0x1F952),          // 🥒
                                                     UCIS_SYM("pepper", 0x1F336),            // 🌶
                                                     UCIS_SYM("mushroom", 0x1F344),          // 🍄
                                                     UCIS_SYM("peanut", 0x1F95C),            // 🥜
                                                     UCIS_SYM("bread", 0x1F35E),             // 🍞
                                                     UCIS_SYM("croissant", 0x1F950),         // 🥐
                                                     UCIS_SYM("baguette", 0x1F956),          // 🥖
                                                     UCIS_SYM("pancakes", 0x1F95E),          // 🥞
                                                     UCIS_SYM("cheese", 0x1F9C0),            // 🧀
                                                     UCIS_SYM("bacon", 0x1F953),             // 🥓
                                                     UCIS_SYM("egg", 0x1F95A),               // 🥚
                                                     UCIS_SYM("fried_egg", 0x1F373),         // 🍳
                                                     UCIS_SYM("pancake", 0x1F95E),           // 🥞
                                                     UCIS_SYM("butter", 0x1F9C8),            // 🧈
                                                     UCIS_SYM("honey", 0x1F36F),             // 🍯
                                                     UCIS_SYM("milk", 0x1F95B),              // 🥛
                                                     UCIS_SYM("coffee", 0x2615),             // ☕
                                                     UCIS_SYM("tea", 0x1F375),               // 🍵
                                                     UCIS_SYM("sake", 0x1F376),              // 🍶
                                                     UCIS_SYM("beer", 0x1F37A),              // 🍺
                                                     UCIS_SYM("wine", 0x1F377),              // 🍷
                                                     UCIS_SYM("cocktail", 0x1F378),          // 🍸
                                                     UCIS_SYM("tropical_drink", 0x1F379),    // 🍹
                                                     UCIS_SYM("champagne", 0x1F37E),         // 🍾
                                                     UCIS_SYM("whiskey", 0x1F943),           // 🥃
                                                     UCIS_SYM("icecream", 0x1F368),          // 🍨
                                                     UCIS_SYM("donut", 0x1F369),             // 🍩
                                                     UCIS_SYM("cookie", 0x1F36A),            // 🍪
                                                     UCIS_SYM("chocolate", 0x1F36B),         // 🍫
                                                     UCIS_SYM("candy", 0x1F36C),             // 🍬
                                                     UCIS_SYM("lollipop", 0x1F36D),          // 🍭
                                                     UCIS_SYM("custard", 0x1F36E),           // 🍮
                                                     UCIS_SYM("popcorn", 0x1F37F),           // 🍿
                                                     UCIS_SYM("airplane", 0x2708),           // ✈️
                                                     UCIS_SYM("helicopter", 0x1F681),        // 🚁
                                                     UCIS_SYM("rocket", 0x1F680),            // 🚀
                                                     UCIS_SYM("satellite", 0x1F6F0),         // 🛰
                                                     UCIS_SYM("car", 0x1F697),               // 🚗
                                                     UCIS_SYM("taxi", 0x1F695),              // 🚕
                                                     UCIS_SYM("bus", 0x1F68C),               // 🚌
                                                     UCIS_SYM("truck", 0x1F69A),             // 🚚
                                                     UCIS_SYM("tractor", 0x1F69C),           // 🚜
                                                     UCIS_SYM("bike", 0x1F6B2),              // 🚲
                                                     UCIS_SYM("motorcycle", 0x1F3CD),        // 🏍
                                                     UCIS_SYM("train", 0x1F682),             // 🚂
                                                     UCIS_SYM("tram", 0x1F68A),              // 🚊
                                                     UCIS_SYM("metro", 0x1F687),             // 🚇
                                                     UCIS_SYM("light_rail", 0x1F688),        // 🚈
                                                     UCIS_SYM("station", 0x1F689),           // 🚉
                                                     UCIS_SYM("tram_stop", 0x1F68F),         // 🚏
                                                     UCIS_SYM("bus_stop", 0x1F68D),          // 🚍
                                                     UCIS_SYM("taxi_stand", 0x1F696),        // 🚖
                                                     UCIS_SYM("car_park", 0x1F697),          // 🚗
                                                     UCIS_SYM("bed", 0x1F6CC),               // 🛌
                                                     UCIS_SYM("couch", 0x1F6CB),             // 🛋
                                                     UCIS_SYM("chair", 0x1FA91),             // 🪑
                                                     UCIS_SYM("toilet", 0x1F6BD),            // 🚽
                                                     UCIS_SYM("shower", 0x1F6BF),            // 🚿
                                                     UCIS_SYM("bathtub", 0x1F6C1),           // 🛁
                                                     UCIS_SYM("potable_water", 0x1F6B0),     // 🚰
                                                     UCIS_SYM("shopping_cart", 0x1F6D2),     // 🛒
                                                     UCIS_SYM("luggage", 0x1F9F3),           // 🧳
                                                     UCIS_SYM("bellhop", 0x1F6CE),           // 🛎
                                                     UCIS_SYM("key", 0x1F511),               // 🔑
                                                     UCIS_SYM("lock", 0x1F512),              // 🔒
                                                     UCIS_SYM("unlock", 0x1F513),            // 🔓
                                                     UCIS_SYM("bell", 0x1F514),              // 🔔
                                                     UCIS_SYM("no_bell", 0x1F515),           // 🔕
                                                     UCIS_SYM("no_entry", 0x26D4),           // ⛔
                                                     UCIS_SYM("no_smoking", 0x1F6AD),        // 🚭
                                                     UCIS_SYM("no_littering", 0x1F6AF),      // 🚯
                                                     UCIS_SYM("no_bikes", 0x1F6B3),          // 🚳
                                                     UCIS_SYM("no_skateboards", 0x1F6A7),    // 🚧
                                                     UCIS_SYM("no_pedestrians", 0x1F6B7),    // 🚷
                                                     UCIS_SYM("no_children", 0x1F6B8),       // 🚸
                                                     UCIS_SYM("no_mobiles", 0x1F4F5),        // 📵
                                                     UCIS_SYM("water", 0x1F4A7),             // 💧
                                                     UCIS_SYM("fire_extinguisher", 0x1F9EF), // 🧯
                                                     UCIS_SYM("fire_alarm", 0x1F6A8),        // 🚨
                                                     UCIS_SYM("first_aid", 0x1F3E5),         // 🏥
                                                     UCIS_SYM("crossed_fingers", 0x1F91E),   // 🤞
                                                     UCIS_SYM("handshake", 0x1F91D),         // 🤝
                                                     UCIS_SYM("sun", 0x2600),                // ☀️
                                                     UCIS_SYM("moon", 0x1F319),              // 🌙
                                                     UCIS_SYM("star", 0x2B50),               // ⭐
                                                     UCIS_SYM("cloud", 0x2601),              // ☁️
                                                     UCIS_SYM("rain", 0x1F327),              // 🌧
                                                     UCIS_SYM("snow", 0x1F328),              // 🌨
                                                     UCIS_SYM("thunder", 0x26A1),            // ⚡
                                                     UCIS_SYM("tornado", 0x1F32A),           // 🌪
                                                     UCIS_SYM("fog", 0x1F32B),               // 🌫
                                                     UCIS_SYM("wind", 0x1F32C),              // 🌬
                                                     UCIS_SYM("umbrella", 0x2602),           // ☂️
                                                     UCIS_SYM("thermometer", 0x1F321),       // 🌡
                                                     UCIS_SYM("droplet", 0x1F4A7),           // 💧
                                                     UCIS_SYM("wave", 0x1F30A),              // 🌊
                                                     UCIS_SYM("volcano", 0x1F30B),           // 🌋
                                                     UCIS_SYM("earthquake", 0x1F30C),        // 🌌
                                                     UCIS_SYM("hurricane", 0x1F300),         // 🌀
                                                     UCIS_SYM("compass", 0x1F9ED),           // 🧭
                                                     UCIS_SYM("map", 0x1F5FA),               // 🗺
                                                     UCIS_SYM("mountain", 0x26F0),           // ⛰
                                                     UCIS_SYM("camping", 0x1F3D5),           // 🏕
                                                     UCIS_SYM("beach", 0x1F3D6),             // 🏖
                                                     UCIS_SYM("desert", 0x1F3DC),            // 🏜
                                                     UCIS_SYM("island", 0x1F3DD),            // 🏝
                                                     UCIS_SYM("park", 0x1F3DE),              // 🏞
                                                     UCIS_SYM("stadium", 0x1F3DF),           // 🏟
                                                     UCIS_SYM("city", 0x1F3D9),              // 🏙
                                                     UCIS_SYM("house", 0x1F3E0),             // 🏠
                                                     UCIS_SYM("apartment", 0x1F3E1),         // 🏡
                                                     UCIS_SYM("office", 0x1F3E2),            // 🏢
                                                     UCIS_SYM("post_office", 0x1F3E3),       // 🏣
                                                     UCIS_SYM("party", 0x1F389),             // 🎉
                                                     UCIS_SYM("birthday", 0x1F382),          // 🎂
                                                     UCIS_SYM("christmas", 0x1F384),         // 🎄
                                                     UCIS_SYM("fireworks", 0x1F386),         // 🎆
                                                     UCIS_SYM("balloon", 0x1F388),           // 🎈
                                                     UCIS_SYM("gift", 0x1F381),              // 🎁
                                                     UCIS_SYM("ticket", 0x1F3AB),            // 🎫
                                                     UCIS_SYM("trophy", 0x1F3C6),            // 🏆
                                                     UCIS_SYM("medal", 0x1F396),             // 🎖
                                                     UCIS_SYM("rosette", 0x1F3F5),           // 🏵
                                                     UCIS_SYM("ribbon", 0x1F397),            // 🎗
                                                     UCIS_SYM("badge", 0x1F4DB),             // 📛
                                                     UCIS_SYM("flag", 0x1F3F4),              // 🏴
                                                     UCIS_SYM("banner", 0x1F3F7),            // 🏷
                                                     UCIS_SYM("confetti", 0x1F38A),          // 🎊
                                                     UCIS_SYM("pinata", 0x1FA85),            // 🪅
                                                     UCIS_SYM("pin", 0x1F4CC),               // 📌
                                                     UCIS_SYM("paperclip", 0x1F4CE),         // 📎
                                                     UCIS_SYM("scissors", 0x2702),           // ✂️
                                                     UCIS_SYM("pen", 0x1F58A),               // 🖊
                                                     UCIS_SYM("pencil", 0x270F),             // ✏️
                                                     UCIS_SYM("paintbrush", 0x1F58C),        // 🖌
                                                     UCIS_SYM("crayon", 0x1F58D),            // 🖍
                                                     UCIS_SYM("ruler", 0x1F4CF),             // 📏
                                                     UCIS_SYM("notebook", 0x1F4D3),          // 📓
                                                     UCIS_SYM("ledger", 0x1F4D2),            // 📒
                                                     UCIS_SYM("calendar", 0x1F4C5),          // 📅
                                                     UCIS_SYM("clipboard", 0x1F4CB),         // 📋
                                                     UCIS_SYM("file", 0x1F4C1),              // 📁
                                                     UCIS_SYM("folder", 0x1F4C2),            // 📂
                                                     UCIS_SYM("open_file", 0x1F4C4),         // 📄
                                                     UCIS_SYM("trash", 0x1F5D1),             // 🗑
                                                     UCIS_SYM("lock_with_pen", 0x1F50F),     // 🔏
                                                     UCIS_SYM("lock_with_key", 0x1F510),     // 🔐
                                                     UCIS_SYM("lightbulb", 0x1F4A1),         // 💡
                                                     UCIS_SYM("flashlight", 0x1F526),        // 🔦
                                                     UCIS_SYM("candle", 0x1F56F),            // 🕯
                                                     UCIS_SYM("crown", 0x1F451),             // 👑
                                                     UCIS_SYM("ring", 0x1F48D),              // 💍
                                                     UCIS_SYM("gem", 0x1F48E),               // 💎
                                                     UCIS_SYM("money", 0x1F4B0),             // 💰
                                                     UCIS_SYM("credit_card", 0x1F4B3),       // 💳
                                                     UCIS_SYM("receipt", 0x1F9FE),           // 🧾
                                                     UCIS_SYM("chart", 0x1F4C8),             // 📈
                                                     UCIS_SYM("graph", 0x1F4C9),             // 📉
                                                     UCIS_SYM("bar_chart", 0x1F4CA),         // 📊
                                                     UCIS_SYM("pie", 0x1F967),               // 🥧
                                                     UCIS_SYM("leaf", 0x1F343),              // 🍃
                                                     UCIS_SYM("tree", 0x1F333),              // 🌳
                                                     UCIS_SYM("palm_tree", 0x1F334),         // 🌴
                                                     UCIS_SYM("cactus", 0x1F335),            // 🌵
                                                     UCIS_SYM("skull", 0x1F480),             // 💀
                                                     UCIS_SYM("skull_crossbones", 0x2620),   // ☠️
                                                     UCIS_SYM("ghost", 0x1F47B),             // 👻
                                                     UCIS_SYM("alien", 0x1F47D),             // 👽
                                                     UCIS_SYM("robot", 0x1F916),             // 🤖
                                                     UCIS_SYM("zombie", 0x1F9DF),            // 🧟
                                                     UCIS_SYM("santa", 0x1F385),             // 🎅
                                                     UCIS_SYM("elf", 0x1F9DD),               // 🧝
                                                     UCIS_SYM("vampire", 0x1F9DB),           // 🧛
                                                     UCIS_SYM("wizard", 0x1F9D9),            // 🧙
                                                     UCIS_SYM("mermaid", 0x1F9DC),           // 🧜
                                                     UCIS_SYM("fairy", 0x1F9DA),             // 🧚
                                                     UCIS_SYM("angel", 0x1F47C),             // 👼
                                                     UCIS_SYM("devil", 0x1F47F),             // 👿
                                                     UCIS_SYM("clown", 0x1F921),             // 🤡
                                                     UCIS_SYM("jester", 0x1F9B9),            // 🦹
                                                     UCIS_SYM("mage", 0x1F9D9),              // 🧙
                                                     UCIS_SYM("ninja", 0x1F977),             // 🥷
                                                     UCIS_SYM("princess", 0x1F478),          // 👸
                                                     UCIS_SYM("prince", 0x1F934),            // 🤴
                                                     UCIS_SYM("bride", 0x1F470),             // 👰
                                                     UCIS_SYM("groom", 0x1F935),             // 🤵
                                                     UCIS_SYM("baby", 0x1F476),              // 👶
                                                     UCIS_SYM("child", 0x1F9D2),             // 🧒
                                                     UCIS_SYM("left_arrow", 0x2B05),         // ⬅️
                                                     UCIS_SYM("right_arrow", 0x27A1),        // ➡️
                                                     UCIS_SYM("up_arrow", 0x2B06),           // ⬆️
                                                     UCIS_SYM("down_arrow", 0x2B07),         // ⬇️
                                                     UCIS_SYM("up_left_arrow", 0x2196),      // ↖️
                                                     UCIS_SYM("up_right_arrow", 0x2197),     // ↗️
                                                     UCIS_SYM("down_right_arrow", 0x2198),   // ↘️
                                                     UCIS_SYM("down_left_arrow", 0x2199)     // ↙️
);
#endif

/* ##################################################################### */
/* ########################### Define Combos ########################### */
/* ##################################################################### */
// Combos are performed by pressing multiple keys simultaneously.
// This is different than tap dance keys, which are performed by tapping keys in sequence.
const uint16_t PROGMEM combo_undo[]             = {KC_Z, KC_X, COMBO_END};                        // Undo: Z + X
const uint16_t PROGMEM combo_cut[]              = {KC_Z, KC_C, COMBO_END};                        // Cut: Z + C
const uint16_t PROGMEM combo_copy[]             = {KC_X, KC_C, COMBO_END};                        // Copy: X + C
const uint16_t PROGMEM combo_paste[]            = {KC_C, KC_V, COMBO_END};                        // Paste: C + V
const uint16_t PROGMEM combo_select_all[]       = {KC_Z, KC_V, COMBO_END};                        // Select all: Z + A
const uint16_t PROGMEM combo_shift_lock[]       = {KC_LSFT, KC_RSFT, COMBO_END};                  // Shift lock: Left Shift + Right Shift
const uint16_t PROGMEM combo_select_word[]      = {KC_LEFT_BRACKET, KC_RIGHT_BRACKET, COMBO_END}; // Select word: [ + ]
const uint16_t PROGMEM combo_select_line[]      = {KC_SEMICOLON, KC_QUOTE, COMBO_END};            // Select the current line: ; + '
const uint16_t PROGMEM combo_join_lines[]       = {KC_COMMA, KC_DOT, COMBO_END};                  // Join lines like Vim's `J` command: , + .
const uint16_t PROGMEM combo_autocorrect[]      = {KC_A, KC_C, COMBO_END};                        // Autocorrect toggle: A + C
const uint16_t PROGMEM combo_search_selection[] = {KC_F, KC_S, COMBO_END};                        // Search selection: F + S
const uint16_t PROGMEM combo_up_dir[]           = {KC_DOT, KC_SLASH, COMBO_END};                  // Go up a directory: . + /

// ##################################################################### //
// ######################### Define Combo Array ######################## //
// ##################################################################### //
// The combo array allows the firmware to keep track of the state of each combo.
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
