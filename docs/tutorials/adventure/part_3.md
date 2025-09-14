---
title: "Part 3: Adding New Choices and Screens"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 3: Adding New Choices and Screens

Now that you understand the basic structure, let's learn how to extend the game with new choices and screens. This is where the real fun begins!

## Adding a New Choice

Currently, the game only has left and right choices. Let's add an "up" choice using the d-pad up button.

### Step 1: Add the Choice Handler

Find the `if/else` statements in the `loop()` function and add a new condition:

```cpp
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
} else if (choice == Kywy::Events::KywyEvents::D_PAD_UP_PRESSED) {
  // NEW: Show up screen
  engine.display.clear();
  engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen); // Reuse start screen for now
  drawBottomText("You climbed to higher ground!");
  engine.display.update();
}
```

### Step 2: Update the Input Function

Make sure the `waitForInput()` function can detect the up button:

```cpp
if (leftPressed && !wasLeftPressed) {
  pressed = Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED;
} else if (rightPressed && !wasRightPressed) {
  pressed = Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED;
} else if (upPressed && !wasUpPressed) {  // Add this
  pressed = Kywy::Events::KywyEvents::D_PAD_UP_PRESSED;
}
```

### Step 3: Update the Start Screen Text

Modify the `drawText()` function to mention the new choice:

```cpp
// In drawText() function:
engine.display.drawText(centerX + 10, centerY - 30, "Go Right");
engine.display.drawText(centerX - 60, centerY - 20, "Go Left");
engine.display.drawText(centerX - 40, centerY + 10, "Go Up");  // Add this line
```

## Adding New Screens

To add truly unique screens, you'll need to create new bitmap images. For now, you can reuse existing screens or create simple colored backgrounds.

### Creating a Simple Colored Screen

```cpp
// Add this function for a colored screen
void drawColoredScreen(uint16_t color) {
  engine.display.clear();
  engine.display.fillRectangle(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, Display::Object2DOptions().color(color));
}

// Use it in your choice:
} else if (choice == Kywy::Events::KywyEvents::D_PAD_UP_PRESSED) {
  drawColoredScreen(0x07E0); // Green screen
  drawBottomText("You found a peaceful meadow!");
  engine.display.update();
}
```

## Your Challenge

1. **Add an "up" choice** that shows a different outcome
2. **Add a "down" choice** with another unique result
3. **Create different colored backgrounds** for each choice
4. **Experiment with different story messages**

Try these modifications and see how they change the gameplay. Which choices feel most interesting to you?
