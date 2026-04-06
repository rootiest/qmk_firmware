// Copyright 2024 rootiest
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#include QMK_KEYBOARD_H

/*
 * Chord Unicode System
 * ====================
 * Activate chord mode with CHORD_KEY (Fn1 + Left Alt).
 *
 * Two activation styles:
 *   Tap  - tap CHORD_KEY, release it, then type the sequence within
 *          CHORD_TIMEOUT_MS.  Output fires on an unambiguous exact match.
 *   Hold - hold CHORD_KEY, type the sequence, then release CHORD_KEY.
 *          Output fires on exact match or on key release if buffer matches.
 *
 * While collecting:
 *   - Alpha / digit keys append to the sequence buffer (always lowercase).
 *   - Backspace deletes the last character.
 *   - Enter confirms the current buffer (useful when a shorter sequence is
 *     a prefix of a longer one with different output).
 *   - Escape cancels chord mode.
 *   - Any other key cancels chord mode silently.
 *
 * Prefix disambiguation:
 *   If the current buffer is an exact match AND a prefix of a longer entry
 *   that maps to a DIFFERENT character, a CHORD_DISAMBIG_MS timer fires
 *   and commits the shorter match if no more keys arrive in time.
 *   Aliases (different sequences → same output) coexist without conflict.
 */

/* Maximum sequence length (characters). */
#define CHORD_MAX_LEN 8

/* Milliseconds of idle time before tap-mode chord is cancelled. */
#define CHORD_TIMEOUT_MS 2000

/* Milliseconds to wait for more input when an exact match is also a prefix
 * of a longer entry with different output. */
#define CHORD_DISAMBIG_MS 300

/* Called once at startup to prepare the chord subsystem.
 * The caller (keyboard_post_init_user in keymap.c) is responsible for
 * setting the unicode input mode via set_unicode_input_mode(). */
void chord_init(void);

/* Call from process_record_user when CHORD_KEY is pressed. */
void chord_activate(void);

/* Call from process_record_user when CHORD_KEY is released. */
void chord_key_released(void);

/* Call from process_record_user for every other key event.
 * Returns false when the key was consumed by the chord subsystem (do not
 * pass through to the host), true otherwise. */
bool process_chord(uint16_t keycode, keyrecord_t *record);

/* Call from matrix_scan_user to drive timeouts. */
void chord_scan(void);
