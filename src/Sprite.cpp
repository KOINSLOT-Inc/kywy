// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <math.h>
#include <stdint.h>

#include "Sprite.hpp"

Sprite::~Sprite() {
  delete[] frames;
};

Sprite::Sprite(const uint8_t *frames[], uint16_t numFrames, int16_t width,
               int16_t height)
  : width(width), height(height) {
  this->numFrames = numFrames;
  this->frames = new const uint8_t *[numFrames];
  for (uint16_t i = 0; i < numFrames; i++) {
    this->frames[i] = frames[i];
  }
};

void Sprite::setFrame(uint16_t frame) {
  this->frame = frame;
}

void Sprite::setNegative(bool negative) {
  this->negative = negative;
}
void Sprite::setColor(uint16_t color) {
  this->color = color ? 0xff : 0x00;
}
void Sprite::setRotation(int angle) {
  this->angle = angle;
}


void Sprite::draw() {
  display->drawBitmap(x, y, width, height, (uint8_t *)frames[frame],
                      Display::BitmapOptions().negative(negative).color(color).rotation(angle));
  lastRenderedFrame = frame;
}

void Sprite::erase(int16_t lastRenderedX, int16_t lastRenderedY) {
  display->drawBitmap(lastRenderedX, lastRenderedY, width, height,
                      (uint8_t *)frames[lastRenderedFrame],
                      Display::BitmapOptions().negative(negative).color(!color).rotation(0));
}

void Sprite::translate(int16_t x, int16_t y) {
  this->setPosition(this->x + x, this->y + y);
};

void Sprite::translate(int16_t x, int16_t y, int16_t distance) {
  float scale = distance / sqrt(x * x + y * y);
  this->setPosition(this->x + (x * scale), this->y + (y * scale));
};

void Sprite::incrementFrame() {
  this->setFrame(this->frame + 1);
};

void Sprite::decrementFrame() {
  this->setFrame(this->frame - 1);
};

void Sprite::advanceFrame(uint16_t startFrame, uint16_t endFrame,
                          uint16_t ticksPerFrame, FrameLoopMode mode) {
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
