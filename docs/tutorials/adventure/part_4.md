---
title: "Part 4: Creating Complex Story Structures"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 4: Creating Complex Story Structures

Now that you can add basic choices, let's create more complex story structures with multiple levels of choices and branching narratives.

## Nested Choices

Instead of just one choice leading to an ending, let's create choices that lead to more choices. This creates a tree-like story structure.

### Example: The Forest Path

```cpp
// Add this variable at the top of your file
int gameState = 0; // 0 = start, 1 = forest, 2 = cave, etc.

void loop() {
  if (gameState == 0) {
    // Start screen
    engine.display.clear();
    engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen);
    drawText();
    engine.display.update();

    ButtonEvent choice = waitForInput();

    if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
      gameState = 1; // Go to forest
    } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
      gameState = 2; // Go to cave
    }

  } else if (gameState == 1) {
    // Forest screen with new choices
    engine.display.clear();
    drawColoredScreen(0x07E0); // Green forest
    drawBottomText("You enter a mysterious forest.\nLeft: Follow the path\nRight: Climb a tree");
    engine.display.update();

    ButtonEvent choice = waitForInput();

    if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
      gameState = 3; // Follow path
    } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
      gameState = 4; // Climb tree
    }

  } else if (gameState == 2) {
    // Cave screen
    engine.display.clear();
    drawColoredScreen(0x2945); // Dark cave
    drawBottomText("You enter a dark cave.\nLeft: Light a torch\nRight: Feel your way");
    engine.display.update();

    ButtonEvent choice = waitForInput();

    if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
      gameState = 5; // Light torch
    } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
      gameState = 6; // Feel way
    }

  } else if (gameState == 3) {
    // Forest path ending
    engine.display.clear();
    drawColoredScreen(0xF800); // Red for danger
    drawBottomText("You find a wolf!\nPress any button to restart.");
    engine.display.update();
    waitForInput();
    gameState = 0; // Back to start

  } else if (gameState == 4) {
    // Tree climbing ending
    engine.display.clear();
    drawColoredScreen(0x07FF); // Cyan for success
    drawBottomText("You find a bird's nest with treasure!\nPress any button to restart.");
    engine.display.update();
    waitForInput();
    gameState = 0; // Back to start
  }

  // Add more game states for cave endings...
}
```

## Adding a Reset Function

Create a helper function to reset the game:

```cpp
void resetGame() {
  gameState = 0;
  // Reset any other game variables here
}
```

## Story Design Tips

1. **Keep choices clear**: Each choice should have distinct, interesting outcomes
2. **Balance difficulty**: Mix positive and negative outcomes
3. **Create meaningful branches**: Different paths should feel different
4. **Add replayability**: Multiple ways to reach the same ending

## Your Challenge

Create a 3-level deep story with:
- 2 choices at the start
- 2 choices in each middle section
- 4 different endings
- At least 3 different colored screens

Think about what kind of story you want to tell. Some ideas:
- **Fantasy Adventure**: Forest, castle, dragon
- **Space Exploration**: Planets, aliens, discoveries
- **Mystery Story**: Rooms, clues, suspects
- **Survival Scenario**: Weather, resources, decisions

How does the deeper story structure change the player's experience?
