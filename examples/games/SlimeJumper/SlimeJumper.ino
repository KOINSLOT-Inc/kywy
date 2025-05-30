// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

const uint8_t slimeJumperSplashScreenBMP[] = {
  0x55, 0x69, 0x3f, 0xfe, 0x35, 0x60, 0xa9, 0x7a, 0xf5, 0x7a, 0xe3, 0x53,
  0xff, 0xff, 0x95, 0x49, 0x26, 0xa5, 0xa4, 0xa6, 0x7f, 0xff, 0x34, 0x8d,
  0x56, 0x8d, 0xab, 0x8d, 0x54, 0xa7, 0xff, 0xff, 0xca, 0xb2, 0x92, 0x52,
  0x52, 0x50, 0x7f, 0xff, 0x2b, 0x6d, 0xa9, 0x4a, 0xd5, 0x53, 0xaf, 0x67,
  0xff, 0xff, 0xc5, 0x29, 0x55, 0x29, 0xf5, 0x20, 0x7f, 0xff, 0x14, 0x90,
  0x54, 0xb5, 0x2e, 0xbc, 0xda, 0xc7, 0xff, 0xff, 0xe5, 0x4a, 0x4a, 0xd4,
  0x5a, 0x86, 0x3f, 0xff, 0x02, 0xa6, 0xab, 0x4a, 0x93, 0x6b, 0x12, 0x4f,
  0xff, 0xff, 0xc6, 0x95, 0xa5, 0x4b, 0xa4, 0x0f, 0xbf, 0xfe, 0x01, 0x6b,
  0x54, 0xa5, 0x69, 0x41, 0xad, 0x4f, 0xff, 0xff, 0xe4, 0xc9, 0x34, 0xa9,
  0xf2, 0x3f, 0xff, 0x4c, 0x78, 0xb4, 0xad, 0x7a, 0xae, 0xb0, 0xd2, 0xcf,
  0xff, 0xff, 0xe3, 0x55, 0x2a, 0xb5, 0x24, 0xff, 0xff, 0xb7, 0xfc, 0x4b,
  0x53, 0x8d, 0x50, 0xd9, 0x5a, 0x4f, 0xff, 0xff, 0xc0, 0x6a, 0xcb, 0x4a,
  0xd8, 0xff, 0xfe, 0x6f, 0xfe, 0x75, 0x7d, 0x52, 0xb0, 0x57, 0xab, 0x4f,
  0xff, 0xff, 0x80, 0x26, 0xb4, 0xa5, 0x61, 0xfe, 0xad, 0xb0, 0xbe, 0x0e,
  0x96, 0xbd, 0xa8, 0x6a, 0x6d, 0xcf, 0xff, 0xff, 0x8e, 0x1d, 0x5b, 0x5a,
  0xb3, 0xeb, 0x56, 0xdf, 0x4f, 0x0b, 0xbb, 0x68, 0x1d, 0xb6, 0xd0, 0x0f,
  0xff, 0xfd, 0x9f, 0x80, 0x32, 0x63, 0xd6, 0x95, 0xb5, 0x4a, 0xe9, 0xc4,
  0xf6, 0xd0, 0x0f, 0x4d, 0xb0, 0x07, 0xff, 0xf6, 0x7f, 0xc0, 0x0d, 0x5a,
  0x50, 0x00, 0x00, 0x00, 0x00, 0x07, 0x5e, 0xd0, 0xe2, 0xd4, 0xa3, 0xc7,
  0xff, 0xeb, 0xd7, 0x8f, 0x05, 0xad, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x1d,
  0xbb, 0x60, 0x32, 0xbb, 0x47, 0xf7, 0xff, 0xb6, 0xa9, 0x3f, 0xe2, 0xb5,
  0x9f, 0xbd, 0xbb, 0xfd, 0xef, 0xea, 0xd3, 0x56, 0x99, 0x6e, 0xcf, 0xff,
  0xbb, 0x5a, 0xda, 0xfb, 0xf1, 0x55, 0x59, 0x56, 0xd5, 0x26, 0xb5, 0x57,
  0x63, 0xb3, 0x48, 0xb1, 0x0f, 0xff, 0xfe, 0xd6, 0xdd, 0xcd, 0x79, 0xa2,
  0xa6, 0xa9, 0x6a, 0xdb, 0x16, 0xbb, 0x63, 0xed, 0x4c, 0x36, 0x1f, 0xff,
  0xf5, 0xb6, 0xa6, 0x56, 0xbc, 0x39, 0x79, 0x56, 0xad, 0x55, 0xea, 0xca,
  0xe3, 0x5a, 0x8e, 0x58, 0x7f, 0x76, 0xab, 0x6b, 0x76, 0xeb, 0x4c, 0x04,
  0xd6, 0xab, 0x5b, 0x2c, 0xb5, 0x6d, 0xc1, 0xb7, 0x8e, 0x28, 0xfe, 0xdb,
  0x5d, 0xb5, 0xbb, 0x5a, 0xea, 0x0b, 0xcb, 0x75, 0xb5, 0xdb, 0x4d, 0x5b,
  0x88, 0xed, 0xcf, 0x13, 0xfb, 0xb6, 0xea, 0xae, 0xaa, 0xb5, 0x5f, 0xc9,
  0xa5, 0xad, 0x8e, 0xd5, 0x6a, 0xac, 0x1c, 0x1d, 0x4d, 0x90, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x7a, 0x56, 0x01, 0x6a, 0xb5, 0xd6,
  0x1e, 0x1e, 0xcf, 0xc8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a,
  0xb7, 0xb4, 0x00, 0xb7, 0x6e, 0xbb, 0x08, 0x7b, 0xdf, 0xce, 0xd5, 0x52,
  0xae, 0xed, 0xb7, 0x6e, 0xd9, 0xb5, 0xdd, 0x68, 0xfc, 0x6d, 0x99, 0xad,
  0xc1, 0xf6, 0x9d, 0xe3, 0xff, 0xff, 0xf7, 0xfb, 0x7a, 0xdb, 0xb7, 0xda,
  0xea, 0xb1, 0xfe, 0x35, 0x6d, 0x56, 0xe3, 0xdd, 0x9f, 0x71, 0x6d, 0xb5,
  0x5a, 0xdf, 0xb5, 0x64, 0xd4, 0xa3, 0x27, 0x43, 0xff, 0x2b, 0xb6, 0xeb,
  0xb3, 0x6b, 0x1b, 0xf0, 0xfe, 0xee, 0xdd, 0x6d, 0xae, 0xb7, 0x6f, 0x7a,
  0xb5, 0xa7, 0xff, 0x1d, 0x6b, 0x5d, 0xe3, 0xbc, 0x3f, 0xd8, 0xdb, 0x76,
  0xeb, 0xb9, 0xdb, 0x5a, 0x95, 0xad, 0xfa, 0xe7, 0xff, 0x9a, 0xdd, 0xaa,
  0xb3, 0x6e, 0x3f, 0xbc, 0x6d, 0xbb, 0x55, 0xb9, 0xf9, 0xad, 0xba, 0xb5,
  0xad, 0x27, 0xff, 0x9b, 0x6a, 0xdd, 0xd3, 0xec, 0x7a, 0xec, 0x3b, 0xdd,
  0xb9, 0x71, 0xad, 0xda, 0xd7, 0x6a, 0x76, 0xe7, 0xff, 0x9e, 0xb6, 0xad,
  0xbe, 0xd8, 0x77, 0xfe, 0x1e, 0xee, 0xde, 0xe0, 0xd6, 0xb5, 0xfd, 0xdb,
  0xeb, 0x07, 0xff, 0x81, 0xdb, 0x6e, 0xed, 0xb8, 0xff, 0xb7, 0x0f, 0x75,
  0xb5, 0xc4, 0x7b, 0x6e, 0x80, 0x6c, 0xda, 0x07, 0xff, 0x80, 0x6d, 0xd7,
  0x77, 0xe0, 0xf7, 0xff, 0x83, 0xdb, 0xda, 0x0e, 0x3d, 0x95, 0x00, 0x3b,
  0xb4, 0x67, 0xfb, 0x3c, 0x76, 0xdb, 0xbb, 0x61, 0xfd, 0xf7, 0xc0, 0xee,
  0xef, 0x86, 0x06, 0xea, 0x1e, 0x1d, 0xb8, 0xff, 0xea, 0x7e, 0x37, 0x7d,
  0xde, 0xc7, 0xef, 0xba, 0xe1, 0xff, 0x79, 0xc4, 0x3b, 0x74, 0x7f, 0x8a,
  0x31, 0xbf, 0xff, 0xff, 0x0d, 0xd6, 0xed, 0x87, 0xbb, 0xef, 0xf0, 0x5b,
  0xb6, 0xe0, 0xf7, 0x58, 0xff, 0xcd, 0xd3, 0x48, 0x95, 0xb7, 0x83, 0xbb,
  0xf7, 0x0f, 0xff, 0x7e, 0xdc, 0x3d, 0xdb, 0x71, 0xea, 0xd1, 0xff, 0xe5,
  0x67, 0xff, 0x7b, 0x6d, 0xc3, 0x7f, 0x7c, 0x1e, 0xd6, 0xdb, 0xff, 0x1f,
  0xbd, 0xd9, 0xbd, 0xb3, 0xff, 0xe6, 0xb8, 0x00, 0x00, 0x00, 0x21, 0xed,
  0xd8, 0x3f, 0x7b, 0xb7, 0x7b, 0x8f, 0xf6, 0xfb, 0xdf, 0xa3, 0xff, 0xe3,
  0x90, 0x00, 0x00, 0x00, 0x03, 0xb7, 0xf0, 0xfb, 0xbf, 0x7b, 0xaf, 0xc3,
  0x6d, 0x5f, 0xb9, 0x67, 0xff, 0xf9, 0x54, 0x84, 0xab, 0x55, 0x2f, 0xfb,
  0x61, 0xfe, 0xf5, 0xdd, 0xfd, 0xe1, 0xfa, 0xeb, 0xf7, 0xe7, 0xff, 0xfc,
  0xaf, 0xff, 0xff, 0xff, 0xfe, 0x5f, 0xc3, 0xef, 0xee, 0xee, 0xbf, 0xf0,
  0xf6, 0xdf, 0x6d, 0x67, 0xff, 0xfe, 0x75, 0x7e, 0xd9, 0x6b, 0x75, 0xee,
  0x87, 0xfe, 0xdf, 0xf7, 0x6d, 0xb8, 0x7e, 0xda, 0xcb, 0xa7, 0xff, 0xff,
  0xd6, 0xcb, 0x6f, 0xb6, 0xef, 0xdd, 0x0f, 0xbd, 0xbb, 0x6d, 0xde, 0xfc,
  0x6d, 0xbd, 0xbe, 0x07, 0xff, 0xe9, 0xbd, 0xfd, 0xb6, 0xff, 0x7a, 0xfe,
  0x1f, 0xef, 0xf6, 0xfe, 0xf7, 0x5e, 0x3f, 0x6f, 0x78, 0x07, 0xff, 0x56,
  0x6d, 0x6f, 0x7b, 0x55, 0xdf, 0x76, 0x3f, 0xf6, 0xdf, 0xb7, 0xbb, 0xb7,
  0x1b, 0xf5, 0xb0, 0xe3, 0xf5, 0xaa, 0xd8, 0xd6, 0xdd, 0xee, 0xfb, 0xbc,
  0x7e, 0xdd, 0xfb, 0xdb, 0x6d, 0xef, 0x0e, 0xdb, 0xc1, 0xff, 0xf5, 0x75,
  0xb8, 0xfb, 0x66, 0xbd, 0xb6, 0xd8, 0xf7, 0xff, 0xb6, 0xff, 0xff, 0x77,
  0x8f, 0xbe, 0xc7, 0xff, 0xdb, 0x4a, 0x70, 0xef, 0xbf, 0xdb, 0x6f, 0xf1,
  0xfe, 0xed, 0xff, 0xb6, 0xdb, 0xb9, 0xc7, 0x6d, 0x87, 0xff, 0xad, 0xbd,
  0xc2, 0x1d, 0xf6, 0xdf, 0xdd, 0xf1, 0xdf, 0xff, 0x6d, 0xfd, 0xff, 0xee,
  0xe3, 0xf7, 0x0f, 0xfe, 0xd5, 0x54, 0x87, 0x1a, 0xb6, 0xec, 0xbe, 0xa3,
  0xf7, 0x3b, 0xff, 0x6f, 0xb6, 0x7d, 0xe3, 0xbb, 0x1f, 0x6b, 0x7b, 0xbb,
  0xe2, 0x3f, 0x6f, 0x77, 0x6f, 0xe3, 0x78, 0x07, 0xb7, 0xf7, 0xd8, 0x0e,
  0xb1, 0xfe, 0x30, 0x00, 0x00, 0x00, 0xd8, 0xf7, 0xfb, 0xdf, 0xf5, 0xc7,
  0xb0, 0x03, 0xfa, 0xdd, 0xe0, 0x07, 0xf1, 0xee, 0x00, 0x00, 0x00, 0x00,
  0xf8, 0xdb, 0xb6, 0xcb, 0x7f, 0x47, 0xe0, 0x00, 0xdf, 0xff, 0x60, 0x03,
  0x58, 0xff, 0x84, 0xaa, 0x95, 0x54, 0x68, 0xfe, 0xef, 0xed, 0xaf, 0xce,
  0xc4, 0x00, 0xff, 0x6f, 0xc0, 0x01, 0xb8, 0xf7, 0xff, 0xff, 0xff, 0xff,
  0xf9, 0xec, 0xdd, 0xf7, 0xfb, 0x8f, 0x4e, 0x40, 0x76, 0xfb, 0x9c, 0x90,
  0xec, 0xff, 0xf7, 0xfd, 0xfb, 0xfb, 0x69, 0xd8, 0xff, 0x7e, 0xdf, 0x1d,
  0x8e, 0x50, 0x7f, 0xff, 0x1c, 0x80, 0xdc, 0x6d, 0xff, 0xaf, 0x6f, 0x6d,
  0xb7, 0xf8, 0xee, 0xdb, 0xff, 0x1e, 0x8e, 0xc4, 0x3d, 0xdf, 0x1c, 0xc0,
  0x76, 0x7e, 0xdb, 0x76, 0xfd, 0xbf, 0xfe, 0xb8, 0xf7, 0xee, 0xdb, 0x3b,
  0x80, 0xe0, 0x3f, 0xfb, 0x00, 0xc4, 0x6e, 0x3f, 0xf7, 0xf7, 0xb7, 0xf6,
  0x57, 0xd8, 0x7d, 0xbd, 0xdf, 0x37, 0x00, 0xe1, 0x3f, 0xde, 0x01, 0xe0,
  0x7e, 0x3b, 0x7e, 0xbb, 0x6e, 0xdf, 0xef, 0xf8, 0x6f, 0x7f, 0xfe, 0x3d,
  0x0f, 0xf0, 0x3b, 0x7e, 0x03, 0xe0, 0x66, 0x3f, 0xdb, 0xdd, 0xfd, 0xed,
  0xdb, 0x50, 0x77, 0xed, 0xb6, 0x37, 0x07, 0xfc, 0x3f, 0xee, 0x0f, 0xfa,
  0x7b, 0x1e, 0xff, 0xff, 0xb7, 0xbf, 0xdf, 0xf0, 0x3e, 0xf7, 0xfc, 0x7b,
  0x21, 0xf0, 0x3a, 0xf7, 0x03, 0xe0, 0x6f, 0x1f, 0xdb, 0x6d, 0xdf, 0xf6,
  0xed, 0x80, 0x0f, 0xbf, 0x78, 0xed, 0x00, 0xc1, 0x3f, 0xbf, 0x10, 0xc0,
  0x77, 0x8d, 0xff, 0xbb, 0x62, 0xdf, 0x77, 0x01, 0x07, 0xff, 0xe0, 0xf7,
  0x00, 0x40, 0x3f, 0xff, 0x00, 0xc4, 0x6f, 0xc7, 0xfd, 0xff, 0xc1, 0xef,
  0xdf, 0x80, 0x27, 0xed, 0xe1, 0xfb, 0x84, 0x40, 0x39, 0xe7, 0x00, 0x80,
  0x75, 0xc1, 0xb7, 0xdb, 0x91, 0xff, 0xcb, 0xe4, 0x1f, 0x7f, 0x83, 0xdd,
  0x80, 0x49, 0x78, 0xc7, 0x24, 0x80, 0xef, 0xe0, 0xff, 0xff, 0x91, 0xf6,
  0xed, 0xf0, 0xbd, 0xfe, 0x07, 0xd6, 0xc0, 0x40, 0x78, 0x07, 0x80, 0x08,
  0xdb, 0xf8, 0x3f, 0x77, 0xc1, 0xdf, 0xf7, 0xf8, 0x7f, 0xe8, 0x1f, 0x6f,
  0xc9, 0x00, 0xfc, 0x0f, 0xc0, 0x01, 0xf5, 0x7c, 0x0f, 0xff, 0xc3, 0xfb,
  0xbe, 0xf8, 0xfd, 0x70, 0x7e, 0xfa, 0xe0, 0x09, 0xfe, 0x3f, 0xc4, 0x93,
  0xbe, 0xbf, 0x07, 0xfd, 0xff, 0xfe, 0xfb, 0xf8, 0xfb, 0xe0, 0xf6, 0xad,
  0xb0, 0x03, 0xff, 0xff, 0xe0, 0x03, 0x56, 0xcf, 0x83, 0x6f, 0xfe, 0xdf,
  0xeb, 0xf8, 0xff, 0xc3, 0xdb, 0x77, 0x78, 0x07, 0xad, 0x56, 0xf8, 0x0f,
  0xeb, 0x72, 0xe1, 0xff, 0xff, 0xf7, 0x2f, 0x7c, 0xeb, 0x87, 0xf7, 0x6e,
  0xff, 0x3f, 0x7e, 0xeb, 0x5f, 0xfa, 0xdd, 0xdf, 0xf1, 0xfd, 0xbe, 0xfe,
  0xbf, 0xec, 0xdf, 0x8e, 0xad, 0xb7, 0x6a, 0xfd, 0xdb, 0xb5, 0xb5, 0x77,
  0x77, 0xbb, 0xb8, 0xdf, 0xf7, 0xdf, 0xff, 0xfc, 0xf5, 0x1d, 0xfb, 0xda,
  0xf7, 0xee, 0xef, 0x6a, 0xea, 0xad, 0xad, 0x6d, 0xdc, 0x7f, 0xff, 0xf7,
  0x2d, 0xff, 0xbf, 0x1b, 0x6f, 0x7d, 0xdd, 0xf7, 0xb6, 0xd5, 0x77, 0x5b,
  0x7b, 0x7f, 0xdc, 0x7b, 0x3e, 0xf8, 0x3f, 0xbf, 0xee, 0x37, 0xbb, 0xaf,
  0x7b, 0xed, 0xed, 0xfe, 0xdd, 0xb6, 0xb7, 0xb6, 0xd6, 0x3f, 0x1f, 0xe0,
  0x0f, 0xf7, 0xfe, 0x35, 0xd7, 0x77, 0xb6, 0xbb, 0x5e, 0xb7, 0x7b, 0x6f,
  0xda, 0xef, 0xb6, 0x3f, 0x1f, 0xc0, 0x1f, 0xff, 0x7e, 0x6b, 0xaa, 0xdb,
  0x6f, 0xee, 0xf7, 0xdb, 0xb6, 0xf5, 0x7d, 0x9b, 0x63, 0x3f, 0x3d, 0x83,
  0x06, 0xdf, 0xf6, 0x66, 0xfd, 0xb6, 0xd5, 0xfd, 0xb5, 0x6d, 0x6f, 0xbe,
  0xd7, 0x6d, 0xb3, 0x3b, 0xff, 0x07, 0x87, 0xf8, 0xfe, 0x65, 0x56, 0xed,
  0x7e, 0xd7, 0x6a, 0xed, 0xf5, 0xab, 0x6b, 0xed, 0xb3, 0x1f, 0xff, 0x1f,
  0xc3, 0xfa, 0x7c, 0x66, 0xdb, 0x7b, 0xab, 0x6d, 0xf7, 0x76, 0xbe, 0xde,
  0xad, 0x5a, 0xc3, 0x1f, 0xf6, 0x3f, 0xc1, 0xf8, 0x74, 0x63, 0x6d, 0xaf,
  0x76, 0xde, 0xbd, 0xbf, 0xab, 0x6b, 0x76, 0xee, 0xe3, 0x1f, 0x7c, 0x7e,
  0xe1, 0xdc, 0xfc, 0x61, 0xb6, 0xd5, 0xae, 0xdb, 0xdb, 0xd5, 0xf7, 0xbd,
  0xbb, 0x6d, 0x4b, 0x3f, 0xfc, 0xfd, 0xe1, 0xff, 0xfe, 0x68, 0xdb, 0x7b,
  0x77, 0x76, 0xdd, 0xbb, 0x5d, 0xdb, 0x6d, 0xb7, 0x03, 0x3b, 0xe8, 0xec,
  0xf0, 0xff, 0xfe, 0x24, 0x36, 0xdd, 0xab, 0x6d, 0xb6, 0xee, 0xed, 0x6d,
  0xb6, 0xda, 0x56, 0x3f, 0x79, 0xd3, 0x78, 0xef, 0xfe, 0x32, 0x1b, 0xad,
  0xdd, 0xbf, 0x7b, 0x77, 0x77, 0xb6, 0xdb, 0x68, 0x26, 0x3f, 0xf9, 0xff,
  0xbc, 0xff, 0xdb, 0x31, 0x42, 0xf6, 0xed, 0xd5, 0xdf, 0xbb, 0xba, 0xdb,
  0x6d, 0x92, 0x86, 0x7f, 0xf9, 0xd4, 0x3c, 0xfd, 0xff, 0x18, 0x84, 0x1b,
  0x6e, 0xed, 0xa9, 0xda, 0xdd, 0xbd, 0xa8, 0x04, 0x4c, 0x7b, 0x71, 0xeb,
  0xdc, 0xff, 0xff, 0x1c, 0x21, 0x00, 0xb5, 0x6e, 0xf6, 0xdd, 0xad, 0xd6,
  0x02, 0xa1, 0x3c, 0x7f, 0xc1, 0xcd, 0xbc, 0xf7, 0x6f, 0x8f, 0x14, 0xa4,
  0x0f, 0x76, 0xdf, 0x6f, 0x76, 0xd8, 0x94, 0x52, 0x38, 0xff, 0x01, 0xf2,
  0x6c, 0xff, 0xff, 0xc7, 0x48, 0x49, 0x43, 0xad, 0xb5, 0xeb, 0xb6, 0xb1,
  0x22, 0x08, 0xf1, 0xfa, 0x0f, 0xc9, 0xbc, 0x03, 0xfd, 0xe3, 0xa2, 0x12,
  0x20, 0xdb, 0x6e, 0xba, 0xdb, 0x42, 0x49, 0x45, 0xc1, 0xf8, 0x3f, 0xd6,
  0xd8, 0x00, 0xdf, 0xe0, 0xf2, 0x81, 0x28, 0x6d, 0xb7, 0x6d, 0xb7, 0x90,
  0x84, 0x2b, 0x83, 0xf0, 0xff, 0xbb, 0x58, 0xe0, 0x7f, 0xf8, 0x7d, 0x69,
  0x15, 0x36, 0xdb, 0x77, 0x6c, 0x2a, 0x25, 0xbf, 0x0f, 0xf1, 0xff, 0xa5,
  0x6b, 0xfe, 0x1b, 0xfc, 0x0f, 0xfc, 0x82, 0x1b, 0x6d, 0xb6, 0xdc, 0x84,
  0xff, 0xf8, 0x1f, 0xe3, 0xff, 0x4d, 0xbf, 0xff, 0x1f, 0x7e, 0x01, 0xff,
  0xd1, 0x4d, 0xb6, 0xdb, 0x69, 0x23, 0xff, 0x00, 0x3f, 0xc7, 0xfe, 0xbc,
  0x57, 0xff, 0x8f, 0xff, 0xc0, 0x03, 0xf2, 0x26, 0xdb, 0x6d, 0xb0, 0x9f,
  0x80, 0x01, 0xff, 0xcf, 0xf9, 0xd3, 0xad, 0x7f, 0xc7, 0xdf, 0xf0, 0x00,
  0x3c, 0x81, 0xb6, 0xdb, 0xc4, 0x9c, 0x00, 0x0f, 0xff, 0x8f, 0xe6, 0x2d,
  0x92, 0xbf, 0xe3, 0xff, 0xff, 0x00, 0x1e, 0x48, 0xdb, 0x6d, 0x09, 0x38,
  0x01, 0xff, 0xfe, 0x9f, 0x9b, 0xf6, 0x6a, 0x4f, 0xe3, 0x77, 0xff, 0xfe,
  0x07, 0x24, 0x2d, 0xb6, 0x24, 0xe0, 0xff, 0xff, 0xff, 0x9f, 0x6d, 0x4a,
  0xdd, 0xa5, 0xf1, 0xfe, 0xff, 0xff, 0xc3, 0x85, 0x05, 0x50, 0x91, 0xc1,
  0xff, 0xbf, 0xef, 0x1c, 0xd5, 0x7b, 0x23, 0x75, 0x39, 0xff, 0xff, 0xff,
  0xe1, 0xd0, 0xa0, 0x04, 0x8b, 0x87, 0xff, 0x9f, 0x7f, 0x1b, 0x36, 0xb4,
  0x0c, 0x9a, 0xd9, 0xdb, 0xdf, 0xff, 0xf8, 0xe9, 0x2a, 0xa9, 0x27, 0x0f,
  0xff, 0x1f, 0xff, 0x3b, 0x6a, 0xd8, 0x05, 0xab, 0xb8, 0xff, 0xfb, 0xff,
  0xfc, 0x72, 0x11, 0x12, 0x4e, 0x3f, 0xff, 0x1f, 0xf7, 0x3a, 0xd5, 0x61,
  0x06, 0xd4, 0xd8, 0xfe, 0xfc, 0x3f, 0xfc, 0x19, 0x44, 0x40, 0x9c, 0x3f,
  0xfb, 0x1d, 0xbf, 0x3a, 0xad, 0x87, 0x80, 0xad, 0x6c, 0xf7, 0xd8, 0x1e,
  0xff, 0x1c, 0x92, 0x4c, 0x58, 0xff, 0xfc, 0x4f, 0xff, 0x1b, 0x5a, 0x1e,
  0xf0, 0x6a, 0x4c, 0xff, 0xf0, 0x1f, 0xdf, 0x8f, 0x21, 0x22, 0xb0, 0xff,
  0xb0, 0xe1, 0xfb, 0x9a, 0xd8, 0x3c, 0x3c, 0x15, 0xbc, 0xff, 0xf0, 0x1f,
  0xff, 0xc3, 0xca, 0x49, 0x63, 0xff, 0xf0, 0xe0, 0xff, 0x1d, 0x60, 0xf0,
  0x07, 0x0d, 0x6c, 0xf6, 0xf0, 0x1d, 0xfb, 0xe1, 0xe9, 0x24, 0xc7, 0xfd,
  0xf8, 0x43, 0xe8, 0x1a, 0xa3, 0xc1, 0x03, 0x86, 0x9c, 0xff, 0xd0, 0x9e,
  0x7f, 0x70, 0x7c, 0x97, 0xc7, 0xff, 0xfe, 0x4f, 0xf0, 0x1d, 0x87, 0x00,
  0x00, 0xe2, 0xb8, 0xff, 0xf0, 0x3e, 0x6f, 0xf8, 0x3f, 0xff, 0x0f, 0xdf,
  0x7f, 0x1f, 0xe1, 0xfe, 0x8c, 0x01, 0x00, 0x71, 0x58, 0x1b, 0xf8, 0x3e,
  0x7f, 0xfe, 0x07, 0xfc, 0x0f, 0xfb, 0xef, 0x1d, 0xc7, 0xfb, 0x18, 0x72,
  0x20, 0x19, 0xaa, 0x0f, 0x7f, 0xff, 0xcd, 0xdf, 0x80, 0x00, 0x1f, 0xbf,
  0xff, 0x9f, 0x8f, 0xf4, 0x30, 0xf8, 0x00, 0x08, 0xaf, 0x87, 0xff, 0xff,
  0x87, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xff, 0xdf, 0x1f, 0xec, 0x63, 0xf8,
  0x09, 0x0c, 0x5f, 0xe3, 0xff, 0xff, 0xc7, 0xff, 0x7f, 0xff, 0xdf, 0xfb,
  0x43, 0xff, 0x1f, 0x98, 0xc3, 0xf8, 0x41, 0x86, 0x65, 0xf3, 0xbf, 0xcd,
  0xef, 0xbf, 0xff, 0xff, 0xfd, 0xbf, 0xc3, 0xff, 0x3e, 0x69, 0x8f, 0xf0,
  0x01, 0x82, 0x2b, 0xf1, 0xf6, 0xc7, 0xff, 0xf7, 0xdf, 0xff, 0x7f, 0xff,
  0x99, 0xfa, 0x39, 0xd1, 0x0f, 0xe0, 0x00, 0x23, 0x34, 0xf8, 0xff, 0xc0,
  0xff, 0xff, 0xfb, 0xef, 0xff, 0xff, 0x91, 0xfe, 0x7b, 0x63, 0x1b, 0xc8,
  0x22, 0x01, 0x96, 0x3c, 0xff, 0xe0, 0x1e, 0xff, 0x7f, 0xff, 0xbf, 0xdb,
  0xc3, 0xde, 0x76, 0xa6, 0x13, 0x08, 0x00, 0x49, 0x97, 0x5c, 0xf7, 0xf4,
  0x0f, 0xdb, 0xfd, 0xff, 0xfd, 0xcf, 0xe7, 0xfe, 0x75, 0x46, 0x1e, 0x00,
  0x00, 0x00, 0x89, 0xac, 0xfe, 0xff, 0xc7, 0xff, 0xff, 0xef, 0xff, 0xcf,
  0xff, 0xfe, 0x7a, 0xcc, 0x08, 0x0a, 0x89, 0x00, 0xcc, 0xdc, 0x7f, 0xff,
  0xc7, 0xff, 0xbf, 0xff, 0xbf, 0x8e, 0xff, 0xee, 0x6d, 0x8c, 0x40, 0x02,
  0x00, 0x24, 0x4d, 0x5c, 0x77, 0xbf, 0xe6, 0xdf, 0xff, 0xbb, 0xfe, 0x87,
  0xff, 0xf8, 0x75, 0x48, 0x40, 0x80, 0x00, 0x00, 0x46, 0xac, 0xff, 0xef,
  0xff, 0xfb, 0xfd, 0xff, 0xff, 0x87, 0xff, 0x60, 0x6a, 0x98, 0x48, 0x03,
  0x21, 0x01, 0x65, 0x4c, 0xfd, 0xfd, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x87,
  0xb7, 0xc3, 0xf6, 0x98, 0x80, 0x29, 0x00, 0x48, 0x66, 0xdc, 0x6f, 0xbf,
  0xff, 0x7f, 0x6f, 0xdb, 0x7e, 0x33, 0xff, 0x8f, 0xf5, 0x90, 0xc2, 0x04,
  0x04, 0x80, 0x65, 0x2c, 0x07, 0xff, 0x6f, 0xef, 0xff, 0xff, 0xf0, 0x30,
  0x7e, 0x1f, 0xf6, 0x10, 0x80, 0x82, 0x01, 0xc4, 0x26, 0xd8, 0x03, 0xef,
  0xfd, 0xfd, 0xfe, 0xff, 0xf0, 0xf8, 0x3e, 0x3e, 0xb5, 0x30, 0x10, 0x00,
  0x00, 0xc0, 0x25, 0x5f, 0xf0, 0xfd, 0xff, 0xff, 0xff, 0xdf, 0xfc, 0x70,
  0xfc, 0x7b, 0x4b, 0x32, 0x04, 0x08, 0x45, 0xc2, 0x25, 0xbf, 0xf8, 0xff,
  0xbf, 0xbf, 0xbf, 0xfe, 0xff, 0x23, 0xfc, 0x75, 0xbb, 0x20, 0x41, 0x20,
  0x01, 0xc0, 0x26, 0x4f, 0xfc, 0x6f, 0xf7, 0xff, 0xf7, 0xef, 0xdf, 0x87,
  0xd8, 0xed, 0x55, 0x20, 0x00, 0x00, 0x07, 0xf0, 0x25, 0x62, 0xfe, 0x3d,
  0xfe, 0xfb, 0xfe, 0xff, 0xff, 0x87, 0xf9, 0xea, 0xdb, 0x21, 0x10, 0x88,
  0x2f, 0xfd, 0x26, 0xdd, 0x1f, 0x3f, 0xbf, 0xff, 0x7f, 0xfd, 0xff, 0x8f,
  0xf9, 0xd6, 0xa9, 0x20, 0x04, 0x00, 0x8f, 0x7c, 0x24, 0x1a, 0xcf, 0x1f,
  0xfb, 0xb7, 0xff, 0xdf, 0xbb, 0xce, 0xf9, 0xb5, 0xb6, 0x20, 0x80, 0x00,
  0x03, 0x20, 0x40, 0x03, 0x67, 0x9d, 0xff, 0xfe, 0xef, 0xff, 0xff, 0xcf,
  0xd9, 0xad, 0x55, 0x30, 0x22, 0x44, 0x01, 0xc4, 0x43, 0x80, 0xb3, 0x9f,
  0xbf, 0xff, 0xfd, 0xfb, 0xff, 0xcf, 0xf9, 0xaa, 0xab, 0x32, 0x08, 0x00,
  0x89, 0xc0, 0x8f, 0xf8, 0x59, 0x9f, 0xf7, 0xff, 0xff, 0xbf, 0x6d, 0xff,
  0xf9, 0xdf, 0x6d, 0x90, 0x1f, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xfc, 0x03, 0x83, 0xfe, 0x00, 0x38, 0x3f, 0x83, 0x80, 0x0f, 0xfc, 0x03,
  0x82, 0x0e, 0x0f, 0xe0, 0xe0, 0x1f, 0xfb, 0xfd, 0x7d, 0xfd, 0xff, 0xd7,
  0xdf, 0x7d, 0x7f, 0xf7, 0xfb, 0xfd, 0x7d, 0xf5, 0xf7, 0xdf, 0x5f, 0xef,
  0xfa, 0x05, 0x45, 0xfd, 0x00, 0x54, 0x5f, 0x45, 0x40, 0x17, 0xfa, 0x05,
  0x45, 0x15, 0x17, 0xd1, 0x50, 0x2f, 0xe6, 0xf5, 0x55, 0xfd, 0x7f, 0x55,
  0x64, 0xd5, 0x5f, 0xd7, 0xfa, 0xf5, 0x55, 0x55, 0x59, 0x35, 0x57, 0xb3,
  0xde, 0x05, 0x55, 0xfd, 0x08, 0x55, 0x7b, 0xd5, 0x50, 0x17, 0xfa, 0x15,
  0x55, 0x55, 0x5e, 0xf5, 0x54, 0x3d, 0xd1, 0xfd, 0x55, 0xfd, 0xeb, 0xd5,
  0x0a, 0x15, 0x57, 0xf7, 0xfb, 0xd5, 0x55, 0x55, 0x42, 0x85, 0x55, 0xc5,
  0xd5, 0x03, 0x55, 0xfe, 0x6b, 0x35, 0xea, 0xf5, 0x54, 0x0f, 0xfc, 0xd5,
  0x55, 0x55, 0x7a, 0xbd, 0x55, 0x55, 0xd1, 0xef, 0x55, 0xff, 0xaa, 0xf5,
  0x0e, 0x15, 0x57, 0xbf, 0xff, 0x55, 0x55, 0x55, 0x43, 0x85, 0x55, 0xc5,
  0xde, 0x2f, 0x55, 0xff, 0xaa, 0xf5, 0x71, 0xd5, 0x50, 0xbf, 0xff, 0x55,
  0x55, 0x55, 0x5c, 0x75, 0x54, 0x3d, 0xe6, 0xb3, 0x55, 0xff, 0xaa, 0xf5,
  0x55, 0x55, 0x5e, 0xbf, 0xe0, 0x55, 0x55, 0x55, 0x55, 0x55, 0x57, 0xb3,
  0xfa, 0x3d, 0x55, 0xff, 0xaa, 0xf5, 0x51, 0x55, 0x50, 0xbf, 0xdf, 0x55,
  0x55, 0x55, 0x54, 0x55, 0x54, 0x2f, 0xfb, 0xc5, 0x55, 0xff, 0xaa, 0xf5,
  0x5f, 0x55, 0x57, 0xbf, 0xd1, 0x55, 0x55, 0x55, 0x57, 0xd5, 0x55, 0xef,
  0xe0, 0x55, 0x56, 0x0e, 0x6b, 0x35, 0x40, 0x55, 0x56, 0x0f, 0xd5, 0x55,
  0x55, 0x55, 0x50, 0x15, 0x55, 0x9f, 0xdf, 0xc5, 0x57, 0xf5, 0xeb, 0xd5,
  0x5f, 0x55, 0x57, 0xf7, 0xd1, 0xc5, 0x47, 0x15, 0x57, 0xd5, 0x55, 0x7f,
  0xd0, 0x3d, 0x50, 0x15, 0x08, 0x55, 0x5f, 0x55, 0x50, 0x17, 0xde, 0x3d,
  0x78, 0xf5, 0x57, 0xd5, 0x55, 0x7f, 0xd7, 0xb3, 0x5f, 0xd5, 0x7f, 0x55,
  0x5f, 0x55, 0x5f, 0xd7, 0xe6, 0xb3, 0x9a, 0xcd, 0x57, 0xd5, 0x55, 0x7f,
  0xd0, 0x2f, 0x40, 0x15, 0x00, 0x54, 0x5f, 0x45, 0x40, 0x17, 0xfa, 0x2f,
  0xe8, 0xbd, 0x17, 0xd1, 0x51, 0x7f, 0xdf, 0xef, 0x7f, 0xf5, 0xff, 0xd7,
  0xdf, 0x7d, 0x7f, 0xf7, 0xfb, 0xef, 0xef, 0xbd, 0xf7, 0xdf, 0x5f, 0x7f,
  0xe0, 0x1f, 0x80, 0x0e, 0x00, 0x38, 0x3f, 0x83, 0x80, 0x0f, 0xfc, 0x1f,
  0xf0, 0x7e, 0x0f, 0xe0, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc2, 0x10, 0x84,
  0x38, 0x45, 0x6d, 0xa1, 0x6c, 0x10, 0xc2, 0x08, 0x42, 0x0f, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xda, 0xd7, 0xbd, 0xfb, 0x55, 0x6d, 0xaf, 0x6f, 0x76,
  0xdf, 0xbb, 0x5b, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc2, 0x30, 0x84,
  0x38, 0x55, 0x0c, 0x61, 0x0f, 0x76, 0xc3, 0xb8, 0x47, 0xbf, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xde, 0xd7, 0xf7, 0xbb, 0x55, 0xed, 0xaf, 0xef, 0x76,
  0xfb, 0xbb, 0x5b, 0xbf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xde, 0xd0, 0x84,
  0x3b, 0x51, 0x0d, 0xa1, 0x0f, 0x70, 0xc3, 0xbb, 0x5b, 0xbf, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const uint8_t spriteSheetData[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00,
  0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00,
  0x00, 0x01, 0x80, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x0c, 0x30,
  0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00,
  0x0c, 0x30, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0x00, 0x00, 0xe0, 0x00, 0x0b, 0xd0, 0x00, 0x00, 0x05, 0xa0, 0x00,
  0x0c, 0x02, 0x40, 0x30, 0x00, 0x1a, 0x58, 0x00, 0x00, 0x0d, 0xb0, 0x00, 0x18, 0x0e, 0x70, 0x18, 0x00, 0x36, 0x6c,
  0x00, 0x00, 0x08, 0x10, 0x00, 0x10, 0x38, 0x1c, 0x08, 0x00, 0xe4, 0x27, 0x00, 0x00, 0x18, 0x18, 0x00, 0x10, 0x00,
  0x00, 0x08, 0x01, 0x80, 0x01, 0x80, 0x00, 0x30, 0x0c, 0x00, 0x30, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x00, 0x80, 0x00,
  0x20, 0x04, 0x00, 0x20, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0xc0, 0x00, 0x60, 0x06, 0x00, 0x20, 0x00, 0x00, 0x04,
  0x06, 0x00, 0x00, 0x60, 0x00, 0xc0, 0x03, 0x00, 0x20, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x20, 0x00, 0x80, 0x01,
  0x00, 0x20, 0x00, 0x00, 0x04, 0x0c, 0x00, 0x00, 0x30, 0x00, 0x80, 0x01, 0x00, 0x20, 0x00, 0x00, 0x04, 0x08, 0x00,
  0x00, 0x10, 0x01, 0x80, 0x01, 0x80, 0x20, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x80, 0x30,
  0x00, 0x00, 0x0c, 0x08, 0x00, 0x00, 0x10, 0x00, 0x80, 0x01, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xe0,
  0x00, 0x7f, 0xfe, 0x00
};

::SpriteSheet spriteSheet = ::SpriteSheet(spriteSheetData, 96, 32, 3);

const uint8_t *frames[] = {};
Sprite slime(frames, 3, 32, 32);

typedef enum : uint16_t {
  START_SCREEN = Kywy::Events::USER_EVENTS,
  PLATFORM_COLLISION,
  JUMP,
  GAME_OVER,
} SlimeJumperSignal;

::Actor::Message platformCollisionMessage = ::Actor::Message(PLATFORM_COLLISION);
::Actor::Message jumpMessage = ::Actor::Message(JUMP);
::Actor::Message gameOverMessage = ::Actor::Message(GAME_OVER);

class SlimeManager : public Actor::Actor {
public:
  int xMaxVelocity = 5;
  int xVelocity = 0;
  int xPosition = 48;

  int yMaxVelocity = -10;  // note that origin is top left so negative velocity
                           // is up on the screen
  int yVelocity = yMaxVelocity;
  int yPosition = 90;

  int padding = 5;

  bool buttonLeftPressed = false;
  bool buttonRightPressed = false;

  void initialize() {
    slime.setPosition(xPosition, yPosition);
    slime.setVisible(true);
    slime.setNegative(true);
  }

  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::D_PAD_LEFT_PRESSED:
        buttonLeftPressed = true;
        xVelocity = -1 * xMaxVelocity;
        break;
      case Kywy::Events::D_PAD_RIGHT_PRESSED:
        buttonRightPressed = true;
        xVelocity = xMaxVelocity;
        break;
      case Kywy::Events::D_PAD_LEFT_RELEASED:
        buttonLeftPressed = false;
        if (buttonRightPressed) {
          xVelocity = xMaxVelocity;
        } else {
          xVelocity = 0;
        }
        break;
      case Kywy::Events::D_PAD_RIGHT_RELEASED:
        buttonRightPressed = false;
        if (buttonLeftPressed) {
          xVelocity = -1 * xMaxVelocity;
        } else {
          xVelocity = 0;
        }
        break;
      case PLATFORM_COLLISION:
        if (yVelocity > 0) {
          yVelocity = yMaxVelocity;

          publish(&jumpMessage);
        }
        break;
      case Kywy::Events::TICK:
        // side to side motion
        xPosition += xVelocity;

        if (xPosition < padding)
          xPosition = padding;

        if (xPosition > KYWY_DISPLAY_WIDTH - 32 - padding)
          xPosition = KYWY_DISPLAY_WIDTH - 32 - padding;

        // falling
        yPosition += yVelocity;
        if (yVelocity < 7)
          yVelocity += 1;

        // jumping
        if (yPosition > KYWY_DISPLAY_HEIGHT - 18) {
          publish(&gameOverMessage);
        }

        // animation
        if (yVelocity < -5) {
          slime.setFrame(2);
        } else if (yVelocity < -2) {
          slime.setFrame(1);
        } else {
          slime.setFrame(0);
        }

        slime.setPosition(xPosition, yPosition);

        slime.render();
        break;
    }
  }

} slimeManager;

typedef struct Platform {
  int x, y, width;
} Platform;

class PlatformManager : public Actor::Actor {
public:
  const static int numPlatforms = 4;

  int yVelocity = 2;
  int platformWidth = 40;

  Platform platforms[numPlatforms];

  void initialize() {
    for (int i = 0; i < numPlatforms; i++) {
      platforms[i] = Platform{ .x = (int)fmax(5, fmin(random(KYWY_DISPLAY_WIDTH), KYWY_DISPLAY_WIDTH - 5 - platformWidth)),
                               .y = KYWY_DISPLAY_HEIGHT - 18 - (50 * i),
                               .width = platformWidth };
    }
  }

  void drawPlatforms(uint16_t color) {
    for (int i = 0; i < numPlatforms; i++) {
      if (platforms[i].y >= 0 && platforms[i].y <= KYWY_DISPLAY_HEIGHT) {
        engine.display.drawLine(platforms[i].x, platforms[i].y, int16_t(platforms[i].x + platforms[i].width), int16_t(platforms[i].y), Display::Object1DOptions().color(color));
      }
    }
  }

  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::TICK:
        drawPlatforms(WHITE);  // erase platforms

        // check for collisions
        for (int i = 0; i < numPlatforms; i++) {

          // stupid hack because the uint y value rolls over to 65535 instead of
          // going negative
          int slimeY;
          if (slime.y < 200) {
            slimeY = slime.y;
          } else {
            slimeY = (int)slime.y - 65535;
          }

          int slimeBottom = slimeY + slime.height;
          int slimeSide = slime.x;

          if (slimeBottom > platforms[i].y + 3)
            continue;

          if (slimeBottom < platforms[i].y - 3)
            continue;

          if (slimeSide > platforms[i].x + platformWidth - 10)
            continue;

          if (slimeSide < platforms[i].x - platformWidth + 10)
            continue;

          publish(&platformCollisionMessage);
        }

        // move platforms
        for (int i = 0; i < numPlatforms; i++) {
          platforms[i].y += yVelocity;

          if (platforms[i].y > KYWY_DISPLAY_HEIGHT + 25) {
            platforms[i].y = -5;
            platforms[i].x = (int)fmax(5, fmin(random(KYWY_DISPLAY_WIDTH), KYWY_DISPLAY_WIDTH - 5 - platformWidth));
          }
        }

        drawPlatforms(BLACK);  // draw platforms
    }
  }

} platformManager;

class GameManager : public Actor::Actor {
public:
  int score = 0;
  int highScore = 0;

  void drawScore(uint16_t color) {
    char msg[16];
    snprintf(msg, sizeof(msg), "%d", (uint16_t)score);
    engine.display.drawText(5, 5, msg, Display::TextOptions().color(color));
  }

  void initialize() {}

  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case START_SCREEN:
        {
          slimeManager.unsubscribe(&engine.clock);
          platformManager.unsubscribe(&engine.clock);
          unsubscribe(&engine.clock);
          engine.display.drawBitmap(0, 0, 144, 168, (uint8_t *)slimeJumperSplashScreenBMP);
          engine.display.update();
          subscribe(&engine.input);
          break;
        }
      case Kywy::Events::TICK:
        drawScore(BLACK);
        engine.display.update();
        break;
      case JUMP:
        drawScore(WHITE);
        score += 1;
        break;
      case GAME_OVER:
        {
          slimeManager.unsubscribe(&engine.clock);
          platformManager.unsubscribe(&engine.clock);
          unsubscribe(&engine.clock);
          subscribe(&engine.input);

          if (score > highScore)
            highScore = score;

          engine.display.clear();
          engine.display.drawText(5, 5, "GAME OVER");
          char msg[32];
          snprintf(msg, sizeof(msg), "Score: %d", (uint16_t)score);
          engine.display.drawText(5, 15, msg);
          snprintf(msg, sizeof(msg), "High Score: %d", highScore);
          engine.display.drawText(5, 25, msg);
          engine.display.drawText(5, 45, "Press any button");
          engine.display.drawText(5, 55, "to try again.");
          engine.display.update();
          break;
        }
      case Kywy::Events::INPUT_PRESSED:
        {
          unsubscribe(&engine.input);

          platformManager.initialize();
          slime.setPosition(48, 90);
          slimeManager.yVelocity = -12;

          score = 0;

          engine.display.clear();

          slimeManager.subscribe(&engine.clock);
          platformManager.subscribe(&engine.clock);
          subscribe(&engine.clock);
          break;
        }
    }
  }
} gameManager;

void setup() {
  engine.start();

  spriteSheet.addFrames(0, 0, 32, 32, 3);
  slime.frames = spriteSheet.frames;
  slime.setDisplay(&engine.display);

  // set up game managers
  slimeManager.subscribe(&engine.input);
  slimeManager.subscribe(&engine.clock);
  slimeManager.subscribe(&platformManager);
  slimeManager.start();

  platformManager.subscribe(&engine.clock);
  platformManager.start();

  gameManager.subscribe(&slimeManager);
  gameManager.subscribe(&engine.clock);
  gameManager.start();

  ::Actor::Message message(START_SCREEN);
  gameManager.handle(&message);
}

void loop() {
  delay(1000);
}
