---
title: "Part 2: Adding More Choices to Your Story"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 2: Adding More Choices to Your Story

Now that you understand the basic structure, let's add one more choice to your adventure. We'll keep it simple - just add a second choice after your first one.

## Adding a Second Choice

Your current game has one choice. Let's add a second choice that comes right after:

```cpp
void loop() {
  // Show start screen
  engine.display.clear();
  engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen);
  drawInstructionText();
  engine.display.update();

  ButtonEvent choice = waitForInput();

  if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
    // Show left screen
    engine.display.clear();
    engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, leftScreen);
    score += 10;  // Good choice gives points
    drawBottomText("You found treasure! (+10 points)");
  } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
    // Show right screen
    engine.display.clear();
    engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, rightScreen);
    health -= 10;  // Bad choice hurts you
    drawBottomText("You were attacked! (-10 health)");
  }

  // SECOND CHOICE - Add this after your first choice
  //Here we can add an image to display (optional)
  //engine.display.clear();
  //engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, YOUR_SCREEN_NAME); // Replace YOUR_SCREEN_NAME with your image
  drawInstructionText();
  drawBottomText("You find a split path") // replace with some interesting story
  //engine.display.update();

  choice = waitForInput(); // wait for the user to press a button

  if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
    // Second good choice
    engine.display.clear();

    score += 15;  // More points for second good choice
    drawBottomText("Your text goes here");
  } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
    // Second bad choice
    engine.display.clear();
    health -= 15;  // More damage for second bad choice
    drawBottomText("Your text goes here");
  }

  // Loop back to start
}
```

## What This Does

**First Choice:**
- **Left (Good)**: +10 points, shows left screen
- **Right (Bad)**: -10 health, shows right screen

**Second Choice:**
- **Left (Good)**: +15 points, shows green success screen
- **Right (Bad)**: -15 health, shows red danger screen

## Your Challenge

1. **Add the second choice** after your existing first choice
2. **Test both paths**:
   - Left → Left: Should give +25 total points
   - Left → Right: Should give +10 points, -15 health
   - Right → Left: Should give -10 health, +15 points
   - Right → Right: Should give -25 total health

**Questions to consider:**
- How do your score and health change with different combinations?
- Which sequence gives the best results?
- Which sequence gives the worst results?

In the next part, we'll add win/lose conditions based on your final score and health!
