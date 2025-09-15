---
title: "Part 5: Advanced Features and Polish"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 5: Adding Custom Sprites with the HPP Generator

Congratulations! You've mastered game loops, win/lose states, and nested if statements. Now let's add visual polish by creating your own custom sprites using the online HPP generator tool.

## What is the HPP Generator?

The HPP generator is an online tool that converts images into C++ header files that can be used in your Kywy games. It takes any image and converts it into the format your game needs.

For detailed instructions on how to create HPP files from images, see the [Image Generation Guide](../guides/images.md).

Once you have your HPP file, here's how to use it in your game:

### Step 1: Include Your File

Add this at the top of your `Adventure.ino` file:

```cpp
// Include your generated sprite file
#include "my_image.hpp"
```
You can replace my_image.hpp with what you named your image. Make sure each one is unique!

### Step 2: Use Your Sprite in the Game

The HPP file will contain:
- `my_image_data[]` - The sprite image data
- `MY_IMAGE_WIDTH` - The sprite width
- `MY_IMAGE_HEIGHT` - The sprite height

Use it in your game like this:

```cpp
void loop() {
  // Show your screen with your custom sprite
  engine.display.clear();
  engine.display.drawBitmap(50, 100, MY_HERO_WIDTH, MY_HERO_HEIGHT, MY_IMAGE_DATA); // Replace with your image
  ButtonEvent choice = waitForInput(); // Wait for input and store it in choice
  // ... rest of your game logic
}
```

## Use sprites in your nested if statements:

```cpp
void loop() {
  if (gameState == 0) {
    // Draw hero on start screen
    engine.display.clear();
    engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen);
    drawHeroBasedOnHealth(50, 100); // Position hero on screen
    drawInstructionText();
    engine.display.update();

    // ... rest of start logic

  } else if (gameState == 1) {
    // Different hero appearance based on choices
    engine.display.clear();
    drawColoredScreen(0x07E0);

    if (score > 30) {
      // Confident hero sprite
      engine.display.drawBitmap(50, 100, HERO_WIDTH, HERO_HEIGHT, heroConfident_data);
      drawBottomText("Your hero looks determined!\nLeft: Fight\nRight: Negotiate");
    } else {
      // Nervous hero sprite
      engine.display.drawBitmap(50, 100, HERO_WIDTH, HERO_HEIGHT, heroNervous_data);
      drawBottomText("Your hero looks uncertain...\nLeft: Fight\nRight: Run");
    }

    engine.display.update();
    // ... choice handling
  }
}
```
The sky's the limit! You can now create your own rich, interactive stories with unique visuals.