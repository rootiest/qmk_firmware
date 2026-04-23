// Copyright 2024 rootiest
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Tight, strict combo window for a simultaneous 3-key press.
// This ensures the fallback combo only fires when intended.
#define COMBO_TERM 10
#define COMBO_STRICT_TIMER

// Use a more conservative debounce period (20ms is standard for Keychron)
// but since we switched to 'sym_defer_pk' in rules.mk, this will now
// require 20ms of STABILITY before a keypress is reported.
#define DEBOUNCE 20

// Always resolve combo keycodes from BASE (layer 0) so the
// COMM+DOT+SLSH fallback combo fires regardless of the active layer.
#define COMBO_ONLY_FROM_LAYER 0

// Pressing the Shift key continues Caps Word and inverts the shift state
#define CAPS_WORD_INVERT_ON_SHIFT

// Default tapping term for mod-tap, layer-tap, and tap-dance keys.
#define TAPPING_TERM 200
// Allow per-key overrides via get_tapping_term() in keymap.c.
#define TAPPING_TERM_PER_KEY

// Use right CTRL key to neutralize modifier taps when cancelled.
#define DUMMY_MOD_NEUTRALIZER_KEYCODE KC_RIGHT_CTRL

// Neutralize left ALT and left GUI (Default value)
#define MODS_TO_NEUTRALIZE {MOD_BIT(KC_LEFT_ALT), MOD_BIT(KC_LEFT_GUI)}
