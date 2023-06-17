// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "GraphicsObject.hpp"

enum class FrameLoopMode {
  BOOMERANG,
  PERIODIC,
};

class Sprite : public GraphicsObject {
public:
  ~Sprite();

  Sprite(const uint8_t *frames[], uint8_t numFrames, int8_t width, int8_t height);

  void setFrame(uint16_t frame);

  // move sprite to (x, y) coordinate relative to current position
  void translate(int16_t x, int16_t y);
  // move sprite from current position a specified distance in (x, y) direction
  void translate(int16_t x, int16_t y, int16_t distance);

  void incrementFrame();
  void decrementFrame();

  void advanceFrame(uint16_t startFrame, uint16_t endFrame, uint8_t ticksPerFrame, FrameLoopMode mode);

  void (*tick)(Sprite *sprite);

  uint16_t frame = 0;

  int8_t width;
  int8_t height;

  const uint8_t **frames;

protected:
  void draw();
  void erase(int16_t lastRenderedX, int16_t lastRenderedY);

private:
  uint8_t numFrames;

  uint16_t lastRenderedFrame = 0;

  bool boomerangReverseDirection = false;
  uint8_t ticksSinceLastAdvance = 1;
};
