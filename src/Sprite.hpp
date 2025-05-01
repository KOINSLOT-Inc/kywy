// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_SPRITE
#define KYWY_LIB_SPRITE 1

#include "GraphicsObject.hpp"

enum class FrameLoopMode {
  BOOMERANG,
  PERIODIC,
};

class Sprite : public GraphicsObject {
public:
  ~Sprite();

  Sprite(const uint8_t *frames[], uint16_t numFrames, int16_t width,
         int16_t height);

  void setFrame(uint16_t frame);

  //Rotate the sprite/sprite sheet a number of degrees
  void rotate(const uint8_t* bitmap, uint8_t* output, int width, int height, double angle);
  //Update the spritesheet of the sprite (need to do after rotation)
  void setSheet(const uint8_t* newFrames[], uint16_t newNumFrames);

  void debugPrintBitmapInfo() const;

  // move sprite to (x, y) coordinate relative to current position
  void translate(int16_t x, int16_t y);
  // move sprite from current position a specified distance in (x, y) direction
  void translate(int16_t x, int16_t y, int16_t distance);

  void incrementFrame();
  void decrementFrame();

  void advanceFrame(uint16_t startFrame, uint16_t endFrame,
                    uint16_t ticksPerFrame, FrameLoopMode mode);

  void (*tick)(Sprite *sprite);

  uint16_t numFrames;
  uint16_t frame = 0;

  int16_t width;
  int16_t height;

  const uint8_t **frames;

  bool negative = false;
  void setNegative(bool negative);

  uint16_t color = 0x00;
  void setColor(uint16_t color);

protected:
  void draw();
  void erase(int16_t lastRenderedX, int16_t lastRenderedY);

private:
  uint16_t lastRenderedFrame = 0;

  bool boomerangReverseDirection = false;
  uint16_t ticksSinceLastAdvance = 1;
};

#endif
