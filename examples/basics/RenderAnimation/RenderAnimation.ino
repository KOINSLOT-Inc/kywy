// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"
#include "my_animation.hpp"  // Include the generated animation data in the same folder

Kywy::Engine engine;

int currentFrame = 0;

// Calculate center position for the animation
int centerX = (144 - MY_ANIMATION_WIDTH) / 2;   // 144 is display width
int centerY = (168 - MY_ANIMATION_HEIGHT) / 2;  // 168 is display height

void setup() {
  engine.start();
}

void loop() {
  // Clear the display to get rid of the last frame
  engine.display.clear();

  // Draw the current frame
  engine.display.drawBitmap(centerX, centerY, MY_ANIMATION_WIDTH, MY_ANIMATION_HEIGHT, (uint8_t *)my_animation_frames[currentFrame]);

  // Update the display
  engine.display.update();

  // Advance to next frame
  currentFrame = (currentFrame + 1) % MY_ANIMATION_FRAME_COUNT;

  // Wait for the next frame
  delay(MY_ANIMATION_SPEED);
}