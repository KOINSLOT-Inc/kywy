// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <math.h>

#include "kywy/Sprite.hpp"

Sprite::~Sprite() { delete[] frames; };

Sprite::Sprite(const uint8_t *frames[], Display::Bitmap::BitmapFormat format, uint16_t numFrames, int16_t width,
               int16_t height)
    : width(width), height(height), format(format) {
  this->numFrames = numFrames;
  this->frames = new const uint8_t *[numFrames];
  for (uint16_t i = 0; i < numFrames; i++) {
    this->frames[i] = frames[i];
  }
};

void Sprite::setFrame(uint16_t frame) { this->frame = frame; }

void Sprite::draw() {
  display->drawBitmap(Display::Origin::Object2D::TOP_LEFT, x, y, width, height, format, (void *)frames[frame], 0xf,
                      {.transparent = true});
  lastRenderedFrame = frame;
}

void Sprite::erase(int16_t lastRenderedX, int16_t lastRenderedY) {
  switch (format) {
  case Display::Bitmap::MONOCHROME:
    display->drawBitmap(Display::Origin::Object2D::TOP_LEFT, lastRenderedX, lastRenderedY, width, height, format,
                        (void *)frames[lastRenderedFrame], 0x0, {.transparent = true});
    break;
  case Display::Bitmap::GRAYSCALE_4_BIT:
    display->drawBitmap(Display::Origin::Object2D::TOP_LEFT, lastRenderedX, lastRenderedY, width, height, format,
                        (void *)frames[lastRenderedFrame], 0x0, {.transparent = true, .erase = true});
    break;
  }
}

void Sprite::translate(int16_t x, int16_t y) { this->setPosition(this->x + x, this->y + y); };

void Sprite::translate(int16_t x, int16_t y, int16_t distance) {
  float scale = distance / sqrt(x * x + y * y);
  this->setPosition(this->x + (x * scale), this->y + (y * scale));
};

void Sprite::incrementFrame() { this->setFrame(this->frame + 1); };

void Sprite::decrementFrame() { this->setFrame(this->frame - 1); };

void Sprite::advanceFrame(uint16_t startFrame, uint16_t endFrame, uint16_t ticksPerFrame, FrameLoopMode mode) {
  if (this->ticksSinceLastAdvance != ticksPerFrame) {
    this->ticksSinceLastAdvance++;
    return;
  } else {
    this->ticksSinceLastAdvance = 1;
  }

  if (this->frame < startFrame || this->frame > endFrame) {
    this->setFrame(startFrame);
    return;
  }

  switch (mode) {
  case FrameLoopMode::PERIODIC:
    if (this->frame == endFrame) {
      this->setFrame(startFrame);
    } else {
      this->incrementFrame();
    }
    return;
  case FrameLoopMode::BOOMERANG:
    if (this->frame == startFrame) {
      this->boomerangReverseDirection = false;
      this->incrementFrame();
    } else if (this->frame == endFrame) {
      this->boomerangReverseDirection = true;
      this->decrementFrame();
    } else if (this->boomerangReverseDirection) {
      this->decrementFrame();
    } else {
      this->incrementFrame();
    }
    return;
  }
};
