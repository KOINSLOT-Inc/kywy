// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "kywy/SpriteSheet.hpp"

SpriteSheet::SpriteSheet(const uint8_t *sheet, int8_t sheetWidth, int8_t sheetHeight, uint16_t numFrames)
    : sheet(sheet), sheetWidth(sheetWidth), sheetHeight(sheetHeight), numFrames(numFrames) {
  frames = new const uint8_t *[numFrames];
}

SpriteSheet::~SpriteSheet() {
  for (int i = 0; i < numFrames; i++)
    delete[] frames[i];
  delete[] frames;
}

void SpriteSheet::addFrame(uint16_t x, uint16_t y, uint16_t frameWidth, uint16_t frameHeight) {
  // TODO: this'll probably break if (width * height) isn't divisible by 8
  uint8_t bytes = frameWidth * frameHeight / 8;
  uint8_t *frame = new uint8_t[bytes];

  // create a cursor to track where in the array we want to pull from since
  // we're mapping a 2D sprite sheet to a 1D array which means we'll have to
  // jump around a lot.
  int cursor = (sheetWidth * y + x) / 8; // start in the top left of the frame
  uint16_t cursorX = 0;

  for (int i = 0; i < bytes; i++) {
    frame[i] = sheet[cursor];

    cursor++;
    cursorX++;

    // if we have more than one frame in a horizontal row we have to skip
    // forward in the array past the frames to the right of the one we want to
    // add
    if (cursorX >= (frameWidth / 8)) {
      cursorX = 0;
      cursor += (sheetWidth - frameWidth) / 8;
    }
  }

  frames[framesAdded] = frame;
  framesAdded++;
}

void SpriteSheet::addFrames(uint16_t x, uint16_t y, uint16_t frameWidth, uint16_t frameHeight, uint16_t numFrames) {
  uint16_t cursorX = x, cursorY = y;
  for (int i = 0; i < numFrames; i++) {
    addFrame(cursorX, cursorY, frameWidth, frameHeight);

    uint16_t newCursorX = (cursorX + frameWidth) % sheetWidth;

    if (newCursorX <= cursorX) // wrap around to next line of frames
      cursorY += frameHeight;

    cursorX = newCursorX;
  }
}
