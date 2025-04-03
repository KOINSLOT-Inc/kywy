// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"
// #include "AsteroidSpriteSheet.h"

Kywy::Engine engine;

#define DISPLAY_WIDTH  144
#define DISPLAY_HEIGHT 168
#define BLACK          0x0
#define WHITE          0xf

const uint8_t asteroidSplashScreen[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
    0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0F,
    0xD0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
    0xF0, 0x44, 0x10, 0x00, 0x08, 0x30, 0x00, 0x20, 0x10, 0x20, 0x01, 0x00,
    0x00, 0x01, 0x02, 0x00, 0x02, 0x0F, 0xF0, 0x00, 0x40, 0x88, 0x80, 0x00,
    0x22, 0x01, 0x00, 0x81, 0x10, 0x01, 0x11, 0x17, 0xF0, 0x11, 0x08, 0x0F,
    0xF0, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x7F, 0xFF, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x01, 0xDD, 0xFF, 0xC0, 0x00, 0x0F,
    0xD0, 0x80, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x02, 0xAF, 0xBF, 0xF0, 0x00, 0x2F, 0xF0, 0x00, 0x01, 0x00, 0x02, 0x28,
    0x00, 0x04, 0x10, 0x00, 0x00, 0x40, 0x0B, 0xFB, 0xFB, 0xF8, 0x00, 0x0F,
    0xF0, 0x04, 0x04, 0x08, 0x42, 0x00, 0x02, 0x10, 0x00, 0x00, 0x11, 0x00,
    0x0A, 0x77, 0xEB, 0xBC, 0x42, 0x0D, 0xF0, 0x00, 0x20, 0x00, 0x02, 0x00,
    0x08, 0x00, 0x01, 0x08, 0x40, 0x00, 0x11, 0x5B, 0xBF, 0xFE, 0x00, 0x0F,
    0xF0, 0x20, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x4D, 0x79, 0x7F, 0xFA, 0x00, 0x0F, 0xF0, 0x80, 0x00, 0x00, 0x0A, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xBF, 0xFE, 0xD7, 0x00, 0x0F,
    0xD0, 0x00, 0x00, 0x21, 0x42, 0x00, 0x40, 0x00, 0x44, 0x00, 0x00, 0x00,
    0x02, 0x7B, 0x7F, 0xFB, 0x00, 0x4F, 0xF0, 0x00, 0x80, 0x80, 0x02, 0x11,
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x40, 0x1B, 0xF5, 0xBF, 0x84, 0x0F,
    0xF0, 0x00, 0x02, 0x00, 0x0F, 0x80, 0x01, 0x00, 0x00, 0x11, 0x00, 0x44,
    0x22, 0x5E, 0x7E, 0xBE, 0x80, 0x0F, 0xF0, 0x04, 0x00, 0x00, 0x0F, 0xC0,
    0x00, 0x01, 0x60, 0x00, 0x09, 0x00, 0x03, 0x0A, 0xDF, 0xEB, 0x80, 0x0F,
    0xF0, 0x42, 0x00, 0x00, 0x7F, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x4B, 0x9F, 0xFF, 0x80, 0x0F, 0xF0, 0x00, 0x00, 0x02, 0x0F, 0x80,
    0x08, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x01, 0xF7, 0x1A, 0x82, 0x0F,
    0xD0, 0x00, 0x10, 0x08, 0x13, 0x40, 0x40, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x41, 0x04, 0xF1, 0xD7, 0x02, 0x0F, 0xF0, 0x00, 0x00, 0x80, 0x02, 0x00,
    0x00, 0x08, 0x00, 0x00, 0x80, 0x00, 0x00, 0x02, 0xAE, 0xD9, 0x00, 0x2F,
    0xF0, 0x40, 0x00, 0x00, 0x02, 0x00, 0x00, 0x41, 0x00, 0x00, 0x80, 0x04,
    0x24, 0x10, 0x32, 0x7F, 0x00, 0x0D, 0xF0, 0x01, 0x00, 0x00, 0x02, 0x00,
    0x01, 0x00, 0x08, 0x00, 0x04, 0x00, 0x00, 0x04, 0x97, 0x56, 0x08, 0x0F,
    0xF0, 0x08, 0x08, 0x00, 0x12, 0x02, 0x04, 0x00, 0x00, 0x04, 0x00, 0x20,
    0x00, 0x80, 0x06, 0x28, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x42, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x21, 0x18, 0x00, 0x0F,
    0xF0, 0x80, 0x00, 0x44, 0x02, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x70, 0x00, 0x0F, 0xD0, 0x00, 0x21, 0x00, 0x00, 0x08,
    0x00, 0x00, 0x20, 0x00, 0x10, 0x00, 0x00, 0x00, 0x09, 0x20, 0x02, 0x0F,
    0xF0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x02, 0x00, 0x80, 0x80, 0x00, 0x00,
    0x80, 0x20, 0x01, 0x00, 0x20, 0x2F, 0xF0, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x40, 0x04, 0x00, 0x00, 0x40, 0x82, 0x00, 0x84, 0x88, 0x00, 0x00, 0x0D,
    0xF0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x04, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x40, 0x00, 0x21, 0x11, 0x10,
    0x00, 0x20, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
    0xF0, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x05, 0x01, 0x0F, 0xD0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x80, 0x04, 0x10, 0x08, 0x00, 0x01, 0x16, 0x08, 0x0F,
    0xF1, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x02, 0x00, 0x40, 0x00,
    0x00, 0x00, 0x20, 0x66, 0x00, 0x0F, 0xF0, 0x01, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x20, 0x00, 0x00, 0x01, 0x00, 0x11, 0x07, 0x30, 0x00, 0x0F,
    0xF0, 0x00, 0x00, 0x04, 0x20, 0x80, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40,
    0x20, 0x00, 0x78, 0xE4, 0x00, 0x0F, 0xF0, 0x00, 0x20, 0x00, 0x02, 0x04,
    0x01, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x43, 0x84, 0x04, 0x0F,
    0xF0, 0x08, 0x00, 0x40, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00,
    0x00, 0x9A, 0x0F, 0x84, 0x10, 0x4F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x10, 0x00, 0x00, 0x04, 0x00, 0x02, 0x00, 0x50, 0xFF, 0xC8, 0x00, 0x0F,
    0xF0, 0x40, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x02, 0x10, 0x00, 0x00,
    0x03, 0xC3, 0x7D, 0x08, 0x00, 0x0D, 0xD0, 0x00, 0x80, 0x02, 0x00, 0x00,
    0x00, 0x20, 0x10, 0x00, 0x08, 0x00, 0x1F, 0x0D, 0xF0, 0x18, 0x00, 0x0F,
    0xF0, 0x00, 0x04, 0x08, 0x04, 0x10, 0x00, 0x80, 0x00, 0x00, 0x20, 0x10,
    0x74, 0x63, 0xF2, 0x10, 0x02, 0x0F, 0xF0, 0x00, 0x00, 0x20, 0x00, 0x00,
    0x42, 0x00, 0xA0, 0x00, 0x00, 0x01, 0xF1, 0x2B, 0xE8, 0x10, 0x20, 0x0F,
    0xF0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86,
    0xC5, 0xBE, 0xB8, 0x20, 0x00, 0x0F, 0xF0, 0x80, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x40, 0x00, 0x00, 0x00, 0x1D, 0x05, 0xFF, 0x60, 0xE0, 0x00, 0x4F,
    0xF0, 0x00, 0x88, 0x00, 0x08, 0x82, 0x00, 0x00, 0x00, 0x22, 0x00, 0x3C,
    0x07, 0xDE, 0xA0, 0x40, 0x01, 0x0F, 0xD0, 0x00, 0x20, 0x40, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x80, 0x00, 0xC0, 0x37, 0x7B, 0x11, 0x40, 0x84, 0x0F,
    0xF0, 0x00, 0x00, 0x00, 0x80, 0x00, 0x08, 0x01, 0x02, 0x00, 0x13, 0xA0,
    0x3B, 0xF7, 0x82, 0x80, 0x00, 0x0F, 0xF0, 0x10, 0x00, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x00, 0x07, 0x40, 0x3B, 0xEE, 0x43, 0x80, 0x00, 0x0F,
    0xF0, 0x40, 0x00, 0x00, 0x00, 0x10, 0x40, 0x08, 0x00, 0x20, 0x1D, 0x08,
    0x37, 0x7E, 0x05, 0x00, 0x00, 0x0D, 0xF0, 0x00, 0x80, 0x01, 0x00, 0x00,
    0x00, 0x40, 0x00, 0x00, 0xBA, 0x0C, 0xEF, 0xE8, 0x03, 0x00, 0x20, 0x0F,
    0xF0, 0x00, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x11,
    0xDD, 0xE8, 0x0A, 0x02, 0x00, 0x0F, 0xF0, 0x10, 0x00, 0x00, 0x00, 0x82,
    0x00, 0x00, 0x00, 0x02, 0xD0, 0x07, 0x7F, 0x10, 0x16, 0x00, 0x10, 0x8F,
    0xE1, 0x00, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x40, 0x03, 0xA0, 0xFE,
    0xFB, 0xA0, 0x8C, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x20, 0x00, 0x04, 0x00,
    0x08, 0x04, 0x04, 0x27, 0x44, 0x9D, 0xFF, 0x60, 0x18, 0x00, 0x00, 0x0F,
    0xF0, 0x02, 0x00, 0x00, 0x40, 0x04, 0x00, 0x00, 0x00, 0x0E, 0x09, 0xD7,
    0xDA, 0x80, 0x50, 0x02, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x3C, 0x09, 0xAF, 0x74, 0x00, 0xB0, 0x08, 0x04, 0x0F,
    0xF0, 0x10, 0x00, 0x20, 0x00, 0x10, 0x00, 0x10, 0x00, 0x70, 0x1F, 0xDF,
    0xE8, 0x00, 0x20, 0x00, 0x10, 0x2F, 0xD0, 0x40, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0xC2, 0x52, 0xBF, 0xD0, 0x80, 0xC0, 0x00, 0x00, 0x0D,
    0xF0, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x09, 0x81, 0x0F, 0x7B,
    0x64, 0x01, 0x80, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x42, 0x00, 0x04, 0x40,
    0x80, 0x40, 0x03, 0x00, 0x71, 0xFF, 0xD0, 0x05, 0x80, 0x40, 0x00, 0x0F,
    0xF0, 0x00, 0x00, 0x00, 0x40, 0x00, 0x01, 0x00, 0x07, 0x0C, 0xFE, 0xDE,
    0x54, 0x01, 0x00, 0x00, 0x80, 0x0F, 0xF0, 0x80, 0x80, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x2E, 0x21, 0xBF, 0xF5, 0x80, 0x26, 0x00, 0x02, 0x01, 0x0F,
    0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x9E, 0x0D, 0xED, 0xFF,
    0x40, 0x0C, 0x01, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x02, 0x30, 0x33, 0xFF, 0xB8, 0x80, 0x4C, 0x04, 0x01, 0x00, 0x0F,
    0xD0, 0x00, 0x04, 0x10, 0x08, 0x80, 0x00, 0x07, 0xB0, 0x27, 0xBE, 0xDA,
    0x00, 0xB8, 0x00, 0x00, 0x04, 0x0F, 0xF0, 0x10, 0x00, 0x00, 0x00, 0x00,
    0x20, 0xFC, 0x60, 0x5F, 0xFB, 0xF0, 0x01, 0x70, 0x00, 0x04, 0x00, 0x0F,
    0xF0, 0x80, 0x00, 0x00, 0x80, 0x00, 0x0F, 0xC2, 0x80, 0x9F, 0xEB, 0x60,
    0x06, 0xE0, 0x20, 0x00, 0x00, 0x2F, 0xF0, 0x00, 0x21, 0x00, 0x00, 0x21,
    0x78, 0x20, 0x02, 0xFD, 0xBE, 0x90, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x0D,
    0xF0, 0x00, 0x80, 0x02, 0x01, 0x83, 0xC6, 0x01, 0x09, 0x6F, 0xF5, 0xA8,
    0x2B, 0x00, 0x00, 0x40, 0x20, 0x0F, 0xF0, 0x00, 0x00, 0x20, 0x07, 0x5E,
    0x20, 0x00, 0x02, 0xFF, 0xFD, 0x40, 0x07, 0x00, 0x00, 0x00, 0x01, 0x0F,
    0xD0, 0x00, 0x00, 0x00, 0x05, 0x30, 0x00, 0x06, 0x27, 0xF8, 0xDA, 0x00,
    0x0C, 0x00, 0x82, 0x02, 0x00, 0x0F, 0xF0, 0x04, 0x01, 0x00, 0x1A, 0xC8,
    0x20, 0x0C, 0x01, 0x65, 0xE0, 0x00, 0x1C, 0x00, 0x08, 0x00, 0x00, 0x0F,
    0xF0, 0x40, 0x00, 0x00, 0x68, 0x49, 0x00, 0x30, 0x2B, 0x93, 0x60, 0x01,
    0x38, 0x00, 0x00, 0x80, 0x08, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0xD8, 0x09,
    0x00, 0x20, 0x8A, 0x63, 0xC8, 0x02, 0x70, 0x08, 0x00, 0x00, 0x00, 0x4F,
    0xF0, 0x00, 0x10, 0x05, 0xA1, 0x00, 0x01, 0x40, 0x79, 0xC6, 0x90, 0x09,
    0x60, 0x20, 0x00, 0x08, 0x42, 0x0F, 0xF0, 0x00, 0x40, 0x12, 0xC0, 0x24,
    0x04, 0x02, 0x23, 0x0C, 0x80, 0x43, 0x80, 0x00, 0x40, 0x00, 0x00, 0x0F,
    0xF0, 0x11, 0x80, 0x8D, 0x90, 0x00, 0x10, 0x84, 0x8E, 0x0D, 0x01, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x80, 0x00, 0x1B, 0x00, 0x90,
    0x00, 0x00, 0x98, 0x95, 0x00, 0x46, 0x00, 0x00, 0x01, 0x00, 0x00, 0x0F,
    0xD0, 0x00, 0x00, 0x36, 0x00, 0x00, 0x01, 0x01, 0x70, 0x30, 0x04, 0x0C,
    0x00, 0x80, 0x04, 0x00, 0x01, 0x0F, 0xF0, 0x00, 0x00, 0x6D, 0x02, 0x04,
    0x44, 0x08, 0xC2, 0x62, 0x00, 0x08, 0x02, 0x02, 0x20, 0x0A, 0x44, 0x0F,
    0xF0, 0x00, 0x04, 0xD8, 0x01, 0x00, 0x04, 0x21, 0x84, 0xC9, 0x10, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x04, 0x00, 0xB0, 0x00, 0x80,
    0x00, 0x86, 0x08, 0x80, 0x80, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
    0xF0, 0x40, 0x41, 0x60, 0x24, 0x0F, 0xD0, 0x0C, 0x10, 0x00, 0x10, 0xE0,
    0x10, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x02, 0x80, 0x21, 0xC0,
    0x20, 0x98, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x08, 0x00, 0x20, 0x00, 0x4F,
    0xF0, 0x00, 0x04, 0x81, 0x1C, 0x00, 0x21, 0x30, 0x02, 0x08, 0x00, 0xD0,
    0x80, 0x00, 0x44, 0x00, 0x10, 0x0F, 0xF0, 0x10, 0x0D, 0x00, 0x60, 0x00,
    0x42, 0xE0, 0x00, 0x00, 0x06, 0xD0, 0x00, 0x80, 0x00, 0x01, 0x00, 0x0F,
    0xD0, 0x80, 0x2A, 0x03, 0x00, 0x01, 0x01, 0x82, 0x08, 0x21, 0x06, 0xC0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x01, 0x14, 0x14, 0x00, 0x09,
    0x83, 0x04, 0x21, 0x00, 0x16, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x0F,
    0xF0, 0x00, 0x20, 0x90, 0x00, 0x00, 0x06, 0x20, 0x00, 0x00, 0x4A, 0xC0,
    0x84, 0x04, 0x40, 0x44, 0x00, 0x0E, 0xF0, 0x00, 0x2C, 0x40, 0x00, 0x04,
    0x0C, 0x40, 0x10, 0x80, 0x5C, 0xC0, 0x00, 0x10, 0x0C, 0x00, 0x08, 0x0F,
    0xE0, 0x04, 0x59, 0x00, 0x00, 0x42, 0x18, 0x40, 0x00, 0x08, 0x95, 0x80,
    0x00, 0x00, 0x08, 0x00, 0x00, 0x0F, 0xF1, 0x10, 0x82, 0x00, 0x01, 0x04,
    0x30, 0x80, 0x80, 0x00, 0xFD, 0x80, 0x00, 0x00, 0x80, 0x00, 0x20, 0x0F,
    0xF0, 0x00, 0x88, 0x00, 0x10, 0x10, 0x66, 0x10, 0x02, 0x04, 0xE5, 0x02,
    0x00, 0x80, 0x00, 0x00, 0x18, 0x0F, 0xF0, 0x01, 0x10, 0x01, 0x00, 0x44,
    0xC4, 0x08, 0x24, 0x01, 0xAD, 0x80, 0x10, 0x00, 0x00, 0x21, 0x00, 0x4F,
    0xF0, 0x02, 0x00, 0x04, 0x00, 0xF0, 0x99, 0x04, 0x00, 0x1A, 0x5D, 0x80,
    0x00, 0x00, 0x00, 0x80, 0x00, 0x0F, 0xF0, 0x02, 0x80, 0x00, 0x01, 0x05,
    0x26, 0x02, 0x20, 0x42, 0xFF, 0x00, 0x40, 0x00, 0x02, 0x00, 0x00, 0x0F,
    0xD0, 0x44, 0x00, 0x20, 0x08, 0x02, 0x48, 0x20, 0x02, 0x13, 0xB3, 0x0C,
    0x00, 0x44, 0x10, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x80, 0x01, 0x36,
    0x84, 0x01, 0x01, 0x2D, 0xBB, 0x10, 0x01, 0x00, 0x40, 0x00, 0x01, 0x0F,
    0xF0, 0x08, 0x02, 0x00, 0x00, 0xFD, 0x12, 0x01, 0x01, 0x6F, 0x7A, 0x68,
    0x00, 0x00, 0x00, 0x04, 0x44, 0x0F, 0xF0, 0x00, 0x08, 0x00, 0x41, 0xE8,
    0x08, 0x14, 0x00, 0x9B, 0xF9, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
    0xF0, 0x80, 0x00, 0x01, 0x02, 0xD1, 0xC6, 0x00, 0x08, 0x9E, 0xE3, 0xD0,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x00, 0x40, 0x00, 0x0F, 0xA7,
    0xE0, 0x40, 0x20, 0xB7, 0xA5, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0F,
    0xF0, 0x01, 0x00, 0x00, 0x1F, 0x8E, 0xE1, 0x40, 0x00, 0xAD, 0x9F, 0x20,
    0x00, 0x08, 0x00, 0x40, 0x00, 0x2F, 0xF0, 0x08, 0x00, 0x04, 0x3B, 0x1F,
    0xD8, 0x01, 0x00, 0xBF, 0x3C, 0x40, 0x00, 0x21, 0x24, 0x02, 0x00, 0x8D,
    0xF1, 0x00, 0x00, 0x10, 0x7F, 0x3F, 0xF0, 0x80, 0x00, 0x9A, 0x7C, 0x00,
    0x00, 0x80, 0x00, 0x00, 0x44, 0x0F, 0xF0, 0x00, 0x02, 0x00, 0xFC, 0xFB,
    0x71, 0x00, 0x00, 0xB5, 0xE8, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x0F,
    0xF0, 0x02, 0x10, 0x01, 0xDF, 0xFE, 0xA0, 0x02, 0x02, 0xB3, 0xB1, 0x01,
    0x10, 0x00, 0x00, 0x04, 0x00, 0x0F, 0xD0, 0x16, 0x00, 0x03, 0x77, 0xEE,
    0xC0, 0x00, 0x08, 0x44, 0xC2, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x0F,
    0xF0, 0x00, 0x00, 0x27, 0x7F, 0x7D, 0x80, 0x08, 0x01, 0x4B, 0x84, 0x04,
    0x00, 0x00, 0x80, 0x00, 0x00, 0x0F, 0xF0, 0x80, 0x00, 0x8C, 0xFF, 0xFF,
    0x00, 0x20, 0x01, 0x16, 0x08, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x0F,
    0xF0, 0x00, 0x00, 0x0B, 0xDB, 0xCD, 0x20, 0x00, 0x02, 0x2C, 0x98, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x8F, 0xF0, 0x00, 0x08, 0x0A, 0xFF, 0x7A,
    0x00, 0x00, 0x22, 0x78, 0x20, 0x00, 0x00, 0x22, 0x00, 0x02, 0x00, 0x0F,
    0xD0, 0x00, 0x40, 0x97, 0xFF, 0xB4, 0x02, 0x40, 0x04, 0xB0, 0x40, 0x20,
    0x21, 0x00, 0x08, 0x08, 0x00, 0x0F, 0xF0, 0x41, 0x01, 0x2F, 0x6D, 0xC8,
    0x00, 0x00, 0x05, 0x60, 0x80, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x0F,
    0xF0, 0x04, 0x02, 0x5D, 0xFF, 0x90, 0x00, 0x00, 0x8A, 0xE1, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x2F, 0xF0, 0x20, 0x04, 0x8F, 0xF5, 0x22,
    0x00, 0x00, 0x05, 0x46, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x0D,
    0xF0, 0x00, 0x08, 0x5F, 0xBA, 0x44, 0x02, 0x02, 0x12, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x40, 0x00, 0x00, 0x0F, 0xD0, 0x80, 0x10, 0xBE, 0xF4, 0x80,
    0x10, 0x00, 0x0C, 0x10, 0x00, 0x24, 0x04, 0x08, 0x00, 0x00, 0x10, 0x8F,
    0xF0, 0x00, 0x01, 0xFB, 0xA0, 0x00, 0x00, 0x10, 0x18, 0x40, 0x08, 0x00,
    0x00, 0x20, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x06, 0x9F, 0x50, 0x00,
    0x00, 0x00, 0x30, 0x80, 0x00, 0x00, 0x24, 0x00, 0x00, 0x42, 0x00, 0x0F,
    0xF0, 0x00, 0x85, 0xFC, 0xA0, 0x00, 0x00, 0x80, 0x62, 0x00, 0x00, 0x40,
    0x04, 0x00, 0x01, 0x00, 0x00, 0x0F, 0xF0, 0x04, 0x11, 0x75, 0x44, 0x48,
    0x82, 0x00, 0xC4, 0x00, 0x40, 0x00, 0x00, 0x00, 0x90, 0x00, 0x04, 0x0F,
    0xE0, 0x80, 0x07, 0xF8, 0x80, 0x80, 0x08, 0x01, 0x90, 0x02, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x20, 0x0F, 0xF0, 0x04, 0x23, 0xD4, 0x00, 0x00,
    0x00, 0x01, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x4F,
    0xF0, 0x08, 0x0B, 0xE0, 0x20, 0x00, 0x40, 0x0A, 0x00, 0x08, 0x00, 0x08,
    0x00, 0x22, 0x00, 0x04, 0x10, 0x0F, 0xF0, 0x00, 0x97, 0x10, 0x41, 0x00,
    0x00, 0x05, 0x00, 0x20, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x0F,
    0xD0, 0x41, 0x3F, 0xA0, 0x00, 0x00, 0x00, 0x08, 0x00, 0x80, 0x03, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x0F, 0xF0, 0x02, 0x5D, 0x41, 0x00, 0x04,
    0x10, 0x40, 0x00, 0x00, 0x08, 0x00, 0x40, 0x00, 0x08, 0x10, 0x00, 0x8F,
    0xF0, 0x04, 0xB6, 0x00, 0x00, 0x00, 0x41, 0x02, 0x10, 0x00, 0x20, 0x00,
    0x00, 0x80, 0x20, 0x00, 0x00, 0x0F, 0xF0, 0x09, 0x68, 0x00, 0x02, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x02, 0x00, 0x00, 0x04, 0x0F,
    0xF1, 0x02, 0xB2, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x0F, 0xD0, 0x05, 0xA4, 0x04, 0x40, 0x00,
    0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x4F,
    0xF0, 0x4A, 0x00, 0x10, 0x00, 0x00, 0x40, 0x04, 0x20, 0x10, 0x00, 0x80,
    0x08, 0x20, 0x00, 0x22, 0x00, 0x0F, 0xF0, 0x20, 0x00, 0x00, 0x00, 0x02,
    0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
    0xF0, 0x05, 0x00, 0x40, 0x04, 0x10, 0x02, 0x00, 0x00, 0x00, 0x10, 0x04,
    0x00, 0x00, 0x20, 0x80, 0x08, 0x0F, 0xE2, 0x09, 0x11, 0x00, 0x00, 0x40,
    0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x80, 0x00, 0x20, 0x02, 0x0F,
    0xF0, 0x24, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x20,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x2F, 0xF0, 0x10, 0x00, 0x02, 0x08, 0x00,
    0x0C, 0x00, 0x80, 0x00, 0x81, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x0F,
    0xE0, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x02, 0x02, 0x00, 0x00, 0x0F, 0xF0, 0x80, 0x04, 0x00, 0x00, 0x00,
    0x40, 0x40, 0x00, 0x20, 0x04, 0x00, 0x81, 0x00, 0x00, 0x04, 0x00, 0x8F,
    0xE1, 0x00, 0x80, 0x10, 0x00, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x08, 0x00, 0x00, 0x08, 0x0F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x01, 0x02, 0x00, 0x20, 0x00, 0x00, 0x00, 0x10, 0x00, 0x0F,
    0xF0, 0x00, 0x00, 0x40, 0x04, 0x00, 0x00, 0x00, 0x08, 0x00, 0x20, 0x02,
    0x04, 0x00, 0x40, 0x40, 0x00, 0x0F, 0xF0, 0x84, 0x10, 0x00, 0x90, 0x00,
    0x00, 0x01, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xC0, 0x2F,
    0xD0, 0x00, 0x00, 0x80, 0x00, 0x20, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x0D, 0xF0, 0x00, 0x02, 0x00, 0x00, 0x21,
    0x00, 0x10, 0x00, 0x10, 0x01, 0x10, 0x10, 0x20, 0x00, 0x00, 0x00, 0x0F,
    0xF0, 0x11, 0x00, 0x02, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x80, 0x00,
    0x00, 0x02, 0x08, 0x00, 0x00, 0x0F, 0xF1, 0x00, 0x00, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x80, 0x00, 0x00, 0x40, 0x10, 0x0F,
    0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x00, 0x02, 0x00, 0x4F, 0xD0, 0x00, 0x10, 0x00, 0x11, 0x05,
    0x10, 0x00, 0x00, 0x82, 0x00, 0x00, 0x03, 0x00, 0x20, 0x00, 0x00, 0x0F,
    0xF0, 0x00, 0x80, 0x80, 0x08, 0x00, 0x00, 0x00, 0x80, 0x08, 0x00, 0x10,
    0x00, 0x00, 0x02, 0x00, 0x01, 0x0F, 0xE0, 0x22, 0x00, 0x04, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x08, 0x02, 0x00, 0x00, 0x84, 0x0F,
    0xF0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x62, 0x00, 0x03, 0x10, 0x00,
    0x20, 0x00, 0x00, 0x08, 0x00, 0x0F, 0xF0, 0x00, 0x02, 0x00, 0x40, 0x20,
    0x80, 0x00, 0x04, 0x00, 0x00, 0x08, 0x00, 0x20, 0x00, 0x00, 0x18, 0x0F,
    0xF2, 0x00, 0x08, 0x10, 0x81, 0x02, 0x04, 0x00, 0x00, 0x40, 0x00, 0x20,
    0x00, 0x00, 0x10, 0x40, 0x00, 0x0F, 0xD0, 0x00, 0x20, 0x00, 0x04, 0x00,
    0x20, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x0F,
    0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x40, 0x04, 0x41, 0x00,
    0x01, 0x02, 0x00, 0x00, 0x00, 0x4F, 0xF0, 0x10, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x82, 0x00, 0x00, 0x04, 0x00, 0x40, 0x08, 0x01, 0x04, 0x42, 0x0F,
    0xE0, 0x40, 0x00, 0x44, 0x80, 0x10, 0x02, 0x00, 0x00, 0x20, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF1, 0x00, 0x01, 0x00, 0x08, 0x40,
    0x10, 0x00, 0x00, 0x80, 0x00, 0x10, 0x04, 0x00, 0x00, 0x00, 0x08, 0x0F,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
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

const int spriteWidth = 32;
const int spriteHeight = 32;
const int numFrames = 3;

// ::SpriteSheet spriteSheet = ::SpriteSheet(spriteSheetData, 96, 32, 3);
::SpriteSheet spriteSheet = ::SpriteSheet(spriteSheetData,spriteWidth*numFrames, spriteHeight, numFrames);

const uint8_t *frames[] = {};
Sprite ship(frames,numFrames,spriteWidth,spriteHeight);
// Sprite ship(frames,3,32,32);


typedef enum : uint16_t {
  START_SCREEN = Kywy::Events::USER_EVENTS,
  ASTEROID_COLLISION,
  GAME_OVER
} AsteroidBlasterSignal;

::Actor::Message asteroidCollisionMessage = ::Actor::Message(ASTEROID_COLLISION);
::Actor::Message gameOverMessage = ::Actor::Message(GAME_OVER);

class ShipManager : public Actor::Actor {
public:
  int xPosition = 84;
  int yPosition = 72;

  const float acceleration = 0.1f;
  const float friction = 0.99f;

  float shipAngle = 0.0f;
  const float rotationSpeed = 5.0f;

  float xVelocity = 0.0f;
  float yVelocity = 0.0f;

  int padding = 5;


  int currentFrame = 0; 
  const int totalFrames = 3;

  int frameDelay = 5; //5 ideal
  int frameCounter = 0;

  bool buttonLeftPressed = false;
  bool buttonRightPressed = false;
  bool buttonUpPressed = false;
  bool buttonDownPressed = false;

  const char *getName() { return "spriteManager"; };

  void initialize() {
    ship.setPosition(xPosition, yPosition);
    ship.setVisible(true);
    ship.setNegative(true);
  }

  void handle(::Actor::Message *message) {
    switch (message->signal) {

      case Kywy::Events::D_PAD_LEFT_PRESSED:
        buttonLeftPressed = true;
        break;

      case Kywy::Events::D_PAD_RIGHT_PRESSED:
        buttonRightPressed = true;
        break;

      case Kywy::Events::D_PAD_UP_PRESSED:
        buttonUpPressed = true;
        break;

      case Kywy::Events::D_PAD_DOWN_PRESSED:
        buttonDownPressed = true;
        break;

      case Kywy::Events::D_PAD_LEFT_RELEASED:
        buttonLeftPressed = false;
        break;

      case Kywy::Events::D_PAD_RIGHT_RELEASED:
        buttonRightPressed = false;
        break;

      case Kywy::Events::D_PAD_UP_RELEASED:
        buttonUpPressed = false;
        break;

      case Kywy::Events::D_PAD_DOWN_RELEASED:
        buttonDownPressed = false;
        break;

      // case Kywy::Events::BUTTON_LEFT_PRESSED:
      //   // engine.display.drawCircle(xPosition,yPosition,10);
      //   break;


      case Kywy::Events::TICK:
      if (buttonUpPressed){
        xVelocity += acceleration * cos(shipAngle * 3.14159265358979323846/180.0f);
        yVelocity += acceleration * -sin(shipAngle * 3.14159265358979323846/180.0f);
      }
      if (buttonRightPressed){
        shipAngle -= rotationSpeed;
        if (shipAngle < 0) shipAngle += 360;
      }
      if (buttonLeftPressed){
        shipAngle += rotationSpeed;
        if (shipAngle >= 360) shipAngle -= 360;
      }

      xPosition += xVelocity;
      yPosition += yVelocity;

      xVelocity *= friction;
      yVelocity *= friction;

      if (xPosition < -1*spriteWidth){
        xPosition = DISPLAY_WIDTH;
      }
      if (xPosition > DISPLAY_WIDTH){
        xPosition = -1*spriteWidth;
        }
      if (yPosition > DISPLAY_HEIGHT){
        yPosition = 0;
        }
      if (yPosition < 0){
        yPosition = DISPLAY_HEIGHT;
        }
      
      frameCounter++;
      //frame rotate
      if (frameCounter >= frameDelay) {
        frameCounter = 0;

      if (buttonRightPressed or buttonUpPressed){ 
        currentFrame = (currentFrame + 1) % totalFrames; 
        ship.setFrame(currentFrame);
        // ship.render();
      }
      if (buttonLeftPressed or buttonDownPressed){ 
        currentFrame = (currentFrame - 1 + totalFrames) % totalFrames; 
        ship.setFrame(currentFrame);
        // ship.render();
      }
      }
      ship.setPosition(xPosition,yPosition);
      engine.display.clear();
      
      // engine.display.drawLine(xPosition,yPosition,-10.0,(shipAngle+15)*M_PI/180.0f);
      // engine.display.drawLine(xPosition,yPosition,+10.0,(shipAngle+15)*M_PI/180.0f);
      // engine.display.drawLine(xPosition,yPosition+5,-10.0,(shipAngle-15)*M_PI/180.0f);
      // engine.display.drawLine(xPosition,yPosition+5,+10.0,(shipAngle-15)*M_PI/180.0f);
      // engine.display.fillCircle(xPosition, yPosition, 5);
      engine.display.drawLine(xPosition,yPosition,-20.0,(shipAngle-15)*M_PI/180.0f);
      engine.display.drawLine(xPosition,yPosition,-20.0,(shipAngle+15)*M_PI/180.0f);


//Debug draw angle and vels 
      // char msg[16];
      // snprintf(msg, sizeof(msg), "%.2f", shipAngle);
      // engine.display.drawText(5, 5, msg, Display::TextOptions().color(BLACK));

      // char msg2[16];
      // snprintf(msg2, sizeof(msg2), "%.2f", xVelocity);
      // engine.display.drawText(5, 15, msg2, Display::TextOptions().color(BLACK));

      // char msg3[16];
      // snprintf(msg3, sizeof(msg3), "%.2f", yVelocity);
      // engine.display.drawText(5, 25, msg3, Display::TextOptions().color(BLACK));

      engine.display.update();
      // ship.render();
      break;

    }
  }
} shipManager;

typedef struct Asteroid {
  int x, y, radius,xVel,yVel;
} Asteroid;

class AsteroidManager : public Actor::Actor {
public:
  const char *getName() { return "platformManager"; };

  const static int numAsteroids = 6;

  int xVelocityMax = 2;
  int yVelocityMax = 2;
  //Casting vel into int so speed is either 1 or 2. Need to fix this but anything above 2 is too hard to dodge
  int radius = 3;

  Asteroid asteroids[numAsteroids];

  void initialize() {
    for (int i = 0; i < numAsteroids; i++) {
      asteroids[i] = Asteroid{
            .x = (int)random(DISPLAY_WIDTH),
            .y = (int)random(DISPLAY_HEIGHT),
            .radius = radius,
            .xVel = random(-xVelocityMax,xVelocityMax),
            .yVel = random(-yVelocityMax,yVelocityMax)};
    }
    
  }
  void drawAsteroids() {
    for (int i = 0; i < numAsteroids; i++) {
      if (asteroids[i].y >= 0 && asteroids[i].y <= DISPLAY_HEIGHT) {
        engine.display.drawCircle(asteroids[i].x, asteroids[i].y, 2 * asteroids[i].radius);
      }
    }
  }

  void handle(::Actor::Message *message) {
    switch (message->signal) {
    case Kywy::Events::TICK:

    // static int tickCounter = 0;
    // tickCounter++;

    // if (tickCounter % 2 == 0) { //Not the most elegant solution, makes the frames choppy
    //   for (int i = 0; i < numAsteroids; i++) {
    //     asteroids[i].x += asteroids[i].xVel;
    //     asteroids[i].y += asteroids[i].yVel;

    //     if (asteroids[i].x < 0){
    //       asteroids[i].x = DISPLAY_WIDTH;
    //     }
    //     if (asteroids[i].x > DISPLAY_WIDTH){
    //       asteroids[i].x = 0;
    //     }
    //     if (asteroids[i].y < 0){
    //       asteroids[i].y = DISPLAY_HEIGHT;
    //     }
    //     if (asteroids[i].y > DISPLAY_HEIGHT){
    //       asteroids[i].y = 0;
    //     }
    //   }
    // }

    //Move asteroids
    for (int i = 0; i < numAsteroids; i++) {
      asteroids[i].x += asteroids[i].xVel;
      asteroids[i].y += asteroids[i].yVel;

      if (asteroids[i].x < 0){
        asteroids[i].x = DISPLAY_WIDTH;
      }
      if (asteroids[i].x > DISPLAY_WIDTH){
        asteroids[i].x = 0;
      }
      if (asteroids[i].y < 0){
        asteroids[i].y = DISPLAY_HEIGHT;
      }
      if (asteroids[i].y > DISPLAY_HEIGHT){
        asteroids[i].y = 0;
      }
    }
    //Collision
    for (int i = 0; i < numAsteroids; i++) {
      if((abs(asteroids[i].x-ship.x) < asteroids[i].radius) or(abs(asteroids[i].y-ship.y) < asteroids[i].radius)){
        publish(&gameOverMessage);
      }
    }


    drawAsteroids();
    }
  }
} asteroidManager;

class GameManager : public Actor::Actor {
public:
  const char *getName() { return "gameManager"; };

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
    case START_SCREEN: {
      shipManager.unsubscribe(&engine.clock);
      asteroidManager.unsubscribe(&engine.clock);
      unsubscribe(&engine.clock);
      engine.display.drawBitmap(0, 0, 144, 168, (uint8_t *)asteroidSplashScreen);
      engine.display.update();
      subscribe(&engine.input);
      break;
    }

    case Kywy::Events::TICK:
      // drawScore(BLACK);
      engine.display.update();
      break;
    case GAME_OVER: {
      shipManager.unsubscribe(&engine.clock);
      asteroidManager.unsubscribe(&engine.clock);
      unsubscribe(&engine.clock);
      subscribe(&engine.input);

      if (score > highScore) highScore = score;

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
    case Kywy::Events::INPUT_PRESSED: {
      unsubscribe(&engine.input);

      asteroidManager.initialize();
      ship.setPosition(84, 72);

      score = 0;

      engine.display.clear();

      shipManager.subscribe(&engine.clock);
      asteroidManager.subscribe(&engine.clock);
      subscribe(&engine.clock);
      break;
    }
    }
  }
} gameManager;

void setup() {
  engine.start();

  spriteSheet.addFrames(0, 0, spriteWidth, spriteHeight, numFrames);
  ship.frames = spriteSheet.frames;
  ship.setDisplay(&engine.display);

  shipManager.subscribe(&engine.input);
  shipManager.subscribe(&engine.clock);
  shipManager.subscribe(&asteroidManager);
  shipManager.start();

  asteroidManager.subscribe(&engine.clock);
  asteroidManager.start();

  gameManager.subscribe(&shipManager);
  gameManager.subscribe(&engine.clock);
  gameManager.start();

  ::Actor::Message message(START_SCREEN);
  gameManager.handle(&message);
}

void loop() {
  delay(1000);
}
