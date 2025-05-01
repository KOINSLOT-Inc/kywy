// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <math.h>
#include <stdint.h>

#include "Sprite.hpp"

Sprite::~Sprite() {
  delete[] frames;
};

// Sprite::Sprite(const uint8_t *frames[], uint16_t numFrames, int16_t width,
//                int16_t height)
//   : width(width), height(height) {
//   this->numFrames = numFrames;
//   this->frames = new const uint8_t *[numFrames];
//   for (uint16_t i = 0; i < numFrames; i++) {
//     this->frames[i] = frames[i];
//   }
// };

Sprite::Sprite(const uint8_t *frames[], uint16_t numFrames, int16_t width,
               int16_t height)
  : width(width), height(height) {
  this->numFrames = numFrames;

  int new_width = static_cast<int>(std::ceil(std::sqrt(2) * width));
  int new_height = static_cast<int>(std::ceil(std::sqrt(2) * height));

  if (new_width % 8 != 0)
    new_width += 8 - (new_width % 8);  // Ensure multiple of 8

  if (new_height % 8 != 0)
    new_height += 8 - (new_height % 8);  // Ensure multiple of 8

  int old_bytes_per_row = (width + 7) / 8;
  int new_bytes_per_row = new_width / 8;

  int left_pad_pixels = (new_width - width) / 2;
  int top_pad_rows = (new_height - height) / 2;
  int byte_offset = left_pad_pixels / 8;

  this->width = new_width;
  this->height = new_height;
  this->frames = new const uint8_t *[numFrames];

  for (uint16_t f = 0; f < numFrames; f++) {
    uint8_t *new_frame = new uint8_t[new_bytes_per_row * new_height];

    for (int i = 0; i < new_bytes_per_row * new_height; ++i) {
      new_frame[i] = 0;
    }

    // Add a white border (set border bits to 1)
    // for (int y = 0; y < new_height; ++y) {
    //   for (int x = 0; x < new_width; ++x) {
    //     if (y == 0 || y == new_height - 1 || x == 0 || x == new_width - 1) {
    //       int byte_index = y * new_bytes_per_row + x / 8;
    //       int bit_offset = 7 - (x % 8);
    //       new_frame[byte_index] |= (1 << bit_offset);
    //     }
    //   }
    // }

    for (int y = 0; y < height; ++y) {
      uint8_t *dst_row = new_frame + (y + top_pad_rows) * new_bytes_per_row + byte_offset;
      const uint8_t *src_row = frames[f] + y * old_bytes_per_row;

      for (int b = 0; b < old_bytes_per_row; ++b) {
        dst_row[b] = src_row[b];
      }
    }

    this->frames[f] = new_frame;
  }
}

void Sprite::debugPrintBitmapInfo() const {
  uint16_t byteWidth = width / 8;
  uint16_t totalBytes = byteWidth * height;
  Serial.print("Width: ");
  Serial.println(width);
  Serial.print("Height: ");
  Serial.println(height);
  Serial.print("Byte width: ");
  Serial.println(byteWidth);
  Serial.print("Total bytes: ");
  Serial.println(totalBytes);
}

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
