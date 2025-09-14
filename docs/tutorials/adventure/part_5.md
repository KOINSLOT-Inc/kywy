---
title: "Part 5: Advanced Features and Polish"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 5: Advanced Features and Polish

Congratulations! You've learned the basics of creating choose-your-own-adventure games. Now let's add some advanced features to make your game more engaging and professional.

## Adding a Scoring System

Keep track of player progress and choices:

```cpp
// Add these variables at the top
int score = 0;
int health = 100;
int choicesMade = 0;

// In your choice handlers:
if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
  score += 10;
  choicesMade++;
  // ... rest of the code
}

// Display score function
void drawScore() {
  char scoreText[32];
  sprintf(scoreText, "Score: %d Health: %d", score, health);
  engine.display.drawText(5, 5, scoreText);
}
```

## Adding Animations

Create simple animations for transitions:

```cpp
void fadeIn() {
  for (int i = 0; i <= 255; i += 25) {
    // This is a simplified example - actual implementation
    // would depend on your graphics system
    delay(50);
  }
}

// Use it in your screens:
} else if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
  engine.display.clear();
  engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, leftScreen);
  drawBottomText("You found a reward!");
  fadeIn(); // Add fade-in effect
  engine.display.update();
}
```

## Sound Effects (if available)

If your Kywy device has sound capabilities:

```cpp
// Add sound effects for different outcomes
void playSuccessSound() {
  // Implementation depends on your sound system
  // tone(speakerPin, 800, 200);
}

void playDangerSound() {
  // tone(speakerPin, 200, 500);
}

// Use in your game:
if (goodOutcome) {
  playSuccessSound();
} else {
  playDangerSound();
}
```

## Creating a Game Over Screen

Add conditions for game endings:

```cpp
bool gameOver = false;

void checkGameOver() {
  if (health <= 0) {
    gameOver = true;
    engine.display.clear();
    drawColoredScreen(0xF800); // Red screen
    drawBottomText("Game Over!\nFinal Score: " + String(score) + "\nPress any button to restart");
    engine.display.update();
    waitForInput();
    resetGame();
  }
}

// Call this after each choice
checkGameOver();
```

## Adding a Title Screen

Create an attractive opening screen:

```cpp
void showTitleScreen() {
  engine.display.clear();
  drawColoredScreen(0x001F); // Blue background

  // Draw title text
  engine.display.getTextSize("ADVENTURE", textWidth, textHeight);
  engine.display.drawText((KYWY_DISPLAY_WIDTH - textWidth) / 2, KYWY_DISPLAY_HEIGHT / 2 - 20, "ADVENTURE");

  // Draw subtitle
  engine.display.getTextSize("Press any button to start", textWidth, textHeight);
  engine.display.drawText((KYWY_DISPLAY_WIDTH - textWidth) / 2, KYWY_DISPLAY_HEIGHT / 2 + 10, "Press any button to start");

  engine.display.update();
  waitForInput();
}
```

## Performance Optimization

Make your game run smoothly:

```cpp
// Add delays to prevent button spam
#define BUTTON_DELAY 200
unsigned long lastButtonPress = 0;

ButtonEvent waitForInput() {
  if (millis() - lastButtonPress < BUTTON_DELAY) {
    return Kywy::Events::KywyEvents::INPUT; // Too soon, ignore
  }
  // ... rest of input logic ...
  lastButtonPress = millis();
  return pressed;
}
```

## Your Final Challenge

Create a complete adventure game with:

1. **Title screen** with your game's name
2. **Multiple story branches** (at least 3 levels deep)
3. **Scoring system** that tracks player choices
4. **Health/damage system** with game over conditions
5. **At least 5 different endings**
6. **Visual polish** with different colors and text styles
7. **Smooth gameplay** with proper timing

## Sharing Your Game

Once you've created your adventure game, consider:

- **Testing with friends**: Get feedback on your story choices
- **Adding more features**: Sound, animations, save system
- **Creating sequels**: Build on your successful story
- **Sharing online**: Post your game code for others to learn from

## What's Next?

You've learned:
- ✅ Basic game structure and loops
- ✅ Input handling and choices
- ✅ Text display and story creation
- ✅ Complex branching narratives
- ✅ Advanced features and polish

The sky's the limit! You can create any kind of interactive story or game you can imagine. What adventure will you create next?
