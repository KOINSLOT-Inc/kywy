---
title: "Part 2: Understanding the Game Structure"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 2: Understanding the Game Structure

Now that you've seen the Adventure game in action, let's dive into how it works. We'll explore the main components and learn how they fit together.

## The Main Loop

The heart of any Arduino program is the `loop()` function. Here's how the Adventure game is structured:

```cpp
void loop() {
  // Show start screen
  engine.display.clear();
  engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen);
  drawText();
  engine.display.update();

  // Wait for input
  ButtonEvent choice = waitForInput();

  if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
    // Show left screen
    engine.display.clear();
    engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, leftScreen);
    drawBottomText("You found a reward!");
    engine.display.update();
  } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
    // Show right screen
    engine.display.clear();
    engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, rightScreen);
    drawBottomText("You are attacked!");
    engine.display.update();
  }

  waitForInput();
  // Loop back to start
}
```

## Key Components

### 1. Screen Management
- `engine.display.clear()` - Clears the screen for new content
- `engine.display.drawBitmap()` - Draws background images
- `engine.display.update()` - Shows all changes on screen

### 2. Input Handling
- `waitForInput()` - Waits for player input and returns which button was pressed
- Uses d-pad left/right for choices

### 3. Text Display
- `drawText()` - Shows the choice options on the start screen
- `drawBottomText()` - Shows story results on choice screens

## Helper Functions

The game uses several helper functions to keep the code organized:

### `waitForInput()`
```cpp
ButtonEvent waitForInput() {
  ButtonEvent pressed = Kywy::Events::KywyEvents::INPUT;
  // ... input detection logic ...
  return pressed;
}
```

### `drawBottomText()`
```cpp
void drawBottomText(const char* text) {
  // ... text drawing logic ...
}
```

## Your Task: Modify the Story Text

Try changing the story text that appears when you make choices. In the `drawBottomText()` calls, replace the current messages with your own creative story outcomes.

For example:
- Change "You found a reward!" to "You discovered a hidden treasure!"
- Change "You are attacked!" to "A mysterious figure approaches!"

Upload your changes and test them on your Kywy device. How does changing the story text affect the gameplay experience?
