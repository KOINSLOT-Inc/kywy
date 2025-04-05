// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

typedef enum : uint16_t {
  START_SCREEN = Kywy::Events::USER_EVENTS,
  GAME_OVER,
} SpelunkerSignal;

const uint8_t splashScreenBMP[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
  0x00, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x20, 0x08, 0x00, 0x20, 0x01, 0x00, 0x00, 0x04,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x80,
  0x04, 0x10, 0x00, 0x40, 0x00, 0x00, 0x00, 0x02, 0x00, 0x90, 0x00, 0x00,
  0x18, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x80,
  0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x08, 0x81, 0x00, 0x00, 0x00, 0x40,
  0x02, 0x00, 0x00, 0x80, 0x11, 0x00, 0x04, 0x00, 0x00, 0x20, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
  0x02, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x08, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x10, 0x02, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x03, 0x84, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x0f, 0x00, 0x00, 0x04, 0x02, 0x00, 0xb6, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x07, 0x80, 0x00, 0x84, 0x01, 0x8c,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0xc0, 0x00, 0x02, 0x00, 0x60, 0x01, 0xf7, 0x40, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x81, 0x00, 0x10,
  0x00, 0x14, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xbc, 0x00, 0x01, 0x00, 0x08, 0x80, 0xbf, 0xf8, 0x00, 0x00, 0x00,
  0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5e, 0x00, 0x00, 0x80, 0x04,
  0x48, 0x00, 0xb7, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x2f, 0x02, 0x00, 0x40, 0x03, 0x62, 0x44, 0x4f, 0x68, 0x00, 0x00,
  0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x35, 0x82, 0x01, 0x20, 0x00,
  0x81, 0x08, 0x08, 0x36, 0x90, 0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x1b, 0xc4, 0x00, 0xd8, 0x00, 0x70, 0x43, 0xfd, 0x90, 0x70, 0x00,
  0x00, 0x04, 0x50, 0x02, 0x10, 0x00, 0x00, 0x0e, 0xf0, 0x00, 0x24, 0x00,
  0x08, 0xb4, 0x1b, 0xc9, 0x0a, 0x80, 0x00, 0x00, 0x00, 0x10, 0x20, 0x00,
  0x00, 0x05, 0xac, 0x00, 0x03, 0x00, 0x06, 0x2f, 0xee, 0xfc, 0x10, 0x60,
  0x00, 0x02, 0x00, 0x01, 0x40, 0x00, 0x00, 0x02, 0xdf, 0x00, 0x10, 0x80,
  0x03, 0xdb, 0x75, 0x95, 0x52, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0x2b, 0x80, 0x24, 0x60, 0x00, 0x76, 0xbb, 0x7b, 0xfc, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x05, 0xd5, 0x61, 0x49, 0x58,
  0x00, 0x1d, 0xde, 0xed, 0x13, 0xe4, 0x00, 0x00, 0x00, 0x38, 0x00, 0x02,
  0x00, 0x04, 0xfb, 0xf8, 0x00, 0x46, 0x00, 0x0b, 0x6b, 0xb7, 0xb6, 0x58,
  0x00, 0x08, 0x01, 0x60, 0x00, 0x00, 0x00, 0x84, 0x3c, 0x54, 0x02, 0x51,
  0x00, 0x03, 0xb6, 0xda, 0xda, 0xd0, 0x80, 0x00, 0x01, 0xc4, 0x00, 0x02,
  0x00, 0x00, 0x0f, 0xaf, 0x80, 0x21, 0xc0, 0x00, 0x59, 0xff, 0xeb, 0x98,
  0x00, 0x01, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00, 0x03, 0xda, 0xf1, 0x0b,
  0xf0, 0x00, 0x00, 0x00, 0x7d, 0x44, 0x08, 0x50, 0x32, 0x00, 0x00, 0x02,
  0x01, 0x00, 0x01, 0xef, 0xbf, 0xa2, 0xbc, 0x00, 0x08, 0x0f, 0xce, 0xe0,
  0x92, 0x01, 0xec, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x7d, 0xf4, 0x8d,
  0x6f, 0x00, 0x02, 0x0a, 0xd7, 0x50, 0x0f, 0xff, 0x92, 0x00, 0x40, 0x04,
  0x00, 0x20, 0x00, 0x36, 0xbc, 0xf7, 0xdb, 0x80, 0x00, 0x1d, 0xfb, 0xbe,
  0xfe, 0x97, 0x60, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x3b, 0xe7, 0xed,
  0x75, 0xe0, 0x00, 0x1e, 0xad, 0xd7, 0x2f, 0x7b, 0x80, 0x01, 0x00, 0x00,
  0x00, 0x80, 0x00, 0x0e, 0xc4, 0xfb, 0xbe, 0xb0, 0x00, 0x03, 0x76, 0xac,
  0x14, 0x9e, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x07, 0xf3, 0x7e,
  0xd7, 0x7e, 0x00, 0x00, 0xdb, 0x7b, 0xbd, 0x64, 0x00, 0x00, 0x20, 0x20,
  0x02, 0x00, 0x00, 0x03, 0xaf, 0xeb, 0x6b, 0xab, 0xc0, 0x00, 0x3d, 0xdd,
  0x68, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf2, 0xbd,
  0x7d, 0xdd, 0xb0, 0x00, 0x07, 0xd6, 0xdf, 0xe0, 0x00, 0x00, 0x10, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xfd, 0xd6, 0xba, 0xef, 0x64, 0x30, 0x00, 0x7f,
  0x69, 0x40, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x10, 0x00, 0xf7, 0x6b,
  0xdf, 0x6b, 0xb0, 0x3e, 0x00, 0x0d, 0xbf, 0xf4, 0x00, 0x00, 0x08, 0x00,
  0x00, 0x00, 0x10, 0x02, 0xbd, 0xbd, 0xab, 0x75, 0xd5, 0x6d, 0xa0, 0x03,
  0xde, 0x2d, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x5b, 0x7e,
  0xf6, 0xde, 0xe0, 0x77, 0x7f, 0x00, 0x3c, 0x81, 0x00, 0x02, 0x04, 0x00,
  0x00, 0x08, 0x00, 0x11, 0xcd, 0xad, 0xbb, 0xb6, 0xb0, 0x6d, 0xde, 0xf8,
  0x0f, 0x6b, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x02, 0x76, 0xf5,
  0x6d, 0xed, 0xd0, 0xdb, 0x6b, 0x57, 0x81, 0xc4, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0d, 0xbb, 0xbb, 0xff, 0xff, 0xa0, 0xf6, 0xf6, 0xdc,
  0xf8, 0x2a, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a, 0xdd, 0xbf,
  0xc0, 0x03, 0xe0, 0xbf, 0x5d, 0xab, 0x4f, 0x1d, 0x40, 0x04, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1d, 0xae, 0xf0, 0x00, 0x00, 0x00, 0xcb, 0xb7, 0x7d,
  0xbb, 0xf4, 0x68, 0x04, 0x00, 0x00, 0x00, 0x00, 0x04, 0x2d, 0xf7, 0x60,
  0x00, 0x00, 0x00, 0x3e, 0xcd, 0xd6, 0xda, 0x77, 0xc0, 0x04, 0x00, 0x00,
  0x00, 0x00, 0x04, 0x16, 0xb5, 0xc8, 0x00, 0x00, 0x00, 0x03, 0xba, 0xdb,
  0xb7, 0x59, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0x04, 0x2b, 0x5e, 0x9f,
  0x00, 0x00, 0x00, 0x00, 0x6f, 0x6d, 0xdd, 0xa5, 0xf8, 0x00, 0x05, 0x00,
  0x00, 0x06, 0x00, 0x17, 0xeb, 0xb9, 0x80, 0x00, 0x00, 0x00, 0x35, 0xbd,
  0x6b, 0xfa, 0x88, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x5c, 0xbd, 0x3f,
  0xc0, 0x00, 0x00, 0x00, 0x0e, 0xd6, 0xf6, 0x5f, 0x0d, 0x00, 0x06, 0x00,
  0x00, 0x00, 0x02, 0x27, 0xdb, 0x67, 0xf0, 0x00, 0x00, 0x00, 0x06, 0xef,
  0x5d, 0xd5, 0x0a, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x1d, 0x6e, 0xfd,
  0xd3, 0x00, 0x00, 0x00, 0x03, 0x75, 0xae, 0xec, 0x18, 0x04, 0x00, 0x00,
  0x01, 0x40, 0x02, 0x5b, 0xb0, 0x77, 0x77, 0xe0, 0x00, 0x00, 0x03, 0xbb,
  0xf6, 0xb8, 0x14, 0x00, 0x02, 0x00, 0x01, 0x00, 0x02, 0x5b, 0x60, 0x3e,
  0xa7, 0xc0, 0x00, 0x00, 0x07, 0x6d, 0x5b, 0x68, 0x20, 0x00, 0x02, 0x00,
  0x00, 0x00, 0x00, 0x2d, 0x80, 0x0e, 0xcd, 0xc0, 0x00, 0x00, 0x0e, 0xde,
  0xed, 0xd0, 0x38, 0x08, 0x00, 0x00, 0x00, 0x00, 0x03, 0x2f, 0x00, 0x03,
  0x93, 0x80, 0x68, 0x00, 0x0c, 0xb7, 0x6e, 0xf0, 0x28, 0x00, 0x01, 0x00,
  0x00, 0x10, 0x02, 0x5a, 0x00, 0x00, 0x3f, 0x80, 0xa5, 0x00, 0x0f, 0xe9,
  0x76, 0xa0, 0x44, 0x00, 0x00, 0x80, 0x00, 0x08, 0x00, 0x2e, 0x00, 0x00,
  0x1f, 0x00, 0x56, 0x80, 0x01, 0xff, 0xf7, 0x40, 0xe8, 0x10, 0x00, 0x00,
  0x00, 0x08, 0x00, 0x1a, 0x00, 0x00, 0x07, 0x00, 0x54, 0xa8, 0x00, 0x00,
  0x0f, 0x80, 0xb0, 0x10, 0x00, 0x00, 0x20, 0x04, 0x02, 0x4a, 0x00, 0x00,
  0x00, 0x01, 0x56, 0x26, 0x00, 0x00, 0x00, 0x01, 0x48, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x02, 0x26, 0x00, 0x00, 0x00, 0x00, 0x2d, 0xa8, 0x00, 0x00,
  0x00, 0x01, 0x70, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x5b, 0x00, 0x00,
  0x00, 0x00, 0xab, 0x28, 0x00, 0x00, 0x00, 0x00, 0xc4, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x02, 0x65, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x16, 0x00, 0x00,
  0x00, 0x00, 0x50, 0x00, 0x01, 0x80, 0x01, 0x86, 0x00, 0x0b, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x07, 0xa4, 0x20, 0x00, 0x80,
  0x00, 0x02, 0x01, 0x16, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00,
  0x00, 0x3a, 0x48, 0x20, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2f, 0x60, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x10, 0x00, 0x00, 0x00,
  0x20, 0x01, 0x00, 0x5f, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xbe, 0x04, 0x90, 0x28, 0x00, 0x80, 0x20, 0x00, 0x00, 0x7a, 0xbe, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x75, 0x06, 0x20, 0x00, 0x00, 0x00,
  0x20, 0x01, 0x03, 0xb6, 0x57, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e,
  0xdb, 0x78, 0x00, 0x04, 0x0a, 0x00, 0x00, 0x00, 0x1f, 0xdf, 0xfc, 0x3c,
  0x00, 0x00, 0x00, 0x00, 0x1f, 0xe9, 0xdd, 0x80, 0xc0, 0x40, 0x08, 0x00,
  0x00, 0x00, 0xfa, 0x7a, 0x8c, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x1b, 0x6e,
  0xe2, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0xed, 0xc0, 0x00,
  0x7c, 0x00, 0x00, 0x00, 0x36, 0xdb, 0x7c, 0x48, 0x00, 0x80, 0x10, 0x00,
  0x00, 0x03, 0x55, 0x6f, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0xdd, 0xf7,
  0xcf, 0x01, 0x00, 0x82, 0x12, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xbf, 0xff,
  0x00, 0x08, 0x00, 0x03, 0xee, 0xaf, 0x62, 0xfa, 0x00, 0x00, 0x00, 0x02,
  0x03, 0xcf, 0xf0, 0x43, 0x6a, 0xe5, 0xff, 0xcf, 0x80, 0x1e, 0xb7, 0x69,
  0xf9, 0xb8, 0x00, 0x80, 0x20, 0x00, 0x06, 0x3e, 0x40, 0x41, 0x75, 0x5a,
  0xe1, 0xc6, 0xfe, 0x37, 0x6b, 0xd4, 0x6f, 0x46, 0x00, 0x00, 0x20, 0x00,
  0x0c, 0x70, 0x40, 0x00, 0xdb, 0xb7, 0x57, 0xe3, 0x77, 0x1b, 0xdc, 0xaa,
  0x27, 0x81, 0x00, 0x00, 0x40, 0x00, 0x1c, 0x40, 0x40, 0x00, 0xdd, 0xdb,
  0xac, 0x63, 0xff, 0x8f, 0x77, 0xa4, 0x11, 0xc0, 0x00, 0x80, 0x40, 0x08,
  0x60, 0x00, 0x00, 0x04, 0x36, 0xb5, 0xf4, 0x00, 0x00, 0x01, 0xb6, 0x90,
  0x04, 0x20, 0x00, 0x02, 0xc0, 0x00, 0x40, 0x00, 0x40, 0x09, 0xeb, 0x6e,
  0x5f, 0x00, 0x00, 0x01, 0xbb, 0x68, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00,
  0x40, 0x00, 0xc0, 0x28, 0x13, 0xdb, 0xeb, 0xc0, 0x00, 0x03, 0xd6, 0x84,
  0x1c, 0x43, 0x80, 0x40, 0x40, 0x00, 0x00, 0x00, 0x00, 0x20, 0x0d, 0x6d,
  0xb5, 0xbf, 0xff, 0xff, 0x6d, 0x54, 0x10, 0x00, 0x80, 0x00, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x08, 0x22, 0xb6, 0xbe, 0xdb, 0x76, 0xdb, 0xbb, 0x40,
  0x10, 0x01, 0x00, 0x00, 0x80, 0x40, 0x00, 0x08, 0x03, 0x00, 0x07, 0x6d,
  0xd6, 0xa4, 0xad, 0xad, 0x6c, 0xc8, 0x20, 0x81, 0x00, 0x04, 0x00, 0x40,
  0x20, 0x00, 0x00, 0x00, 0x01, 0xbb, 0x6b, 0x7b, 0xdb, 0x76, 0xda, 0x80,
  0x20, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x8a, 0xd6,
  0xdd, 0xde, 0xed, 0xb7, 0xb4, 0x20, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00,
  0x24, 0x04, 0x00, 0x01, 0x00, 0xdd, 0xb6, 0xab, 0x5b, 0x6d, 0x6a, 0x40,
  0x44, 0x08, 0x00, 0x01, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0xb6,
  0xdb, 0xf5, 0xb7, 0xb6, 0xd9, 0x00, 0x4c, 0x08, 0x04, 0x01, 0x00, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x0c, 0xb6, 0xed, 0x5e, 0xed, 0x6d, 0xb0, 0x00,
  0x42, 0x08, 0x08, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x41, 0x17,
  0x6e, 0xab, 0x76, 0xdb, 0x4a, 0x00, 0x80, 0x02, 0x00, 0x01, 0x00, 0x00,
  0x04, 0x01, 0x00, 0x00, 0x00, 0x22, 0xdb, 0xf6, 0xb7, 0xb7, 0x98, 0x00,
  0x80, 0x00, 0x10, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3d,
  0xb5, 0x5d, 0xda, 0xd9, 0xf1, 0x00, 0x01, 0x00, 0x10, 0x01, 0x80, 0x00,
  0x04, 0x00, 0x00, 0x20, 0x24, 0x23, 0x6e, 0xdb, 0x6d, 0xa8, 0x78, 0x04,
  0x00, 0x60, 0x00, 0x01, 0x01, 0x00, 0x02, 0x00, 0x00, 0x30, 0x20, 0x13,
  0xb7, 0xb6, 0xdb, 0x52, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x01, 0x80, 0x15, 0xb5, 0x6d, 0xb6, 0xe4, 0x06, 0x01,
  0x14, 0x00, 0x08, 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x01, 0x02, 0x17,
  0xdb, 0xb6, 0xdd, 0xa0, 0x03, 0x00, 0x24, 0x00, 0x50, 0x01, 0x00, 0x00,
  0x00, 0x80, 0x00, 0x04, 0x20, 0x0b, 0x6e, 0xdb, 0x6d, 0x09, 0x71, 0xc1,
  0x05, 0x00, 0x10, 0x21, 0x40, 0x00, 0x00, 0x40, 0x00, 0x04, 0x20, 0xb1,
  0xf5, 0xb6, 0xee, 0x10, 0x10, 0x20, 0x0f, 0x00, 0x00, 0x20, 0x08, 0x00,
  0x00, 0x40, 0x00, 0x00, 0x00, 0x07, 0x6e, 0xdd, 0xb7, 0x00, 0x00, 0x10,
  0x09, 0x00, 0x80, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x18, 0x00, 0x0a,
  0x7d, 0xb6, 0xd9, 0x40, 0x00, 0x04, 0x90, 0x02, 0x00, 0x24, 0x10, 0x00,
  0x00, 0x20, 0x00, 0x00, 0x00, 0x43, 0xf6, 0xdb, 0x6e, 0x70, 0x40, 0x03,
  0x10, 0x82, 0x1e, 0x64, 0x10, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x3c,
  0x2d, 0xed, 0xb4, 0x30, 0x04, 0x40, 0x91, 0x82, 0x00, 0x0c, 0x10, 0x00,
  0x00, 0x20, 0x00, 0x00, 0x81, 0x02, 0xdb, 0x5b, 0x68, 0xa8, 0x00, 0x00,
  0x20, 0x06, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x06, 0x3f, 0xff, 0xfa,
  0xf6, 0xee, 0xd0, 0x10, 0x00, 0x00, 0x24, 0x00, 0x00, 0x08, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x6e, 0xff, 0xdb, 0x6d, 0xb2, 0x22, 0x08, 0x08,
  0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x08, 0x00, 0x00, 0x15, 0x40,
  0x43, 0x6d, 0xe3, 0x12, 0xc6, 0x30, 0x00, 0x10, 0x40, 0x14, 0x04, 0x00,
  0x60, 0x00, 0x00, 0x01, 0xb8, 0x0d, 0x3f, 0xdb, 0x44, 0xd1, 0x00, 0x24,
  0x04, 0x10, 0x84, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
  0x13, 0x6d, 0x82, 0x48, 0x00, 0x00, 0x00, 0x10, 0x06, 0x10, 0x00, 0x00,
  0x00, 0x04, 0x00, 0x00, 0x00, 0x02, 0x03, 0x76, 0xc1, 0x40, 0x80, 0x00,
  0x40, 0x70, 0x00, 0x30, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x22,
  0x06, 0xdb, 0x20, 0x33, 0x60, 0xc0, 0x40, 0x42, 0x00, 0x04, 0x00, 0x40,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x0b, 0x6f, 0x01, 0x81, 0x30, 0x80,
  0x80, 0x81, 0x00, 0x10, 0x00, 0x00, 0x08, 0x01, 0x00, 0x00, 0x00, 0x07,
  0x73, 0x6d, 0x90, 0x40, 0x88, 0x00, 0xc1, 0x11, 0x88, 0x20, 0x08, 0x00,
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0xb6, 0x41, 0x03, 0x44, 0xc0,
  0x02, 0x00, 0x80, 0x20, 0x00, 0x00, 0x00, 0x11, 0x30, 0x00, 0x00, 0x00,
  0x01, 0xdb, 0x00, 0x21, 0xb2, 0x40, 0x02, 0x00, 0x00, 0x20, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x12, 0x6e, 0x84, 0x10, 0x91, 0x00,
  0x00, 0x00, 0x00, 0x28, 0x10, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
  0x01, 0x77, 0x00, 0x08, 0x48, 0x80, 0x40, 0x10, 0x20, 0x08, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x40, 0x14, 0x00, 0xba, 0x24, 0x04, 0x04, 0x40,
  0x04, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x10,
  0x02, 0x2f, 0x01, 0x02, 0x02, 0x30, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08,
  0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0xda, 0x40, 0x20, 0x11, 0x08,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x10, 0x37, 0x10, 0x08, 0x88, 0x82, 0x06, 0x00, 0x08, 0x14, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x5d, 0x09, 0x0a, 0x00, 0x43,
  0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x6f, 0x20, 0x00, 0x00, 0x20, 0x62, 0x03, 0x00, 0x00, 0x40, 0x00,
  0x00, 0x40, 0x02, 0x00, 0x00, 0x40, 0x00, 0x34, 0x50, 0x08, 0x00, 0x90,
  0x04, 0x00, 0x00, 0x40, 0x00, 0x20, 0x00, 0x20, 0x04, 0xe0, 0x00, 0x00,
  0x01, 0x3b, 0x10, 0x20, 0x00, 0x4c, 0x02, 0x00, 0x00, 0x00, 0x00, 0x40,
  0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x2f, 0x20, 0x00, 0x00, 0x00,
  0x04, 0x10, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
  0x00, 0x0d, 0x80, 0x00, 0x00, 0x08, 0xc2, 0x08, 0x00, 0x04, 0x00, 0x20,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x80, 0x00, 0x40, 0x00,
  0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x20, 0x00,
  0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x80, 0x00,
  0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x40, 0x00, 0x7c, 0x1e,
  0x1f, 0xd8, 0x0c, 0x05, 0xbc, 0x20, 0x4f, 0xe7, 0xc0, 0x00, 0x00, 0x00,
  0x40, 0x00, 0x00, 0x00, 0x86, 0x21, 0x30, 0xd8, 0x0c, 0x0d, 0xc6, 0x60,
  0xd8, 0x6c, 0x20, 0x00, 0x80, 0x00, 0x00, 0x00, 0x08, 0x01, 0x82, 0x61,
  0xb0, 0x18, 0x0c, 0x0d, 0xc6, 0x60, 0xd8, 0x0c, 0x30, 0x00, 0x01, 0x80,
  0x00, 0x0c, 0x04, 0x01, 0x80, 0x61, 0xb0, 0x18, 0x0c, 0x0d, 0x83, 0x60,
  0x98, 0x0c, 0x30, 0x00, 0x01, 0x00, 0x20, 0x08, 0x00, 0x00, 0xc0, 0x61,
  0xb8, 0x18, 0x0c, 0x0d, 0x83, 0x63, 0x1c, 0x0e, 0x20, 0x00, 0x0c, 0x00,
  0x20, 0x01, 0x00, 0x00, 0x70, 0x61, 0x9f, 0x18, 0x0c, 0x0d, 0x83, 0x7c,
  0x0f, 0x8f, 0xc0, 0x00, 0x30, 0x00, 0x40, 0x03, 0x00, 0x00, 0x0c, 0x71,
  0x30, 0x18, 0x0c, 0x0d, 0x83, 0x67, 0x18, 0x0d, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x06, 0x00, 0x00, 0x06, 0x6e, 0x30, 0x18, 0x0c, 0x0d, 0x83, 0x61,
  0x98, 0x0c, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x83, 0x60,
  0x30, 0x18, 0x04, 0x09, 0x83, 0x60, 0xd8, 0x0c, 0x60, 0x01, 0x20, 0x00,
  0x00, 0x00, 0x10, 0x01, 0x83, 0x60, 0x30, 0x18, 0x04, 0x09, 0x83, 0x60,
  0xd8, 0x0c, 0x30, 0x00, 0x18, 0x00, 0x00, 0x20, 0x00, 0x00, 0x86, 0x60,
  0x30, 0x18, 0x02, 0x11, 0x83, 0x60, 0xd8, 0x0c, 0x30, 0x00, 0x08, 0x20,
  0x00, 0x00, 0x00, 0x00, 0x7c, 0x60, 0x3f, 0xcf, 0xe1, 0xe1, 0x83, 0x60,
  0x1f, 0xec, 0x30, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x00, 0x20, 0xfd, 0xfb, 0xf7, 0xef, 0xc4, 0x0f, 0xdf, 0xbe, 0x3e, 0xfc,
  0x3f, 0x7d, 0xfb, 0xf7, 0xc0, 0x82, 0x00, 0x20, 0x85, 0x0a, 0x04, 0x08,
  0x04, 0x08, 0x10, 0x08, 0x08, 0x84, 0x20, 0x11, 0x0a, 0x11, 0x00, 0x02,
  0x00, 0x00, 0xfd, 0xf3, 0xf7, 0xef, 0xc4, 0x0f, 0xdf, 0x88, 0x08, 0x84,
  0x3f, 0x11, 0xfb, 0xe1, 0x00, 0x20, 0x00, 0x00, 0x81, 0x0a, 0x00, 0x20,
  0x44, 0x08, 0x10, 0x08, 0x08, 0x84, 0x01, 0x11, 0x0a, 0x11, 0x01, 0x20,
  0x04, 0x00, 0x81, 0x0b, 0xf7, 0xef, 0xc7, 0xef, 0xd0, 0x08, 0x08, 0xfc,
  0x3f, 0x11, 0x0a, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t spelunkerBMP[] = {
  0b00000000,  //
  0b00111110,  //
  0b10001000,  //
  0b11111110,  //
  0b00111111,  //
  0b00100010,  //
  0b00000000,  //
  0b00000000,  //
};
const uint8_t *spelunkerFrames[] = { spelunkerBMP };
Sprite spelunkerSprite(spelunkerFrames, 1, 8, 8);

class spelunkerManager : public Actor::Actor {
public:
  int xPosition = 10;

  int yVelocity = -5;
  int yPosition = 64;

  int padding = 5;

  bool buttonPressed = false;

  void initialize() {
    spelunkerSprite.setDisplay(&engine.display);
    spelunkerSprite.setPosition(xPosition, yPosition);
    spelunkerSprite.setVisible(true);
    spelunkerSprite.setNegative(true);
    spelunkerSprite.setColor(WHITE);
    spelunkerSprite.render();
  }

  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        Serial.println("button left pressed");
        buttonPressed = true;
        break;
      case Kywy::Events::BUTTON_LEFT_RELEASED:
        Serial.println("button left released");
        buttonPressed = false;
        break;
      case Kywy::Events::TICK:
        // up/down movement
        yPosition += yVelocity;

        if (yPosition < padding) {
          yPosition = padding;
          yVelocity = 0;
        }

        if (yPosition > KYWY_DISPLAY_HEIGHT - 8)
          yPosition = KYWY_DISPLAY_HEIGHT - 8;

        // gravity
        if (yVelocity < 7 && !buttonPressed)
          yVelocity += 1;

        if (buttonPressed)
          yVelocity = -5;

        // apply changes
        spelunkerSprite.setPosition(xPosition, yPosition);
        spelunkerSprite.render();
        break;
    }
  }

} spelunkerManager;

class columnManager : public Actor::Actor {
public:
  static const uint8_t numColumns = 16;
  static const uint8_t columnWidth = KYWY_DISPLAY_WIDTH / numColumns;

  uint8_t topColumns[(numColumns + 1)];
  uint8_t bottomColumns[(numColumns + 1)];

  uint8_t numEntraceColumns = 6;
  uint8_t numEntraceColumnsLeft = numEntraceColumns;

  // instead of rotating both column lists as columns scroll across the screen we just change the start index that we
  // consider the head of the list and modulo indices back around to the true head of the list if they overflow
  uint8_t startIndex = 0;

  // how fast the columns should move
  uint8_t pixelsPerTick = 4;
  uint8_t ticksPerColumnWidth = columnWidth / pixelsPerTick;
  uint8_t tickCounter = 0;

  // message
  ::Actor::Message gameOverMessage = ::Actor::Message(GAME_OVER);

  void initialize() {
    // initialize column lists to zero
    memset(topColumns, 0, sizeof(topColumns));
    memset(bottomColumns, 0, sizeof(bottomColumns));

    // set up cave entrace columns
    for (int i = 0; i < numEntraceColumns; i++) {
      topColumns[(numColumns + 1) - numEntraceColumns + i] = 6 * i;
      bottomColumns[(numColumns + 1) - numEntraceColumns + i] = 6 * i;
    }
    startIndex = 0;
  }

  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::TICK:
        int offset = tickCounter * pixelsPerTick;

        for (int i = 0; i < (numColumns + 1); i++) {
          int columnIndex = (startIndex + i) % (numColumns + 1);

          engine.display.fillRectangle(i * columnWidth - offset, 0, columnWidth, topColumns[columnIndex], Display::Object2DOptions().color(WHITE));

          engine.display.fillRectangle(i * columnWidth - offset, topColumns[columnIndex], columnWidth, KYWY_DISPLAY_HEIGHT - bottomColumns[columnIndex] - topColumns[columnIndex]);

          engine.display.fillRectangle(i * columnWidth - offset, KYWY_DISPLAY_HEIGHT - bottomColumns[columnIndex], columnWidth, bottomColumns[columnIndex], Display::Object2DOptions().color(WHITE));
        }

        // record height of the most recently created column
        int lastColumnIndex = (startIndex - 1) % (numColumns + 1);
        if (lastColumnIndex == -1)
          lastColumnIndex = (numColumns + 1) - 1;

        uint8_t lastColumnHeight = topColumns[lastColumnIndex];

        if (tickCounter < (ticksPerColumnWidth - 1)) {
          tickCounter++;
        } else {
          // move head of list to effectively rotate the column lists
          startIndex = (startIndex + 1) % (numColumns + 1);

          // reset tick counter
          tickCounter = 0;

          // choose a new column height around the height of the most recently created column
          int newColumnIndex = (startIndex - 1) % (numColumns + 1);
          if (newColumnIndex == -1)
            newColumnIndex = (numColumns + 1) - 1;

          int8_t newColumnHeight;
          if (random(2) == 1) {  // randomly choose up or down
            newColumnHeight = lastColumnHeight + 6;
          } else {
            newColumnHeight = lastColumnHeight - 6;
          }
          newColumnHeight = fmin(KYWY_DISPLAY_HEIGHT - 72, fmax(0, newColumnHeight));

          topColumns[newColumnIndex] = newColumnHeight;
          bottomColumns[newColumnIndex] = KYWY_DISPLAY_HEIGHT - 72 - newColumnHeight;  // pin tunnel width at 72
        }

        // check for collisions
        uint8_t overlapColumnsStart = 1;
        uint8_t overlapColumnsEnd = (8 / columnWidth) + (offset ? 1 : 0);
        bool collided = false;
        for (int i = overlapColumnsStart; i <= overlapColumnsEnd; i++) {
          int columnIndex = (startIndex + i) % (numColumns + 1);

          // collisions with top columns
          //
          // `- 1` because top pixels of 8x8 spelunker sprite are all off
          if (spelunkerManager.yPosition < (topColumns[columnIndex] - 1))
            collided = true;

          // collisions with bottom columns
          //
          // `+ 5` because bottom pixels of 8x8 spelunker sprite are on the sixth row
          if ((spelunkerManager.yPosition + 5) > (KYWY_DISPLAY_HEIGHT - bottomColumns[columnIndex]))
            collided = true;
        }

        if (collided) {
          publish(&gameOverMessage);
        }

        break;
    }
  }
} columnManager;

class GameManager : public Actor::Actor {
public:
  int score = 0;
  int highScore = 0;

  bool inMenu = true;

  void drawScore() {
    char msg[16];
    snprintf(msg, sizeof(msg), "%d", (uint16_t)score);
    engine.display.fillRectangle(KYWY_DISPLAY_WIDTH - 40, 0, 40, 14, Display::Object2DOptions().color(WHITE));
    engine.display.drawText(KYWY_DISPLAY_WIDTH - 33, 3, msg);
  }

  void initialize() {}

  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case START_SCREEN:
        {
          inMenu = true;
          spelunkerManager.unsubscribe(&engine.clock);
          columnManager.unsubscribe(&engine.clock);
          unsubscribe(&engine.clock);
          engine.display.drawBitmap(0, 0, 144, 168, (uint8_t *)splashScreenBMP);
          engine.display.update();
          subscribe(&engine.input);
          break;
        }
      case Kywy::Events::TICK:
        {
          if (inMenu)
            break;

          score += 1;
          drawScore();

          unsigned long updateStart = millis();
          engine.display.update();

          break;
        }
      case GAME_OVER:
        {
          inMenu = true;
          spelunkerManager.unsubscribe(&engine.clock);
          columnManager.unsubscribe(&engine.clock);
          subscribe(&engine.input);
          unsubscribe(&engine.clock);

          if (score > highScore)
            highScore = score;

          engine.display.clear();

          engine.display.drawText(5, 5, "GAME OVER");
          char msg[32];
          snprintf(msg, sizeof(msg), "Score: %d", (uint16_t)score);
          engine.display.drawText(5, 15, msg);
          snprintf(msg, sizeof(msg), "High Score: %d", highScore);
          engine.display.drawText(5, 25, msg);
          engine.display.drawText(5, 45, "Press left button");
          engine.display.drawText(5, 55, "to try again.");
          engine.display.update();
          break;
        }
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        {  // start a new game
          inMenu = false;
          unsubscribe(&engine.input);

          spelunkerManager.xPosition = 10;
          spelunkerManager.yPosition = 64;
          spelunkerManager.yVelocity = -5;
          spelunkerManager.initialize();
          columnManager.initialize();

          score = 0;

          engine.display.fillRectangle(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT);

          columnManager.subscribe(&engine.clock);
          spelunkerManager.subscribe(&engine.clock);
          subscribe(&engine.clock);
          break;
        }
    }
  }
} gameManager;

void setup() {
  engine.start();

  // make sure column manager subscribes to the ticker before the spelunkerManager so that
  // the columns don't override the spelunker sprite
  columnManager.subscribe(&engine.clock);

  spelunkerManager.subscribe(&engine.input);
  spelunkerManager.subscribe(&engine.clock);

  gameManager.subscribe(&columnManager);
  gameManager.subscribe(&engine.clock);  // sub to clock last so score is written last over top of everything

  columnManager.start();
  spelunkerManager.start();
  gameManager.start();

  ::Actor::Message message(START_SCREEN);
  gameManager.handle(&message);
}

void loop() {
  delay(1000);
}
