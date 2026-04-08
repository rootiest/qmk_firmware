// Copyright 2024 rootiest
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Always resolve combo keycodes from BASE (layer 0) so the
// COMM+DOT+SLSH fallback combo fires regardless of the active layer.
#define COMBO_ONLY_FROM_LAYER 0
