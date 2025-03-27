// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_FONT
#define KYWY_LIB_FONT 1

#include <stdint.h>

namespace Display::Font {

#define FONT_SIZE 0
#define FONT_SIZE_TYPE uint8_t
#define FONT_CHARACTERS 1
#define FONT_CHARACTERS_TYPE uint16_t
#define FONT_BOUNDING_BOX_WIDTH 3
#define FONT_BOUNDING_BOX_WIDTH_TYPE uint8_t
#define FONT_BOUNDING_BOX_HEIGHT 4
#define FONT_BOUNDING_BOX_HEIGHT_TYPE uint8_t
#define FONT_BOUNDING_BOX_X_OFFSET 5
#define FONT_BOUNDING_BOX_X_OFFSET_TYPE int8_t
#define FONT_BOUNDING_BOX_Y_OFFSET 6
#define FONT_BOUNDING_BOX_Y_OFFSET_TYPE int8_t
#define FONT_ASCENT 7
#define FONT_ASCENT_TYPE uint8_t
#define FONT_DESCENT 8
#define FONT_DESCENT_TYPE uint8_t
#define FONT_FIRST_CHARACTER 9
#define CHARACTER_CODE 0
#define CHARACTER_CODE_TYPE uint16_t
#define CHARACTER_BYTES 2
#define CHARACTER_BYTES_TYPE uint16_t
#define CHARACTER_DEVICE_WIDTH_X 4
#define CHARACTER_DEVICE_WIDTH_X_TYPE uint8_t
#define CHARACTER_DEVICE_WIDTH_Y 5
#define CHARACTER_DEVICE_WIDTH_Y_TYPE uint8_t
#define CHARACTER_BBX_WIDTH 6
#define CHARACTER_BBX_WIDTH_TYPE uint8_t
#define CHARACTER_BBX_HEIGHT 7
#define CHARACTER_BBX_HEIGHT_TYPE uint8_t
#define CHARACTER_BBX_X_OFFSET 8
#define CHARACTER_BBX_X_OFFSET_TYPE int8_t
#define CHARACTER_BBX_Y_OFFSET 9
#define CHARACTER_BBX_Y_OFFSET_TYPE int8_t
#define CHARACTER_BITMAP 10

class Character {
public:
  CHARACTER_CODE_TYPE code = 0;
  CHARACTER_BYTES_TYPE bytes = 0;
  CHARACTER_DEVICE_WIDTH_X_TYPE deviceWidthX = 0;
  CHARACTER_DEVICE_WIDTH_Y_TYPE deviceWidthY = 0;
  CHARACTER_BBX_WIDTH_TYPE bbxWidth = 0;
  CHARACTER_BBX_HEIGHT_TYPE bbxHeight = 0;
  CHARACTER_BBX_X_OFFSET_TYPE bbxXOffset = 0;
  CHARACTER_BBX_Y_OFFSET_TYPE bbxYOffset = 0;

  uint8_t *bitmap = nullptr;

  Character(uint8_t *character);
  Character(){};
};

class Font {
public:
  FONT_SIZE_TYPE size;
  FONT_CHARACTERS_TYPE numCharacters;
  FONT_BOUNDING_BOX_WIDTH_TYPE boundingBoxWidth;
  FONT_BOUNDING_BOX_HEIGHT_TYPE boundingBoxHeight;
  FONT_BOUNDING_BOX_X_OFFSET_TYPE boundingBoxXOffset;
  FONT_BOUNDING_BOX_Y_OFFSET_TYPE boundingBoxYOffset;
  FONT_ASCENT_TYPE ascent;
  FONT_DESCENT_TYPE descent;

  uint8_t *firstCharacter;

  Font(uint8_t *font);

  Character getCharacter(uint16_t character);
};

} // namespace Display::Font

#endif