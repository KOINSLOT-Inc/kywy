// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "esp_types.h"

class SpriteSheet {
public:
  SpriteSheet(const uint8_t *sheet, int8_t sheetWidth, int8_t sheetHeight, uint16_t numFrames);
  ~SpriteSheet();

  // Specifies a frame on the sprite sheet
  void addFrame(uint16_t x, uint16_t y, uint16_t frameWidth, uint16_t frameHeight);

  // Specifies multiple frames on the sprite sheet, reads left to right and then
  // top to bottom like a book
  void addFrames(uint16_t x, uint16_t y, uint16_t frameWidth, uint16_t frameHeight, uint16_t numFrames);

  const uint8_t *sheet;
  const uint8_t **frames;

  int8_t sheetWidth, sheetHeight;
  uint16_t numFrames;

private:
  uint16_t framesAdded = 0;
};
