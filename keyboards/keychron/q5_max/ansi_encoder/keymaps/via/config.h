// Copyright 2024 rootiest
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Always resolve combo keycodes from BASE (layer 0) so the
// COMM+DOT+SLSH fallback combo fires regardless of the active layer.
#define COMBO_ONLY_FROM_LAYER 0

// Skip autocorrect on very short words to reduce false positives.
#define AUTOCORRECT_MIN_LENGTH 4
