// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Font.hpp"
#include "Display.hpp"

namespace Display {

namespace Font {

Character::Character(uint8_t *character) {
  code = ((uint16_t)character[CHARACTER_CODE] * 256U) +
         character[CHARACTER_CODE + 1];
  bytes = ((uint16_t)character[CHARACTER_BYTES] * 256U) +
          character[CHARACTER_BYTES + 1];
  deviceWidthX = character[CHARACTER_DEVICE_WIDTH_X];
  deviceWidthY = character[CHARACTER_DEVICE_WIDTH_Y];
  bbxWidth = character[CHARACTER_BBX_WIDTH];
  bbxHeight = character[CHARACTER_BBX_HEIGHT];
  bbxXOffset = character[CHARACTER_BBX_X_OFFSET];
  bbxYOffset = character[CHARACTER_BBX_Y_OFFSET];

  bitmap = character + CHARACTER_BITMAP;
};

Font::Font(uint8_t *font) {
  size = font[FONT_SIZE];
  numCharacters =
      ((uint16_t)font[FONT_CHARACTERS] * 256U) + font[FONT_CHARACTERS + 1];
  boundingBoxWidth = font[FONT_BOUNDING_BOX_WIDTH];
  boundingBoxHeight = font[FONT_BOUNDING_BOX_HEIGHT];
  boundingBoxXOffset = font[FONT_BOUNDING_BOX_X_OFFSET];
  boundingBoxYOffset = font[FONT_BOUNDING_BOX_Y_OFFSET];
  ascent = font[FONT_ASCENT];
  descent = font[FONT_DESCENT];
  firstCharacter = font + FONT_FIRST_CHARACTER;
};

Character Font::getCharacter(uint16_t character) {
  uint8_t *currentCharacter = firstCharacter;

  // first character is always the missing character replacement glyph so skip
  // it
  currentCharacter += 256U * *(currentCharacter + CHARACTER_BYTES) +
                      *(currentCharacter + CHARACTER_BYTES + 1);

  uint16_t charactersLeft = numCharacters;

  while (256U * *(currentCharacter) + *(currentCharacter + 1) != character) {
    charactersLeft--;
    currentCharacter += 256U * *(currentCharacter + CHARACTER_BYTES) +
                        *(currentCharacter + CHARACTER_BYTES + 1);

    if (charactersLeft <= 0) { // if we didn't find the character use the
                               // missing character replacement glyph
      printf("Char not found! '%#x'\n", character);
      return Character(firstCharacter);
    }
  }

  return Character(currentCharacter);
};

} // namespace Font

uint16_t Display::readUTF8Char(char *&string) {
  uint16_t firstByte = string[0];

  // end of string
  if (!firstByte) {
    return 0;
  }

  // three byte character sequence, U+0800 - U+FFFF
  if ((0b11110000 & firstByte) == 0b11100000) {
    uint16_t secondByte = string[1];
    uint16_t thirdByte = string[2];
    string += 3;

    // extract the 16 bits (x's) embedded in '1110xxxx 10xxxxxx 10xxxxxx'
    return (firstByte * 4096U) | ((secondByte & 0b00111111) * 64U) |
           (thirdByte & 0b00111111);
  }

  // two byte character sequence, U+0080 - U+07FF
  if ((0b11100000 & firstByte) == 0b11000000) {
    uint16_t secondByte = string[1];
    string += 2;

    // extract the 11 bits (x's) embedded in '110xxxxx 10xxxxxx'
    return ((firstByte & 0b00011111) * 64U) | (secondByte & 0b00111111);
  }
  string += 1;

  // one byte character sequence, U+0000 - U+007F
  return firstByte;
};

void Display::getTextSize(uint8_t *font, char *text, uint16_t &width,
                          uint16_t &height, int16_t &originXOffset,
                          int16_t &originYOffset, uint16_t &baselineLength) {
  width = 0;
  height = 0;
  baselineLength = 0;

  uint16_t maxAscent = 0, maxDescent = 0;

  Font::Font fontObject = Font::Font(font);

  // if the first character has a negative BBX X Offset we need to add it to the
  // width
  bool firstCharacter = true;

  uint16_t currentCharCode;
  Font::Character currentChar = Font::Character();

  while ((currentCharCode = readUTF8Char(text))) {
    currentChar = fontObject.getCharacter(currentCharCode);

    width += currentChar.deviceWidthX;
    baselineLength += currentChar.deviceWidthX;

    // Starting to the left or right of the origin needs to be factored in for
    // first character. Middle characters are just measured by the device width.
    if (firstCharacter) {
      firstCharacter = false;

      width -= currentChar.bbxXOffset;
      originXOffset = -1 * currentChar.bbxXOffset;
    }

    uint16_t ascent = currentChar.bbxHeight + currentChar.bbxYOffset;
    maxAscent = ascent > maxAscent ? ascent : maxAscent;

    uint16_t descent =
        currentChar.bbxYOffset < 0 ? -1 * currentChar.bbxYOffset : 0;
    maxDescent = descent > maxDescent ? descent : maxDescent;

    height = maxAscent + maxDescent;
  }

  // the device width often extends beyond the BBX, so for the last char we need
  // to calculate the added width based on the BBX instead
  width -= currentChar.deviceWidthX; // undo the last operation
  width += currentChar.bbxWidth +
           currentChar.bbxXOffset; // add width and account for offset

  originYOffset = maxAscent - 1;
};

void Display::setFont(uint8_t *font) { defaultFont = font; }

void Display::getTextSize(char *text, uint16_t &width, uint16_t &height,
                          TextOptions options) {
  if (options.getFont() == nullptr) {
    options = options.font(defaultFont);
  }

  int16_t originXOffset, originYOffset;
  uint16_t baselineLength;
  getTextSize(options.getFont(), text, width, height, originXOffset,
              originYOffset, baselineLength);
};

void Display::drawText(int16_t x, int16_t y, char *text, TextOptions options) {
  if (options.getFont() == nullptr) {
    options = options.font(defaultFont);
  }

  Font::Font fontObject(options.getFont());

  uint16_t width = 0, height = 0, baselineLength = 0;
  int16_t originXOffset = 0, originYOffset = 0;
  getTextSize(options.getFont(), text, width, height, originXOffset,
              originYOffset, baselineLength);

  int16_t originX = x, originY = y;

  switch (options.getOrigin()) {
  case Origin::Text::TOP_LEFT:
    originX = x + originXOffset;
    originY = y + originYOffset;
    break;
  case Origin::Text::TOP_RIGHT:
    originX = x - (height - 1) + originXOffset;
    originY = y + originYOffset;
    break;
  case Origin::Text::BOTTOM_LEFT:
    originX = x + originXOffset;
    originY = y - (height - 1) + originYOffset;
    break;
  case Origin::Text::BOTTOM_RIGHT:
    originX = x - (height - 1) + originXOffset;
    originY = y - (height - 1) + originYOffset;
    break;
  case Origin::Text::CENTER:
    originX = x - (width / 2) + originXOffset;
    originY = y - (height / 2) + originYOffset;
    break;
  case Origin::Text::BASELINE_LEFT:
    originX = x;
    originY = y;
    break;
  case Origin::Text::BASELINE_CENTER:
    originX = x - (baselineLength / 2);
    originY = y;
    break;
  case Origin::Text::BASELINE_RIGHT:
    originX = x - baselineLength;
    originY = y;
    break;
  }

  // we need to draw text in transparent mode so that diacritical marks aren't
  // overridden, to simulate non-transparent text we instead draw a white
  // rectangle over the area the text covers
  if (options.getOpaque()) {
    fillRectangle(originX - originXOffset, originY - originYOffset, width,
                  height,
                  Object2DOptions().color(options.getColor() ? 0x00 : 0xff));
  }

  uint16_t currentCharCode;
  Font::Character currentChar = Font::Character();

  while ((currentCharCode = readUTF8Char(text))) {
    currentChar = fontObject.getCharacter(currentCharCode);
    drawBitmap(originX + currentChar.bbxXOffset,
               originY - currentChar.bbxYOffset,
               currentChar.bbxWidth + (8 - currentChar.bbxWidth % 8),
               currentChar.bbxHeight, currentChar.bitmap,
               BitmapOptions()
                   .opaque(options.getOpaque())
                   .negative(options.getColor() ? false : true)
                   .origin(Origin::Object2D::BOTTOM_LEFT));

    originX += currentChar.deviceWidthX;
  }
};

} // namespace Display
