// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_SPRITE_SHEET
#define KYWY_LIB_SPRITE_SHEET 1

#include "Display.hpp"

#include <stdint.h>

class SpriteSheet {
public:
  SpriteSheet(const uint8_t *sheet, int16_t sheetWidth, int16_t sheetHeight,
              uint16_t numFrames);
  ~SpriteSheet();

  // Specifies a frame on the sprite sheet
  void addFrame(uint16_t x, uint16_t y, uint16_t frameWidth, uint16_t frameHeight);

  // Specifies multiple frames on the sprite sheet, reads left to right and then
  // top to bottom like a book
  void addFrames(uint16_t x, uint16_t y, uint16_t frameWidth, uint16_t frameHeight, uint16_t numFrames);

  const uint8_t *sheet;
  const uint8_t **frames;

  int16_t sheetWidth, sheetHeight;
  uint16_t numFrames;

private:
  uint16_t framesAdded = 0;
};

#endif