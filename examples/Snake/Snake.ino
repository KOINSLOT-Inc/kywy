// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

#define DISPLAY_WIDTH  144
#define DISPLAY_HEIGHT 168
#define BLACK          0x0
#define WHITE          0xf

const uint8_t splashScreen[] = {
    0x3f, 0x7c, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xc9, 0xf9, 0xf2, 0x3e, 0x79, 0x3e, 0xfb, 0x7d, 0xb6,
  0xdb, 0x6d, 0xb6, 0xdb, 0xf6, 0xfe, 0xdb, 0x6f, 0x7b, 0xcf, 0xf8, 0xdb,
  0x7c, 0xf2, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xdf,
  0xff, 0xff, 0xfe, 0xc5, 0x7c, 0x39, 0x7c, 0x62, 0x5b, 0x6f, 0xef, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf5, 0xdf, 0xe2, 0xbe, 0x75,
  0xc6, 0x44, 0x7f, 0xfd, 0xfd, 0xb6, 0xdd, 0xdf, 0x7f, 0x77, 0xed, 0xfb,
  0x6d, 0xbf, 0xfd, 0xf3, 0x7e, 0x33, 0x18, 0x08, 0x20, 0x7f, 0xbf, 0xff,
  0xff, 0xfb, 0xef, 0xfe, 0xff, 0xbf, 0xff, 0xfc, 0xff, 0xe3, 0xbe, 0x2a,
  0xe0, 0x18, 0x0c, 0x77, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff,
  0xff, 0xf3, 0x23, 0xb0, 0xc0, 0x0c, 0x20, 0x7f, 0xfe, 0xff, 0xf7, 0x77,
  0xbb, 0xbf, 0xff, 0xdf, 0xbb, 0xf7, 0x77, 0x73, 0xdb, 0xf0, 0xee, 0x03,
  0x03, 0xfd, 0x00, 0x77, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff,
  0xff, 0xf0, 0x1c, 0x00, 0x6f, 0x85, 0x01, 0x7f, 0xc0, 0x7f, 0xdd, 0xde,
  0xee, 0xff, 0xff, 0x7e, 0xef, 0xdd, 0xdd, 0xf1, 0xbf, 0xf0, 0xe6, 0x80,
  0x77, 0xa0, 0x0b, 0xfb, 0xff, 0xff, 0xff, 0xf7, 0x7f, 0xef, 0xfe, 0xff,
  0xff, 0xb4, 0x00, 0x34, 0x3d, 0xae, 0x7f, 0xe0, 0x01, 0xbf, 0x7f, 0xfb,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x3c, 0x1f, 0xff,
  0x00, 0x00, 0x03, 0xff, 0xf7, 0x7f, 0xff, 0xbf, 0xef, 0xfe, 0xf7, 0xed,
  0xb7, 0xfe, 0x00, 0x30, 0x1f, 0xef, 0x00, 0x18, 0x0f, 0xf7, 0xff, 0xff,
  0xb7, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfe, 0xff, 0x80, 0x83, 0x6d, 0xff,
  0x00, 0x1a, 0x4f, 0xff, 0xbf, 0xf7, 0xff, 0xe0, 0x00, 0x0f, 0xff, 0xff,
  0xff, 0xff, 0x11, 0x00, 0x00, 0x00, 0x00, 0x7b, 0xfd, 0xff, 0xff, 0xff,
  0xfe, 0x00, 0x00, 0x00, 0xed, 0xff, 0xff, 0xdd, 0xf9, 0xf0, 0x00, 0x00,
  0x00, 0x7b, 0xdf, 0xed, 0xfd, 0xff, 0xfc, 0x7e, 0x00, 0x00, 0x7f, 0xfb,
  0xbb, 0xff, 0xf7, 0xf0, 0x00, 0x00, 0xce, 0x0d, 0xfe, 0xff, 0xff, 0x80,
  0x00, 0x3f, 0x80, 0x30, 0x00, 0x3f, 0xff, 0xff, 0xef, 0x83, 0xfe, 0x70,
  0x8e, 0x1e, 0xff, 0xff, 0xbf, 0x00, 0x00, 0x1f, 0x80, 0x30, 0x3f, 0x3f,
  0xff, 0x6f, 0xc7, 0x81, 0x7a, 0x3c, 0x64, 0x07, 0x7b, 0xdf, 0xfc, 0x00,
  0x00, 0x01, 0xf8, 0x38, 0x3c, 0x07, 0xff, 0xff, 0xde, 0x08, 0x06, 0x3e,
  0x76, 0x03, 0xbf, 0xfd, 0xf8, 0x1d, 0xc0, 0x01, 0x7c, 0x38, 0x7c, 0x03,
  0xdb, 0xfd, 0x3e, 0x0c, 0x06, 0x7a, 0x76, 0x21, 0xcf, 0xff, 0xf0, 0x1f,
  0xa0, 0x00, 0x3c, 0x38, 0x78, 0x0b, 0xff, 0xff, 0x30, 0x2d, 0x24, 0x6b,
  0xd3, 0xe0, 0xef, 0x7f, 0xe0, 0x7e, 0xc3, 0xfe, 0x3c, 0x3c, 0x79, 0xf9,
  0xff, 0xef, 0x98, 0x7f, 0x00, 0x1b, 0x63, 0xe0, 0xdd, 0xed, 0xc0, 0x77,
  0x87, 0xfe, 0x0c, 0x3c, 0x63, 0xf9, 0xff, 0x7f, 0x98, 0x7f, 0x02, 0x23,
  0x13, 0xfc, 0x0f, 0xff, 0xc3, 0xfb, 0x0f, 0xff, 0x8f, 0x3e, 0x63, 0xfc,
  0xff, 0xff, 0x01, 0xdf, 0x00, 0x62, 0xc3, 0xf8, 0x2f, 0xff, 0x87, 0xca,
  0x0f, 0xff, 0x8f, 0x3c, 0x42, 0x1c, 0xdf, 0xfb, 0x01, 0xfe, 0x00, 0x60,
  0x15, 0x6f, 0xff, 0x7e, 0x00, 0x96, 0x1e, 0x1f, 0xc3, 0x3c, 0x44, 0x1e,
  0x7d, 0xff, 0xff, 0xfd, 0xf0, 0xf5, 0x49, 0xff, 0xff, 0xfe, 0x12, 0x4a,
  0x18, 0x07, 0xe3, 0x3e, 0x44, 0x2e, 0x7f, 0xdf, 0xef, 0xe9, 0xf1, 0x7d,
  0x60, 0xff, 0xfb, 0xf4, 0x25, 0x30, 0x38, 0x0b, 0xe0, 0x3e, 0x44, 0x0f,
  0x7f, 0xff, 0xfe, 0xf7, 0x01, 0x9d, 0x38, 0xdf, 0x7f, 0xfc, 0x62, 0x58,
  0x34, 0x73, 0xe0, 0x3e, 0x45, 0x8f, 0x77, 0xff, 0xbf, 0xf3, 0xc1, 0x9f,
  0x0e, 0xfb, 0xdf, 0x78, 0x48, 0x80, 0x20, 0x31, 0xf0, 0x3e, 0x05, 0x07,
  0x7f, 0x76, 0xff, 0xb6, 0x05, 0xda, 0x82, 0xff, 0xfe, 0xf8, 0x41, 0x40,
  0x20, 0x01, 0xf0, 0xff, 0x06, 0x06, 0x7f, 0xff, 0xef, 0xf7, 0x07, 0xd0,
  0xc4, 0xef, 0xf7, 0xe8, 0x92, 0x00, 0x30, 0x01, 0xf0, 0xcf, 0x02, 0x0e,
  0x77, 0xee, 0xfd, 0xeb, 0x0f, 0xa0, 0x42, 0xfb, 0xff, 0xf8, 0x0c, 0x0e,
  0x10, 0x03, 0xf0, 0xc7, 0x02, 0x0c, 0xfd, 0xff, 0xff, 0xf0, 0x3d, 0xe0,
  0xf0, 0xdf, 0xdf, 0xf8, 0x08, 0x1f, 0x08, 0x03, 0xf0, 0x87, 0x83, 0xfc,
  0xff, 0xff, 0xff, 0xb4, 0x3f, 0xeb, 0xfc, 0xff, 0xff, 0xf8, 0xf0, 0x3f,
  0x88, 0x03, 0xf8, 0x00, 0x01, 0xf9, 0xdf, 0xff, 0xff, 0xfc, 0xff, 0xfb,
  0xff, 0xf7, 0x77, 0x70, 0x6c, 0x3f, 0xc6, 0x07, 0xf8, 0x00, 0x00, 0x01,
  0xff, 0x6e, 0xef, 0xec, 0xfb, 0xff, 0x77, 0xff, 0xff, 0xf8, 0x62, 0x3f,
  0xe3, 0x9f, 0xe0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
  0xff, 0xbd, 0xff, 0xf8, 0xe0, 0x3f, 0xf0, 0xff, 0xe0, 0xfc, 0x00, 0xf1,
  0xf7, 0xfb, 0xfd, 0xff, 0xfe, 0xff, 0xdf, 0xff, 0xfb, 0xf8, 0x60, 0x3f,
  0xf8, 0x1f, 0x01, 0xff, 0x03, 0xf8, 0xff, 0xaf, 0xff, 0xff, 0xef, 0xdf,
  0xff, 0xf7, 0xff, 0xb0, 0xf2, 0x3f, 0xfe, 0x00, 0x0f, 0xff, 0xff, 0xfc,
  0xff, 0xdf, 0xef, 0x7f, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xf8, 0x04, 0x1f,
  0xff, 0x80, 0x3f, 0xff, 0xff, 0xfc, 0xff, 0x7f, 0xff, 0xfd, 0xff, 0xfd,
  0xfe, 0xff, 0xff, 0xf8, 0x60, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
  0xf7, 0x6f, 0xff, 0xff, 0xbf, 0xff, 0xbf, 0xfd, 0xff, 0xf8, 0xb0, 0x07,
  0xfb, 0xff, 0xff, 0xff, 0xff, 0xfc, 0xff, 0xaf, 0xff, 0xef, 0xfb, 0xbf,
  0xff, 0xfb, 0xfb, 0xf8, 0xd2, 0x07, 0xfd, 0xff, 0xff, 0x07, 0xf8, 0x3c,
  0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xb8, 0x6d, 0x41,
  0xfe, 0xff, 0xff, 0x07, 0xf8, 0x3c, 0xff, 0x5f, 0xef, 0xff, 0xff, 0xfe,
  0xb6, 0xfd, 0xef, 0xf8, 0x00, 0x11, 0xff, 0x3f, 0xff, 0x07, 0xf8, 0x3c,
  0xff, 0xaf, 0xff, 0xfe, 0xfb, 0xf7, 0xff, 0xf9, 0xfe, 0xf8, 0x6c, 0xa0,
  0x7f, 0x9f, 0xff, 0x87, 0xf8, 0x78, 0xde, 0x7b, 0xbd, 0xdf, 0xff, 0xff,
  0xff, 0xfb, 0xff, 0xf8, 0x2a, 0x00, 0x3f, 0xcf, 0xff, 0xcf, 0xfc, 0xf1,
  0xff, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd5, 0xfb, 0xf8, 0x05, 0x40,
  0x0f, 0xe1, 0xff, 0xff, 0xff, 0xe3, 0xff, 0xaf, 0xf7, 0xff, 0xff, 0xdb,
  0xdf, 0xf5, 0xff, 0xf8, 0x28, 0x80, 0x0f, 0xf0, 0x7f, 0xff, 0xff, 0xc3,
  0xff, 0x6f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfb, 0xff, 0xf4, 0x2f, 0x00,
  0x00, 0xfe, 0x0f, 0xff, 0xff, 0x0f, 0xef, 0x5f, 0xff, 0xed, 0xef, 0xff,
  0xff, 0xf5, 0xff, 0xfc, 0x01, 0x00, 0x00, 0xff, 0x83, 0xff, 0xfe, 0x0f,
  0xff, 0x5d, 0xff, 0xff, 0xff, 0xff, 0xbf, 0xf5, 0xff, 0xfe, 0x04, 0x00,
  0x00, 0x1f, 0xe0, 0x00, 0x00, 0xff, 0xff, 0x4f, 0xef, 0xff, 0xff, 0x7f,
  0xff, 0xe5, 0xde, 0xbe, 0x08, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0xfb,
  0xff, 0x6f, 0xf7, 0xbf, 0xbf, 0xdd, 0xff, 0xf5, 0xff, 0xff, 0x00, 0x00,
  0x20, 0x00, 0x10, 0x00, 0x03, 0xff, 0xff, 0x3f, 0xff, 0xff, 0xfe, 0xdd,
  0xfe, 0xf5, 0xfb, 0xff, 0x80, 0x00, 0x15, 0x00, 0x0f, 0xfc, 0x03, 0xff,
  0xf7, 0x4f, 0xff, 0xfd, 0xee, 0x7d, 0xff, 0xe9, 0xff, 0xff, 0xc0, 0x00,
  0x22, 0x00, 0x00, 0x00, 0x03, 0xff, 0x7f, 0x6f, 0xff, 0xff, 0xff, 0x9d,
  0xdb, 0xb5, 0xef, 0xdf, 0xc0, 0x00, 0x08, 0x88, 0x00, 0x00, 0x03, 0xff,
  0xff, 0x3f, 0xe7, 0x2f, 0xff, 0x7e, 0xff, 0xf5, 0x7f, 0xff, 0xe0, 0x00,
  0x11, 0x20, 0x00, 0x01, 0x01, 0xff, 0xff, 0x4f, 0xff, 0xff, 0xfe, 0x5d,
  0xff, 0xe5, 0xff, 0xff, 0xf0, 0x00, 0x04, 0x94, 0x03, 0xff, 0x80, 0xff,
  0xff, 0x5f, 0xff, 0x5f, 0xbf, 0x5d, 0xff, 0xf5, 0xff, 0xff, 0xf0, 0x01,
  0x1a, 0x42, 0x03, 0xff, 0x80, 0xff, 0xff, 0x2f, 0xff, 0x7f, 0xbf, 0x7c,
  0xdf, 0x75, 0xff, 0xdd, 0xf8, 0x16, 0xc9, 0x29, 0x03, 0xff, 0xe0, 0x07,
  0xff, 0x6e, 0xff, 0x5f, 0x7e, 0x9d, 0xff, 0xe5, 0xfb, 0xbf, 0xfc, 0x0e,
  0x6c, 0x92, 0x03, 0xdf, 0xe0, 0x03, 0xff, 0x5e, 0xff, 0x7f, 0x3f, 0x5d,
  0xff, 0xf5, 0xdf, 0xff, 0xfe, 0x01, 0xbf, 0x89, 0x83, 0xff, 0xf8, 0x00,
  0xef, 0x2f, 0xff, 0x5c, 0xf6, 0xde, 0xbf, 0xe9, 0xfe, 0x7e, 0x7f, 0x01,
  0xdf, 0xa4, 0x83, 0xff, 0xfc, 0x00, 0x7f, 0x5f, 0xbf, 0x5c, 0xfe, 0x9d,
  0xfe, 0xf5, 0xfe, 0xf9, 0xff, 0x80, 0x2f, 0xff, 0xe3, 0xdf, 0xbf, 0x0e,
  0x7f, 0x4f, 0x9e, 0xde, 0xff, 0x7d, 0xff, 0xe5, 0xff, 0xf5, 0xf7, 0xc0,
  0x87, 0xff, 0xf3, 0xfb, 0xff, 0xcf, 0x3f, 0x6f, 0xd7, 0x63, 0xdf, 0x5c,
  0xff, 0xf3, 0xfe, 0xf5, 0xff, 0xe0, 0x13, 0xff, 0xf9, 0xfd, 0x7f, 0xcf,
  0xbf, 0x3f, 0xbf, 0x37, 0xfe, 0x9d, 0xf7, 0x95, 0xeb, 0xe1, 0xff, 0xf0,
  0x00, 0xff, 0xfc, 0xfe, 0xdf, 0xcf, 0xff, 0x4f, 0xaf, 0x5b, 0xff, 0x5d,
  0x3f, 0xa9, 0xfa, 0xf7, 0xff, 0xf8, 0x02, 0xff, 0xfc, 0xea, 0xfb, 0xe7,
  0xff, 0x5f, 0xd7, 0xa7, 0xf7, 0x5d, 0x5f, 0xd2, 0xf9, 0x6f, 0xc7, 0xfc,
  0x0d, 0x7f, 0xfe, 0xfe, 0x1f, 0xf7, 0xff, 0x2f, 0xb7, 0x7f, 0xfe, 0xbe,
  0xbd, 0xa9, 0xfb, 0xe7, 0xf7, 0x7e, 0x0f, 0x7f, 0xfe, 0x7f, 0xdf, 0xff,
  0xef, 0x5f, 0xaf, 0x57, 0xff, 0x5d, 0xbf, 0xd5, 0xfa, 0xf5, 0xdb, 0xff,
  0x00, 0x3b, 0x6b, 0x3e, 0x7f, 0xff, 0xff, 0x4f, 0xd7, 0xff, 0xff, 0x9d,
  0xdf, 0xa5, 0xf5, 0xef, 0xcb, 0xef, 0x80, 0x38, 0x03, 0xbe, 0x9f, 0xbb,
  0xff, 0x5f, 0xae, 0x00, 0x01, 0x78, 0x1f, 0xe9, 0xfa, 0x6f, 0xdb, 0xff,
  0x82, 0x9f, 0xff, 0x8f, 0x5f, 0x7f, 0xff, 0x2c, 0xa8, 0xff, 0xf8, 0x9a,
  0x9f, 0xa4, 0xf9, 0x27, 0xd7, 0xdf, 0x81, 0x0f, 0xff, 0xc6, 0xbf, 0xdf,
  0xfd, 0x4b, 0x31, 0xaa, 0xb8, 0xa5, 0x49, 0x6b, 0xfb, 0x57, 0xcb, 0xef,
  0xc0, 0x6f, 0xff, 0xe6, 0xbf, 0x6e, 0xfe, 0xd2, 0xc7, 0x55, 0x70, 0xce,
  0x94, 0xf1, 0xb9, 0xaf, 0xc4, 0x01, 0xf0, 0x2f, 0x71, 0x67, 0x57, 0x5b,
  0x79, 0x75, 0xce, 0xaa, 0xc1, 0xb5, 0x97, 0xe4, 0xf4, 0x57, 0xd2, 0x91,
  0xf0, 0x4f, 0x06, 0x64, 0x4f, 0xaa, 0xfd, 0x23, 0x9f, 0x55, 0xc3, 0x1a,
  0x70, 0xaa, 0x5b, 0x58, 0x00, 0x00, 0x00, 0x23, 0xff, 0xe2, 0xa7, 0x55,
  0xea, 0x92, 0x72, 0xaf, 0x06, 0xa6, 0x9b, 0x65, 0x56, 0xa8, 0x00, 0x00,
  0x00, 0x53, 0xff, 0xe5, 0x57, 0x54, 0xf2, 0x46, 0xf3, 0x5f, 0x0f, 0x44,
  0x61, 0x22, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x13, 0xff, 0xe0, 0xb6, 0xaa,
  0x65, 0x2c, 0xe6, 0xb0, 0x3e, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x11, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0xcd, 0x70, 0x00, 0x00,
  0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1b, 0xff, 0xf0, 0x7f, 0xff,
  0xff, 0xfc, 0xde, 0xe0, 0xff, 0xff, 0xff, 0x7e, 0x00, 0x08, 0x00, 0x00,
  0x00, 0x13, 0xff, 0x30, 0x3f, 0xff, 0xff, 0xfc, 0x9f, 0x81, 0xff, 0xff,
  0xfb, 0xfc, 0x00, 0x00, 0x01, 0x00, 0x00, 0x11, 0xff, 0x30, 0x1f, 0xfb,
  0xfe, 0xf8, 0x38, 0x07, 0xff, 0xff, 0xbf, 0xe0, 0x14, 0x67, 0x20, 0x5a,
  0x40, 0x2d, 0x00, 0xf1, 0x8f, 0xff, 0xf8, 0x00, 0x20, 0x1f, 0xfe, 0xf7,
  0xff, 0xe0, 0x84, 0x57, 0x00, 0x04, 0x80, 0x05, 0xdb, 0xe1, 0x8f, 0xff,
  0xf8, 0x00, 0x02, 0x87, 0xff, 0xff, 0xf7, 0x86, 0x20, 0x6f, 0xc2, 0x49,
  0x00, 0x33, 0xff, 0xc0, 0x63, 0xcf, 0xe0, 0x90, 0x00, 0x78, 0xff, 0xff,
  0xff, 0x8f, 0x00, 0x6d, 0x81, 0x22, 0x00, 0x13, 0xff, 0xe1, 0x63, 0xff,
  0xe0, 0x40, 0x00, 0x3f, 0x7f, 0xfd, 0xbe, 0x0f, 0x40, 0x1f, 0x01, 0x20,
  0x03, 0xa1, 0xff, 0xc0, 0x78, 0xff, 0x01, 0x00, 0x00, 0x7f, 0x9f, 0xff,
  0xfe, 0x1f, 0xc0, 0x1f, 0x0c, 0x90, 0x02, 0xd8, 0x7f, 0xe0, 0x7c, 0xff,
  0x00, 0x94, 0xaa, 0x7f, 0xef, 0xff, 0xde, 0x1f, 0xc0, 0x1f, 0xc0, 0x00,
  0x02, 0x41, 0xff, 0xc0, 0xf8, 0x7e, 0x02, 0x22, 0x44, 0x7f, 0xe7, 0xff,
  0xfe, 0x6a, 0x80, 0x18, 0xf0, 0x00, 0x13, 0xa1, 0xff, 0x00, 0x1c, 0x7c,
  0x00, 0x00, 0x11, 0x9f, 0xf7, 0xef, 0xf0, 0x37, 0xc0, 0x18, 0xf0, 0x00,
  0x00, 0x13, 0xff, 0x01, 0x18, 0x3c, 0x15, 0x2a, 0x88, 0x0f, 0xf7, 0xff,
  0xe0, 0x22, 0x81, 0x00, 0x3f, 0x07, 0xc0, 0x07, 0x80, 0x81, 0xf8, 0x3c,
  0x00, 0x91, 0x44, 0x43, 0xf7, 0xff, 0xe0, 0x23, 0x40, 0x00, 0x3e, 0xa7,
  0x89, 0x07, 0x92, 0x41, 0xf8, 0x3c, 0x02, 0x04, 0x26, 0x03, 0xf7, 0xff,
  0xe0, 0x07, 0xc1, 0x00, 0x3c, 0x00, 0xe0, 0x06, 0x49, 0x05, 0x68, 0x30,
  0x11, 0x22, 0x20, 0x88, 0xc1, 0xff, 0xc0, 0x07, 0x80, 0x84, 0x78, 0x00,
  0xf0, 0x46, 0x54, 0x06, 0xf8, 0x30, 0x00, 0x91, 0x00, 0x40, 0xf9, 0xff,
  0xc0, 0x03, 0xc0, 0x48, 0x04, 0x11, 0x20, 0x16, 0x20, 0x07, 0x98, 0x30,
  0x00, 0x49, 0x50, 0x14, 0x45, 0xf7, 0xc0, 0x11, 0xe0, 0x20, 0x00, 0x00,
  0x90, 0x13, 0x48, 0x0d, 0xe8, 0x30, 0x00, 0x00, 0x05, 0x2a, 0x19, 0xff,
  0xc4, 0x05, 0xf0, 0x00, 0x50, 0x81, 0x24, 0x10, 0x00, 0x05, 0xe0, 0x30,
  0x00, 0xa5, 0x40, 0x91, 0x05, 0xff, 0xc0, 0x02, 0xfe, 0x02, 0x04, 0x08,
  0x03, 0xa8, 0x00, 0x01, 0x60, 0x30, 0x00, 0x12, 0x22, 0x04, 0x59, 0xff,
  0xc2, 0x09, 0xfa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x91, 0xa0, 0x30,
  0x00, 0x40, 0x8d, 0x22, 0x41, 0xff, 0xc0, 0x07, 0x80, 0x70, 0x00, 0x00,
  0x00, 0x01, 0xff, 0x7f, 0x82, 0x30, 0x00, 0x24, 0x40, 0x91, 0x55, 0xff,
  0xe0, 0x03, 0x40, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x3f, 0x00, 0x30,
  0x00, 0x12, 0x24, 0x49, 0x89, 0xf7, 0xe1, 0x00, 0x00, 0x1f, 0xff, 0xff,
  0xff, 0xfe, 0x03, 0xf9, 0x02, 0x30, 0x00, 0x09, 0x30, 0x88, 0x25, 0xff,
  0xe0, 0x80, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf7, 0x03, 0xf6, 0x02, 0x30,
  0x00, 0x01, 0x02, 0x41, 0x11, 0xff, 0xe0, 0x18, 0x02, 0x07, 0x6c, 0x00,
  0xbf, 0x58, 0x06, 0x10, 0x10, 0x30, 0x00, 0x0a, 0x40, 0x12, 0x49, 0xdf,
  0xe0, 0x48, 0x00, 0x83, 0xfc, 0x80, 0x0f, 0xf0, 0x40, 0x00, 0x68, 0x30,
  0x00, 0x05, 0x12, 0xe0, 0x91, 0xf7, 0xe0, 0x00, 0x00, 0x16, 0xb4, 0x24,
  0x0e, 0xe8, 0x00, 0x01, 0x00, 0x3c, 0x00, 0x04, 0x48, 0x89, 0x01, 0xff,
  0xf8, 0x36, 0x00, 0x1d, 0xf0, 0x00, 0x83, 0xb0, 0x10, 0x01, 0xc8, 0x7c,
  0x00, 0x19, 0x01, 0x40, 0x57, 0xff, 0xf0, 0x08, 0x00, 0x00, 0x54, 0x80,
  0x01, 0x51, 0x00, 0x01, 0x80, 0x3c, 0x00, 0x14, 0x14, 0xa4, 0x87, 0xff,
  0xf8, 0x03, 0xd0, 0x00, 0x00, 0x12, 0x40, 0x00, 0x00, 0x3e, 0x00, 0xfc,
  0x40, 0x09, 0x40, 0x11, 0x47, 0xff, 0xf1, 0x08, 0x28, 0x00, 0x04, 0x00,
  0x00, 0x00, 0x00, 0xfe, 0x81, 0xfe, 0x00, 0x00, 0x16, 0x02, 0x07, 0xf7,
  0xfa, 0x00, 0x0d, 0x55, 0x09, 0x00, 0x01, 0xff, 0xff, 0xe0, 0x01, 0xff,
  0x00, 0x04, 0x00, 0xa0, 0x9f, 0xef, 0xf8, 0x40, 0xd2, 0xa8, 0x80, 0x00,
  0x0f, 0xff, 0xff, 0x90, 0x08, 0xff, 0x90, 0x08, 0x94, 0x14, 0x1f, 0xff,
  0xff, 0x80, 0x00, 0x04, 0x54, 0xff, 0xff, 0xfe, 0xfd, 0x00, 0x17, 0xff,
  0x80, 0x00, 0x21, 0x22, 0xff, 0xff, 0xff, 0x88, 0x01, 0xdb, 0x2a, 0xff,
  0xff, 0xff, 0x02, 0x00, 0x1f, 0xff, 0x80, 0x01, 0x00, 0x44, 0xff, 0xff,
  0xff, 0xe2, 0x00, 0x00, 0x44, 0xff, 0xda, 0xa0, 0x00, 0x01, 0x7f, 0xfe,
  0x80, 0x01, 0x25, 0x01, 0xbf, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x52, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xc0, 0x00, 0x01, 0x91, 0x1f, 0xff,
  0xf7, 0xf6, 0x40, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x16, 0x7f, 0xfe,
  0x90, 0x00, 0x48, 0x02, 0x3f, 0xff, 0xff, 0xff, 0x48, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x20, 0x04, 0x80, 0x88, 0x87, 0xdf,
  0xff, 0xff, 0x22, 0x42, 0x00, 0x00, 0x00, 0x00, 0x94, 0x8f, 0xff, 0xfc,
  0x94, 0x00, 0x04, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xfe, 0x28, 0x00, 0x20,
  0x10, 0x08, 0x5f, 0xff, 0xff, 0xbf, 0xc3, 0x00, 0x00, 0x3a, 0x7f, 0xff,
  0xff, 0xdf, 0xfe, 0x04, 0xdb, 0x15, 0x4a, 0xa5, 0x1f, 0xff, 0xff, 0xff,
  0xa5, 0xc0, 0x00, 0xd0, 0x3f, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xe4, 0x82,
  0x74, 0x7f, 0xff, 0xfe, 0xff, 0xff, 0xfb, 0xf0, 0x01, 0x4b, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xfc, 0xca, 0xaa, 0x67, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdd, 0x6f, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xfe, 0x01,
  0xc1, 0xfc, 0x1f, 0x83, 0xf0, 0x41, 0xc0, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xfd, 0xfe, 0xbe, 0xfb, 0xef, 0x7d, 0xef, 0xbe,
  0xbf, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x02,
  0xa2, 0xfa, 0x2f, 0x45, 0xe8, 0xa2, 0xa0, 0x0b, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xfb, 0xf3, 0x02, 0xa3, 0x3a, 0x2c, 0xc6, 0x68, 0xa2,
  0xa0, 0x0b, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x02,
  0xa3, 0xda, 0x2b, 0xc7, 0xa8, 0xa2, 0xa0, 0x0b, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xe8, 0xfe, 0xa0, 0x5a, 0x2a, 0x38, 0xa8, 0xa2,
  0xa3, 0xfb, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xe8, 0x81,
  0xa0, 0x62, 0x2a, 0x28, 0xa8, 0xa2, 0xa2, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xfb, 0xff, 0xfd, 0xff, 0xe8, 0xf7, 0xa0, 0x7a, 0x2a, 0x38, 0xa8, 0xe2,
  0xa3, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xef, 0x17,
  0xa3, 0x8a, 0x2a, 0x00, 0xa8, 0x1e, 0xa0, 0x5f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xf3, 0x19, 0xa2, 0x8a, 0x2a, 0x00, 0xa8, 0x11,
  0xa0, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xfd, 0x1e,
  0xa2, 0x8e, 0x2a, 0x00, 0xa8, 0x1e, 0xa0, 0x5f, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xfd, 0xe2, 0xa2, 0xf0, 0x2a, 0x38, 0xa8, 0xe2,
  0xa3, 0xdf, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x22,
  0xa2, 0x30, 0x2a, 0x28, 0xa8, 0xa2, 0xa3, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xef, 0xef, 0xe2, 0xa2, 0xd0, 0x2a, 0x28, 0xa8, 0xa2,
  0xa3, 0xfb, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xff, 0xe8, 0x1e,
  0xa2, 0xde, 0x2a, 0x28, 0xa8, 0xa2, 0xa0, 0x0b, 0xff, 0xff, 0xff, 0xf7,
  0xff, 0xff, 0xff, 0xff, 0xe8, 0x19, 0xa2, 0xe6, 0x2a, 0x28, 0xa8, 0xa2,
  0xa0, 0x0b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe8, 0x17,
  0xa2, 0xfa, 0x2a, 0x28, 0xa8, 0xa2, 0xa0, 0x0b, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xf7, 0xff, 0xff, 0xef, 0xf7, 0xbe, 0xfb, 0xeb, 0xef, 0xaf, 0xbe,
  0xbf, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x0f,
  0xc1, 0xfc, 0x1c, 0x10, 0x70, 0x41, 0xc0, 0x07, 0xff, 0xff, 0xff, 0xff,
  0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff,
  0xff, 0xff, 0xff, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xfe, 0x04, 0x08, 0x10, 0x20, 0x70, 0x21, 0xaf, 0x77, 0xa0, 0x5e, 0xc1,
  0x03, 0x81, 0x04, 0x08, 0x10, 0x7f, 0xfe, 0xf5, 0xeb, 0xf7, 0xef, 0xf7,
  0xad, 0xaf, 0x77, 0xaf, 0xde, 0xf7, 0x7b, 0xbf, 0xdd, 0xeb, 0xdd, 0xff,
  0xfe, 0x04, 0x18, 0x10, 0x20, 0x70, 0x2d, 0xa0, 0x70, 0x60, 0x40, 0xf7,
  0x7b, 0x81, 0xdc, 0x08, 0x3d, 0xff, 0xfe, 0xfd, 0xeb, 0xff, 0xbf, 0x77,
  0xad, 0xbf, 0x77, 0xaf, 0xfe, 0xf7, 0x7b, 0xfd, 0xdd, 0xeb, 0xdd, 0xff,
  0xfe, 0xfd, 0xe8, 0x10, 0x20, 0x77, 0xac, 0x20, 0x77, 0xa0, 0x40, 0xf7,
  0x03, 0x81, 0xdd, 0xeb, 0xdd, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

class gameManager : public Actor::Actor {
public:
  bool startScreen = true;
  bool gameOver = false;
  int highScore = 0;

  int initialTicksPerMove = 8;
  int ticksPerMove = initialTicksPerMove;
  int ticksSinceLastMove = 0;

  int xDirection = 1;
  int yDirection = 0;
  int newXDirection = 1;
  int newYDirection = 0;

  static const int sectionSize = 8;
  static const int sectionSeparation = 3;

  static const int screenBorder = 2;
  static const int blockSize = sectionSize + sectionSeparation;
  static const int rows = DISPLAY_HEIGHT / blockSize;
  static const int columns = DISPLAY_WIDTH / blockSize;
  static const int numBlocks = rows * columns;
  static const int minBlockX = screenBorder;
  static const int minBlockY = screenBorder;
  static const int maxBlockX = screenBorder + blockSize * (columns - 1);
  static const int maxBlockY = screenBorder + blockSize * (rows - 1);

  int sectionX[numBlocks + 1] = {0};
  int sectionY[numBlocks + 1] = {0};

  int foodX = 0;
  int foodY = 0;

  int length = 2;

  const char *getName() { return "gameManager"; };

  void drawHead() {
    engine.display.fillRectangle(sectionX[length - 1], sectionY[length - 1], sectionSize, sectionSize);
  };
  
  void drawTail() {
    engine.display.fillRectangle(sectionX[0], sectionY[0], sectionSize, sectionSize);
  };  

  void eraseTail() {
    engine.display.fillRectangle(sectionX[0], sectionY[0], sectionSize, sectionSize, WHITE);
  };

  void shiftSectionLists() {
    for (int i = 0; i < length; i++) {
      sectionX[i] = sectionX[i + 1];
      sectionY[i] = sectionY[i + 1];
    }
  }

  void moveSnake() {
    // add new head
    sectionX[length] = sectionX[length - 1] + blockSize * xDirection;
    sectionY[length] = sectionY[length - 1] + blockSize * yDirection;
        
    // handle wrap around
    if (sectionX[length] > maxBlockX) {
      sectionX[length] = screenBorder;
    } else if (sectionX[length] < minBlockX) {
      sectionX[length] = maxBlockX;
    }
    
    if (sectionY[length] > maxBlockY) {
      sectionY[length] = screenBorder;
    } else if (sectionY[length] < minBlockY) {
      sectionY[length] = maxBlockY;
    }

    // handle collision
    for (int i = 0; i < length; i++) {
      if (sectionX[length] == sectionX[i] && sectionY[length] == sectionY[i]) {
        gameOver = true;
        highScore = fmax(length, highScore);
        unsubscribe(&engine.clock);
        engine.display.clear();
        char gameOver[] = "GAME OVER";
        engine.display.drawText(5, 5, gameOver);
        char msg[32];
        snprintf(msg, sizeof(msg), "Score: %d", (uint16_t)length);
        engine.display.drawText(5, 15, msg);
        snprintf(msg, sizeof(msg), "High Score: %d", highScore);
        engine.display.drawText(5, 25, msg);
        char pressLeftButton[] = "Press left button";
        char toTryAgain[] = "to try again.";
        engine.display.drawText(5, 45, pressLeftButton);
        engine.display.drawText(5, 55, toTryAgain);
        engine.display.update();
        return;
      }
    }

    // handle food
    if (sectionX[length] == foodX && sectionY[length] == foodY) {
      dropFood();
      length++;
      ticksPerMove = fmax(2, ticksPerMove - 1);
    } else {
      eraseTail();
      shiftSectionLists();
    }

    drawHead();
  }

  void dropFood() {
    int candidateFoodX, candidateFoodY;
    bool retry;

    while (true) {
      candidateFoodX = screenBorder + random(columns) * blockSize;
      candidateFoodY = screenBorder + random(rows) * blockSize;

      retry = false;
      for (int i = 0; i <= length; i++) {
        if (candidateFoodX == sectionX[i] && candidateFoodY == sectionY[i]) {
          retry = true;
          break;
        }
      }

      if (retry) {
        continue;
      }

      break;
    }

    foodX = candidateFoodX;
    foodY = candidateFoodY;

    engine.display.drawRectangle(foodX, foodY, sectionSize, sectionSize);
  }

  void startGame() {
    gameOver = false;
    ticksPerMove = initialTicksPerMove;
    ticksSinceLastMove = 0;
    xDirection = 1;
    yDirection = 0;

    // set up initial head and tail
    sectionX[0] = screenBorder; // tail
    sectionY[0] = screenBorder;
    sectionX[1] = screenBorder + blockSize; // head
    sectionY[1] = screenBorder;

    length = 2;

    drawHead();
    drawTail();
    dropFood();
    engine.display.update();
    subscribe(&engine.clock);
  };

  void initialize() {
    gameOver = true;
    engine.display.drawBitmap(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, (uint8_t *)splashScreen);
    engine.display.update();
    subscribe(&engine.input);
  }

  void handle(::Actor::Message *message) {
    switch (message->signal) {
    case Kywy::Events::D_PAD_LEFT_PRESSED:
      if (xDirection != 1) {
        newXDirection = -1;
        newYDirection = 0;
      }
      break;
    case Kywy::Events::D_PAD_RIGHT_PRESSED:
      if (xDirection != -1) {
        newXDirection = 1;
        newYDirection = 0;
      }
      break;
    case Kywy::Events::D_PAD_UP_PRESSED:
      if (yDirection != 1) {
        newXDirection = 0;
        newYDirection = -1;
      }
      break;
    case Kywy::Events::D_PAD_DOWN_PRESSED:
      if (yDirection != -1) {
        newXDirection = 0;
        newYDirection = 1;
      }
      break;
    case Kywy::Events::TICK:
      ticksSinceLastMove++;

      if (ticksSinceLastMove >= ticksPerMove) {
        xDirection = newXDirection;
        yDirection = newYDirection;
        moveSnake();
        ticksSinceLastMove = 0;
        engine.display.update();
      }
      break;
    case Kywy::Events::BUTTON_LEFT_PRESSED:
      if (!gameOver) {
        break;
      }      
      
      engine.display.clear();
      startGame();
      subscribe(&engine.clock);
    case Kywy::Events::INPUT:
      startScreen = false;
      engine.display.clear();
      startGame();
      subscribe(&engine.clock);
      break;
    }
  }

} gameManager;

void setup() {
  engine.start();
  gameManager.start();
}

void loop() {
  delay(1000);
}
