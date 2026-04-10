// Copyright 2024 rootiest
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * qmk-host Shared HID Protocol
 * =============================
 * Bi-directional Raw HID protocol shared between QMK firmware and the
 * qmk-host Rust application.  All values MUST match between both sides.
 *
 * Physical layer (QMK defaults, NOT overridden):
 *   Usage Page : 0xFF60
 *   Usage ID   : 0x0061
 *   Packet size: RAW_EPSIZE (32 bytes)
 *
 * Packet layout:
 *   Byte 0    : Command ID
 *   Byte 1    : Source Device ID
 *   Byte 2    : Flags
 *   Bytes 3-31: Payload (29 bytes)
 *
 * Command IDs use the range 0x40-0x7E to avoid any overlap with VIA's
 * protocol (0x01-0x3F) or the reserved unhandled sentinel (0xFF).
 */

// ---------------------------------------------------------------------------
// Command IDs
// ---------------------------------------------------------------------------
#define HID_CMD_LAYER_SYNC   0x40u   // Layer state sync (keyboard ↔ host ↔ keyboard)
#define HID_CMD_VOLUME       0x41u   // System volume level (host → keyboard)
#define HID_CMD_BRIGHTNESS   0x42u   // Screen brightness level (host → keyboard)
#define HID_CMD_ACTIVE_APP   0x43u   // Active window/app name (reserved — future)
#define HID_CMD_ACK          0x7Eu   // Generic acknowledgement

// ---------------------------------------------------------------------------
// Source Device IDs
// ---------------------------------------------------------------------------
#define HID_DEV_HOST    0x00u   // qmk-host Rust application
#define HID_DEV_Q5MAX   0x01u   // Keychron Q5 Max (this firmware)
#define HID_DEV_NUMPAD  0x02u   // Numpad (future second keyboard)

// ---------------------------------------------------------------------------
// Packet Flags (Byte 2)
// ---------------------------------------------------------------------------
#define HID_FLAG_QUERY    0x01u   // Request: send back current state, no change
#define HID_FLAG_RESPONSE 0x02u   // Response: this is a reply to a query

// ---------------------------------------------------------------------------
// Byte offsets within the 32-byte packet
// ---------------------------------------------------------------------------
#define HID_OFF_CMD     0u   // Command ID
#define HID_OFF_SRC     1u   // Source Device ID
#define HID_OFF_FLAGS   2u   // Flags
#define HID_OFF_PAYLOAD 3u   // Start of payload

// ---------------------------------------------------------------------------
// HID_CMD_LAYER_SYNC payload (from byte HID_OFF_PAYLOAD)
//   [0] Active layer index (0 = BASE … 5 = KEEB_CTL)
//   [1] Locked layers bitmask (bit N = layer N is locked)
// ---------------------------------------------------------------------------
#define HID_LAYER_OFF_ACTIVE 0u
#define HID_LAYER_OFF_LOCKED 1u

// ---------------------------------------------------------------------------
// HID_CMD_VOLUME payload
//   [0] Volume level, 0-100 (%)
// ---------------------------------------------------------------------------
#define HID_VOLUME_OFF_LEVEL 0u

// ---------------------------------------------------------------------------
// HID_CMD_BRIGHTNESS payload
//   [0] Brightness level, 0-100 (%)
// ---------------------------------------------------------------------------
#define HID_BRITE_OFF_LEVEL 0u

// ---------------------------------------------------------------------------
// HID_CMD_ACTIVE_APP payload (reserved — no firmware action yet)
//   [0..27] Null-terminated UTF-8 application name (max 28 bytes incl. NUL)
// ---------------------------------------------------------------------------
#define HID_APP_NAME_MAX 28u

// ---------------------------------------------------------------------------
// Convenience: first byte of payload as an absolute packet index
// ---------------------------------------------------------------------------
#define HID_PAYLOAD(offset) ((uint8_t)((HID_OFF_PAYLOAD) + (offset)))
