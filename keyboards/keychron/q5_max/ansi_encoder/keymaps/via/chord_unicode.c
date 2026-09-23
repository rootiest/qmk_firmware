// Copyright 2024 rootiest
// SPDX-License-Identifier: GPL-2.0-or-later

#include "chord_unicode.h"
#include <string.h>

// ============================================================================
// Chord table
// ============================================================================
// Rules:
//   • Sequences are lowercase ASCII letters and digits only.
//   • Two entries whose sequences share a prefix are fine as long as they
//     produce the SAME output (aliases).  If they produce DIFFERENT output,
//     they must differ before the shorter one ends — otherwise the
//     disambiguation timer handles it (CHORD_DISAMBIG_MS).
//   • Max sequence length is CHORD_MAX_LEN (8).
//
// NOTE: ARM targets (this keyboard) store const data in flash regardless of
// PROGMEM; no pgm_read_* helpers are needed here.
// ============================================================================

typedef struct {
    const char *seq; // ASCII sequence to type after CHORD_KEY
    const char *out; // UTF-8 string to output
} chord_entry_t;

// clang-format off
static const chord_entry_t chord_table[] = {

    // ---- Math / Science ----------------------------------------------------
    {"deg",   "°"},  // degree sign          U+00B0
    {"cels",  "℃"},  // degree Celsius       U+2103  (not "degc": would delay "deg")
    {"fahr",  "℉"},  // degree Fahrenheit    U+2109
    {"sqrt",  "√"},  // square root          U+221A
    {"sqrd",  "²"},  // superscript 2        U+00B2
    {"cubd",  "³"},  // superscript 3        U+00B3
    {"aprx",  "≈"},  // almost equal         U+2248
    {"apx",   "≈"},  //   alias
    {"neq",   "≠"},  // not equal            U+2260
    {"lteq",  "≤"},  // less-or-equal        U+2264
    {"lte",   "≤"},  //   alias
    {"gteq",  "≥"},  // greater-or-equal     U+2265
    {"gte",   "≥"},  //   alias
    {"pm",    "±"},  // plus-minus           U+00B1
    {"div",   "÷"},  // division sign        U+00F7
    {"times", "×"},  // multiplication sign  U+00D7
    {"mult",  "×"},  //   alias
    {"infty", "∞"},  // infinity             U+221E
    {"inf",   "∞"},  //   alias (prefix of "infty" → same output, OK)
    {"pi",    "π"},  // pi                   U+03C0
    {"micro", "µ"},  // micro sign           U+00B5
    {"sigma", "Σ"},  // capital sigma        U+03A3
    {"delta", "Δ"},  // capital delta        U+0394
    {"theta", "θ"},  // small theta          U+03B8
    {"omega", "Ω"},  // capital omega        U+03A9
    {"alpha",   "α"},  // small alpha    U+03B1
    {"beta",    "β"},  // small beta     U+03B2
    {"gamma",   "γ"},  // small gamma    U+03B3
    {"epsilon", "ε"},  // small epsilon  U+03B5
    {"eps",     "ε"},  //   alias
    {"zeta",    "ζ"},  // small zeta     U+03B6
    {"eta",     "η"},  // small eta      U+03B7
    {"iota",    "ι"},  // small iota     U+03B9
    {"kappa",   "κ"},  // small kappa    U+03BA
    {"lambda",  "λ"},  // small lambda   U+03BB
    {"mu",      "μ"},  // small mu       U+03BC  (disambig: "mute")
    {"nu",      "ν"},  // small nu       U+03BD
    {"xi",      "ξ"},  // small xi       U+03BE
    {"omicron", "ο"},  // small omicron  U+03BF
    {"omi",     "ο"},  //   alias
    {"rho",     "ρ"},  // small rho      U+03C1
    {"tau",     "τ"},  // small tau      U+03C4
    {"upsilon", "υ"},  // small upsilon  U+03C5
    {"ups",     "υ"},  //   alias
    {"phi",     "φ"},  // small phi      U+03C6
    {"chi",     "χ"},  // small chi      U+03C7
    {"psi",     "ψ"},  // small psi      U+03C8

    // ---- Number sets (blackboard bold) ------------------------------------
    {"real",  "ℝ"},  // real numbers      U+211D
    {"nat",   "ℕ"},  // natural numbers   U+2115
    {"intgr", "ℤ"},  // integers          U+2124
    {"rat",   "ℚ"},  // rational numbers  U+211A
    {"cmplx", "ℂ"},  // complex numbers   U+2102
    {"aleph", "ℵ"},  // aleph             U+2135

    // ---- Calculus / Analysis -----------------------------------------------
    {"integ",   "∫"},  // integral           U+222B
    {"iint",    "∬"},  // double integral    U+222C
    {"oint",    "∮"},  // contour integral   U+222E
    {"partial", "∂"},  // partial derivative U+2202
    {"prt",     "∂"},  //   alias
    {"nabla",   "∇"},  // nabla / del        U+2207
    {"sum",     "∑"},  // summation          U+2211
    {"prod",    "∏"},  // product            U+220F

    // ---- Set theory --------------------------------------------------------
    {"elem",   "∈"},  // element of         U+2208
    {"notin",  "∉"},  // not element of     U+2209
    {"sub",    "⊂"},  // subset of          U+2282  (disambig: "subeq")
    {"subeq",  "⊆"},  // subset or equal    U+2286
    {"sup",    "⊃"},  // superset of        U+2283  (disambig: "supeq")
    {"supeq",  "⊇"},  // superset or equal  U+2287
    {"union",  "∪"},  // union              U+222A
    {"inter",  "∩"},  // intersection       U+2229
    {"empty",  "∅"},  // empty set          U+2205

    // ---- Logic -------------------------------------------------------------
    {"forall", "∀"},  // for all            U+2200
    {"exists", "∃"},  // there exists       U+2203
    {"nexist", "∄"},  // does not exist     U+2204
    {"land",   "∧"},  // logical and        U+2227
    {"lor",    "∨"},  // logical or         U+2228
    {"xor",    "⊕"},  // exclusive or       U+2295
    {"impl",   "⟹"},  // implies            U+27F9
    {"iff",    "⟺"},  // if and only if     U+27FA

    // ---- Geometry / Linear algebra -----------------------------------------
    {"angle",  "∠"},  // angle              U+2220
    {"ang",    "∠"},  //   alias
    {"perp",   "⊥"},  // perpendicular      U+22A5
    {"parl",   "∥"},  // parallel           U+2225

    // ---- Floor / ceiling ---------------------------------------------------
    {"lfl",    "⌊"},  // left floor         U+230A
    {"rfl",    "⌋"},  // right floor        U+230B
    {"lcl",    "⌈"},  // left ceiling       U+2308
    {"rcl",    "⌉"},  // right ceiling      U+2309

    // ---- Other math --------------------------------------------------------
    {"equiv",  "≡"},  // congruent / equiv  U+2261
    {"prop",   "∝"},  // proportional to    U+221D

    // ---- Currency ----------------------------------------------------------
    {"euro",  "€"},  // euro                 U+20AC
    {"pound", "£"},  // pound sterling       U+00A3
    {"yen",   "¥"},  // yen / yuan           U+00A5
    {"cent",  "¢"},  // cent sign            U+00A2

    // ---- Fractions ---------------------------------------------------------
    {"half",  "½"},  //  1/2  U+00BD
    {"qtr",   "¼"},  //  1/4  U+00BC
    {"3qtr",  "¾"},  //  3/4  U+00BE

    // ---- Legal / IP --------------------------------------------------------
    {"copy",  "©"},  // copyright  U+00A9
    {"reg",   "®"},  // registered U+00AE
    {"tm",    "™"},  // trademark  U+2122

    // ---- Arrows ------------------------------------------------------------
    // Use "arr{l,r,u,d}" for the four cardinal arrows and
    // "arr{h,v}" for the bidirectional pair.  This avoids "arrl" being a
    // prefix of a different-output entry.
    {"arrl",  "←"},  // left   U+2190
    {"arrr",  "→"},  // right  U+2192
    {"arru",  "↑"},  // up     U+2191
    {"arrd",  "↓"},  // down   U+2193
    {"arrh",  "↔"},  // left↔right  U+2194
    {"arrv",  "↕"},  // up↕down     U+2195
    {"dbl",   "⇒"},  // double right arrow  U+21D2

    // ---- Typography --------------------------------------------------------
    {"bull",  "•"},  // bullet        U+2022
    {"mdash", "—"},  // em dash       U+2014
    {"ndash", "–"},  // en dash       U+2013
    {"ellip", "…"},  // horizontal ellipsis  U+2026

    // ---- Emoji : Faces -----------------------------------------------------
    {"smile", "🙂"},  // slightly smiling face  U+1F642
    {"grin",  "😁"},  // beaming face           U+1F601
    {"lol",   "😂"},  // face with tears of joy U+1F602
    {"cry",   "😭"},  // loudly crying face     U+1F62D
    {"sad",   "🙁"},  // slightly frowning face U+1F641
    {"wink",  "😉"},  // winking face           U+1F609
    {"cool",  "😎"},  // smiling face w/ sunglasses U+1F60E
    {"think", "🤔"},  // thinking face          U+1F914
    {"shrug", "🤷"},  // person shrugging       U+1F937
    {"ugh",   "😤"},  // face with steam        U+1F624
    {"wow",   "😮"},  // face with open mouth   U+1F62E
    {"zip",   "🤐"},  // zipper-mouth face      U+1F910
    {"nerdy", "🤓"},  // nerd face              U+1F913
    {"love",   "😍"},  // heart eyes             U+1F60D
    {"kiss",   "😘"},  // face blowing a kiss    U+1F618
    {"blush",  "😊"},  // smiling, smiling eyes  U+1F60A
    {"smirk",  "😏"},  // smirking face          U+1F60F
    {"eyeroll","🙄"},  // face with rolling eyes U+1F644
    {"sweat",  "😅"},  // grinning face w/ sweat U+1F605
    {"rofl",   "🤣"},  // rolling on the floor   U+1F923
    {"flip",   "🙃"},  // upside-down face       U+1F643  (not "upside": would delay "ups")
    {"plead",  "🥺"},  // pleading face          U+1F97A
    {"melt",   "🫠"},  // melting face           U+1FAE0
    {"salute", "🫡"},  // saluting face          U+1FAE1
    {"hug",    "🤗"},  // hugging face           U+1F917
    {"shush",  "🤫"},  // shushing face          U+1F92B
    {"mind",   "🤯"},  // exploding head         U+1F92F
    {"mad",    "😠"},  // angry face             U+1F620  (not "angry": would delay "ang")
    {"hot",    "🥵"},  // hot face               U+1F975
    {"cold",   "🥶"},  // cold face              U+1F976
    {"puke",   "🤮"},  // face vomiting          U+1F92E
    {"party",  "🥳"},  // partying face          U+1F973
    {"cowboy", "🤠"},  // cowboy hat face        U+1F920
    {"halo",   "😇"},  // smiling face with halo U+1F607  (not "angel": would delay "ang")
    {"devil",  "😈"},  // smiling face w/ horns  U+1F608
    {"horny",  "😈"},  //   slang alias
    {"clown",  "🤡"},  // clown face             U+1F921
    {"ghost",  "👻"},  // ghost                  U+1F47B

    // ---- Emoji : Gestures --------------------------------------------------
    {"thup",  "👍"},  // thumbs up    U+1F44D
    {"thdn",  "👎"},  // thumbs down  U+1F44E
    {"wave",  "👋"},  // waving hand  U+1F44B
    {"clap",  "👏"},  // clapping     U+1F44F
    {"fist",  "✊"},  // raised fist  U+270A
    {"pray",  "🙏"},  // folded hands U+1F64F
    {"ok",    "👌"},  // ok hand      U+1F44C
    {"point", "👉"},  // backhand index pointing right U+1F449
    {"facepalm","🤦"}, // person facepalming  U+1F926
    {"flex",   "💪"},  // flexed biceps       U+1F4AA  (not "muscle": would delay "mus")
    {"praise", "🙌"},  // raising hands       U+1F64C
    {"shake",  "🤝"},  // handshake           U+1F91D
    {"luck",   "🤞"},  // crossed fingers     U+1F91E
    {"nail",   "💅"},  // nail polish         U+1F485

    // ---- Emoji : Symbols ---------------------------------------------------
    // Variation selectors (U+FE0F) are omitted for cross-app compatibility;
    // modern renderers apply emoji presentation automatically.
    {"heart", "❤"},   // heavy black heart  U+2764
    {"check", "✓"},   // check mark         U+2713
    {"cross", "✗"},   // ballot X           U+2717
    {"warn",  "⚠"},   // warning sign       U+26A0
    {"stop",  "🛑"},   // stop sign          U+1F6D1
    {"yes",   "✅"},   // white heavy check mark  U+2705
    {"nope",  "❌"},   // cross mark              U+274C
    {"ques",  "❓"},   // question mark           U+2753
    {"excl",  "❗"},   // exclamation mark        U+2757
    {"help",  "ℹ"},   // information source  U+2139
    {"fire",  "🔥"},   // fire       U+1F525
    {"star",  "⭐"},   // star       U+2B50
    {"tada",  "🎉"},   // party popper  U+1F389
    {"100",   "💯"},   // hundred points U+1F4AF
    {"zzz",   "💤"},   // zzz            U+1F4A4
    {"skull", "💀"},   // skull          U+1F480
    {"poop",  "💩"},   // pile of poo    U+1F4A9
    {"eyes",  "👀"},   // eyes           U+1F440
    {"bell",  "🔔"},   // bell           U+1F514
    {"mute",  "🔇"},   // muted speaker  U+1F507
    {"loud",  "🔊"},   // loud speaker   U+1F50A
    {"bulb",  "💡"},   // light bulb     U+1F4A1
    {"tack",  "📌"},   // pushpin / thumbtack  U+1F4CC
    {"key",   "🔑"},   // key     U+1F511
    {"lock",  "🔒"},   // lock    U+1F512
    {"robot", "🤖"},   // robot   U+1F916
    {"alien", "👽"},   // alien   U+1F47D
    {"broken", "💔"},  // broken heart   U+1F494
    {"sparkle","✨"},  // sparkles       U+2728
    {"drops",  "💦"},  // sweat droplets U+1F4A6
    {"cum",    "💦"},  //   slang alias
    {"brain",  "🧠"},  // brain          U+1F9E0
    {"trophy", "🏆"},  // trophy         U+1F3C6
    {"moai",   "🗿"},  // moai           U+1F5FF

    // ---- Emoji : Nature ----------------------------------------------------
    {"sun",   "☀"},   // black sun  U+2600
    {"moon",  "🌙"},   // crescent moon  U+1F319
    {"snow",  "❄"},   // snowflake   U+2744
    {"rain",  "🌧"},   // cloud with rain  U+1F327
    {"bolt",  "⚡"},   // high voltage / lightning  U+26A1
    {"cat",   "🐱"},   // cat face   U+1F431
    {"dog",   "🐶"},   // dog face   U+1F436
    {"fox",   "🦊"},   // fox face   U+1F98A
    {"bear",  "🐻"},   // bear face  U+1F43B
    {"goat",   "🐐"},  // goat      U+1F410
    {"unicorn","🦄"},  // unicorn   U+1F984
    {"rnbw",   "🌈"},  // rainbow   U+1F308  (not "rainbow": would delay "rain")

    // ---- Emoji : Food & Objects --------------------------------------------
    {"coffee","☕"},   // hot beverage  U+2615
    {"beer",  "🍺"},   // beer mug      U+1F37A
    {"pizza", "🍕"},   // pizza         U+1F355
    {"cake",  "🎂"},   // birthday cake U+1F382
    {"gift",  "🎁"},   // wrapped gift  U+1F381
    {"mike",  "🎤"},   // microphone    U+1F3A4
    {"mus",   "🎵"},   // musical note  U+1F3B5
    {"phone", "📱"},   // mobile phone  U+1F4F1
    {"pc",    "💻"},   // laptop        U+1F4BB
    {"book",  "📖"},   // open book     U+1F4D6
    {"mail",  "📧"},   // e-mail        U+1F4E7
    {"money", "💰"},   // money bag     U+1F4B0
    {"gem",   "💎"},   // gem stone     U+1F48E
    {"sword", "⚔"},   // crossed swords  U+2694
    {"shield","🛡"},   // shield          U+1F6E1
    {"rocket","🚀"},   // rocket          U+1F680
    {"tools", "🔧"},   // wrench / tools  U+1F527
    {"trash", "🗑"},   // wastebasket     U+1F5D1
    {"clock", "🕐"},   // one o'clock     U+1F550
    {"hour",  "⏳"},   // hourglass with flowing sand  U+23F3
    {"eggplant","🍆"}, // eggplant  U+1F346
    {"cock",   "🍆"},  //   slang alias
    {"dick",   "🍆"},  //   slang alias
    {"peach",  "🍑"},  // peach     U+1F351
    {"butt",   "🍑"},  //   slang alias
    {"ass",    "🍑"},  //   slang alias
    {"melon",  "🍈"},  // melon     U+1F348
    {"boobs",  "🍈"},  //   slang alias
    {"taco",   "🌮"},  // taco      U+1F32E
    {"pussy",  "🌮"},  //   slang alias
    {"vag",    "🌮"},  //   slang alias
    {"wine",   "🍷"},  // wine glass  U+1F377
    {"chair",  "🪑"},  // chair     U+1FA91
    {"lmao",   "🪑"},  //   slang alias (TikTok "I'm dead" laugh)
};
// clang-format on

#define CHORD_TABLE_LEN (sizeof(chord_table) / sizeof(chord_table[0]))

// ============================================================================
// State machine
// ============================================================================

typedef enum {
    CS_IDLE,
    CS_ACTIVE,
} chord_state_t;

static chord_state_t chord_state   = CS_IDLE;
static bool          chord_held    = false; // true while CHORD_KEY is still held
static char          chord_buf[CHORD_MAX_LEN + 1];
static uint8_t       chord_buf_len = 0;
static uint16_t      chord_timer   = 0; // last activity timestamp

// Disambiguation: pending exact match that is also a prefix of a longer entry
// whose output differs.
static bool     chord_disambig   = false;
static uint16_t chord_disambig_t = 0;
static uint16_t chord_disambig_i = 0;

// ============================================================================
// Internal helpers
// ============================================================================

static void chord_reset(void) {
    chord_state      = CS_IDLE;
    chord_held       = false;
    chord_buf_len    = 0;
    chord_buf[0]     = '\0';
    chord_disambig   = false;
}

static void chord_output(uint16_t idx) {
    send_unicode_string(chord_table[idx].out);
    chord_reset();
}

/*
 * Scan the chord table against the current buffer.
 *
 * Returns:
 *   -2  — no match at all (not even a prefix) → cancel chord mode
 *   -1  — buffer is a prefix of ≥1 entry but no exact match yet → keep going
 *  ≥ 0  — index of the first exact match; *is_prefix is set if the buffer is
 *          also a prefix of a DIFFERENT-output longer entry
 */
static int chord_check(bool *is_prefix) {
    *is_prefix      = false;
    int  exact_idx  = -1;
    bool any_match  = false;

    for (uint16_t i = 0; i < CHORD_TABLE_LEN; i++) {
        uint8_t slen = (uint8_t)strlen(chord_table[i].seq);

        // Entry shorter than current buffer → can't match
        if (slen < chord_buf_len) continue;

        // Must be a prefix match at minimum
        if (memcmp(chord_table[i].seq, chord_buf, chord_buf_len) != 0) continue;

        any_match = true;

        if (slen == chord_buf_len) {
            // Exact match — take the first one found
            if (exact_idx < 0) exact_idx = (int)i;
        } else {
            // Buffer is a strict prefix of this longer entry
            // Flag as a prefix conflict only when output differs from the
            // already-found exact match (same-output aliases are harmless).
            if (exact_idx >= 0) {
                if (strcmp(chord_table[i].out, chord_table[exact_idx].out) != 0) {
                    *is_prefix = true;
                }
            } else {
                // No exact match found yet; mark the prefix tentatively.
                // We'll re-evaluate the flag after finding an exact match, but
                // we need any_match = true to avoid returning -2.
                *is_prefix = true; // tentative; may be cleared below
            }
        }
    }

    if (!any_match) return -2;
    if (exact_idx < 0) return -1; // prefix(es) only

    // Re-verify is_prefix: we may have set it before seeing the exact match.
    // Walk again only for the is_prefix refinement.
    if (*is_prefix) {
        *is_prefix = false;
        for (uint16_t i = 0; i < CHORD_TABLE_LEN; i++) {
            uint8_t slen = (uint8_t)strlen(chord_table[i].seq);
            if (slen <= chord_buf_len) continue;
            if (memcmp(chord_table[i].seq, chord_buf, chord_buf_len) != 0) continue;
            // Longer entry that shares our prefix
            if (strcmp(chord_table[i].out, chord_table[exact_idx].out) != 0) {
                *is_prefix = true;
                break;
            }
        }
    }

    return exact_idx;
}

// ============================================================================
// Public API
// ============================================================================

void chord_init(void) {
    chord_reset();
}

void chord_activate(void) {
    chord_reset();
    chord_state = CS_ACTIVE;
    chord_held  = true;
    chord_timer = timer_read();
}

void chord_key_released(void) {
    if (chord_state != CS_ACTIVE) return;

    if (chord_buf_len == 0) {
        // Chord key tapped with nothing typed yet → switch to tap mode.
        // Keep collecting; idle timeout will cancel if nothing arrives.
        chord_held  = false;
        chord_timer = timer_read();
        return;
    }

    // Hold mode: key released with a partial or complete sequence.
    if (chord_disambig) {
        chord_output(chord_disambig_i);
        return;
    }

    bool is_prefix;
    int idx = chord_check(&is_prefix);
    if (idx >= 0) {
        chord_output(idx);
    } else {
        chord_reset();
    }
}

bool process_chord(uint16_t keycode, keyrecord_t *record) {
    if (chord_state == CS_IDLE) return true;

    // Pass through key-up events while chord mode is active.
    // Basic key-ups (a-z, 0-9) are no-ops on the host since we never sent
    // the corresponding key-down.  Layer/quantum key-ups (TT, MO, LT, etc.)
    // must reach QMK's layer system so layers deactivate correctly.
    if (!record->event.pressed) return true;

    // Any keypress resets the idle timer.
    chord_timer = timer_read();

    // --- Special keys -------------------------------------------------------

    if (keycode == KC_ESC) {
        chord_reset();
        return false;
    }

    if (keycode == KC_BSPC) {
        if (chord_disambig) {
            chord_disambig = false;
        } else if (chord_buf_len > 0) {
            chord_buf[--chord_buf_len] = '\0';
        }
        return false;
    }

    if (keycode == KC_ENT) {
        // Enter explicitly confirms the current buffer.
        if (chord_disambig) {
            chord_output(chord_disambig_i);
        } else {
            bool is_prefix;
            int  idx = chord_check(&is_prefix);
            if (idx >= 0) {
                chord_output(idx);
            } else {
                chord_reset();
            }
        }
        return false;
    }

    // Modifier keys (Ctrl, Shift, Alt, GUI) pass through without disturbing
    // chord mode — the user may have them held for unrelated reasons.
    if (keycode >= KC_LCTL && keycode <= KC_RGUI) {
        return true;
    }

    // --- Build the sequence buffer ------------------------------------------

    char c = 0;
    if (keycode >= KC_A && keycode <= KC_Z) {
        c = 'a' + (keycode - KC_A);
    } else if (keycode >= KC_1 && keycode <= KC_9) {
        c = '1' + (keycode - KC_1);
    } else if (keycode == KC_0) {
        c = '0';
    } else {
        // Unrecognised key: cancel chord mode silently.
        chord_reset();
        return false;
    }

    if (chord_buf_len >= CHORD_MAX_LEN) {
        chord_reset();
        return false;
    }

    chord_buf[chord_buf_len++] = c;
    chord_buf[chord_buf_len]   = '\0';
    chord_disambig             = false;

    // --- Check the table ----------------------------------------------------

    bool is_prefix;
    int  idx = chord_check(&is_prefix);

    if (idx == -2) {
        // Dead end — no entry can ever match this buffer.
        chord_reset();
    } else if (idx >= 0 && !is_prefix) {
        // Clean exact match: output immediately.
        chord_output(idx);
    } else if (idx >= 0 && is_prefix) {
        // Exact match, but a longer different-output entry shares the prefix.
        // Start the disambiguation timer; output fires when it expires.
        chord_disambig   = true;
        chord_disambig_t = timer_read();
        chord_disambig_i = (uint16_t)idx;
    }
    // idx == -1: only prefix match(es) so far → keep collecting.

    return false;
}

void chord_scan(void) {
    if (chord_state == CS_IDLE) return;

    // Disambiguation timeout: no longer input arrived → commit exact match.
    if (chord_disambig && timer_elapsed(chord_disambig_t) > CHORD_DISAMBIG_MS) {
        chord_output(chord_disambig_i);
        return;
    }

    // Idle timeout (tap mode only): cancel if nothing typed for too long.
    if (!chord_held && timer_elapsed(chord_timer) > CHORD_TIMEOUT_MS) {
        chord_reset();
    }
}
