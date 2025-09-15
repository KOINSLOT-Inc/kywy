---
title: "Part 3: Optional: Adding Win/Lose States"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 3: Adding Win/Lose States

If you want to add simple win/lose conditions to your game, you can check if the player reaches a certain score or if their health gets too low.

## Simple Win Condition

Add this check after your choices to see if the player won:

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

  // CHECK WIN/LOSE CONDITIONS
  if (score >= 10) {
    // WIN! Player reached 10 points
    engine.display.clear();
    drawBottomText("YOU WIN!");
    score = 0; // Reset score for new game
    heath = 100; // Reset health for new game
  } else if (health <= 0) {
    // LOSE! Player's health is too low
    engine.display.clear();
    drawBottomText("GAME OVER!");
    score = 0; // Reset score for new game
    heath = 100; // Reset health for new game
  }
  
  waitForInput();
  // Loop back to start for new game
}
```

## How It Works

**Win Condition:**
- If `score >= 10` → Player wins! Shows green screen with "YOU WIN!"
- This happens when they make good choices that give points

**Lose Condition:**
- If `health <= 0` → Player loses! Shows red screen with "GAME OVER!"
- This happens when they make too many bad choices

**No Condition Met:**
- Game continues normally, loops back to start

## Your Challenge

1. **Add the win/lose checks** after your second choice
2. **Test winning**: Make choices that give you +10 points or more
3. **Test losing**: Make choices that hurt your health until it reaches 0
4. **Try different combinations** to see when you win or lose

**Questions to consider:**
- How many good choices do you need to win?
- How many bad choices before you lose?
- What happens if you don't meet either condition?

This is completely optional - your game works fine without win/lose conditions! In the next part, we'll explore nested if statements for more complex logic.
