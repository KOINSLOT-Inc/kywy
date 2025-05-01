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



void Sprite::rotate(const uint8_t *bitmap, uint8_t *output, int width, int height, double angle) {
  int byteCount = (width * height) / 8;
  for (int i = 0; i < byteCount; ++i) {
    output[i] = 0;
  }

  double cosA = cos(angle * M_PI / 180.0);
  double sinA = sin(angle * M_PI / 180.0);
  double cx = width / 2.0;
  double cy = height / 2.0;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      double dx = x - cx;
      double dy = y - cy;

      double srcX = dx * cosA + dy * sinA + cx;
      double srcY = -dx * sinA + dy * cosA + cy;

      int ix = (int)(srcX + 0.5);
      int iy = (int)(srcY + 0.5);

      if (ix >= 0 && iy >= 0 && ix < width && iy < height) {
        // Annoying bitmapping that was needed
        int srcIndex = iy * width + ix;
        int srcByte = srcIndex / 8;
        int srcBit = 7 - (ix % 8);
        int bit = (bitmap[srcByte] >> srcBit) & 1;

        int dstIndex = y * width + x;
        int dstByte = dstIndex / 8;
        int dstBit = 7 - (x % 8);
        if (bit) {
          output[dstByte] |= (1 << dstBit);
        }
      }
    }
  }
}

void Sprite::setFrame(uint16_t frame) {
  this->frame = frame;
}

void Sprite::setSheet(const uint8_t *newFrames[], uint16_t newNumFrames) {
  delete[] this->frames;
  this->numFrames = newNumFrames;
  this->frames = new const uint8_t *[newNumFrames];
  for (uint16_t i = 0; i < newNumFrames; ++i) {
    this->frames[i] = newFrames[i];
  }
}

void Sprite::setNegative(bool negative) {
  this->negative = negative;
}
void Sprite::setColor(uint16_t color) {
  this->color = color ? 0xff : 0x00;
}

void Sprite::draw() {
  display->drawBitmap(x, y, width, height, (uint8_t *)frames[frame],
                      Display::BitmapOptions().negative(negative).color(color));
  lastRenderedFrame = frame;
}

void Sprite::erase(int16_t lastRenderedX, int16_t lastRenderedY) {
  display->drawBitmap(lastRenderedX, lastRenderedY, width, height,
                      (uint8_t *)frames[lastRenderedFrame],
                      Display::BitmapOptions().negative(negative).color(!color));
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
