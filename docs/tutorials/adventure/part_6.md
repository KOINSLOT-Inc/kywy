---
title: "Part 6: Code Explanation"
---

<!-- hide next page button that links outside of tutorial -->
<style>
  .md-footer__link--next:not([hidden]) { display: none }
</style>

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Bonus: Understanding Helper Functions

Welcome to the bonus tutorial! In this part, we'll dive deep into the helper functions used in the Adventure game. Helper functions are one of the most important concepts in programming, so let's explore them in detail.

## What are Helper Functions?

Helper functions are small, reusable pieces of code that perform specific tasks. They help you:

- **Organize your code** - Break complex tasks into smaller, manageable pieces
- **Reuse code** - Write once, use many times
- **Make code easier to read** - Give meaningful names to complex operations
- **Debug more easily** - Test and fix problems in isolated functions
- **Maintain code** - Changes to one function don't affect the rest of your program

## The Helper Functions in Our Adventure Game

Let's examine each helper function in the Adventure game and understand how they work.

### 1. `waitForInput()` - The Input Handler

```cpp
ButtonEvent waitForInput() {
  ButtonEvent pressed = Kywy::Events::KywyEvents::INPUT;

  // Check initial state of buttons
  bool initialLeftPressed = engine.input.dPadLeftPressed;
  bool initialRightPressed = engine.input.dPadRightPressed;

  // Track previous state to make sure a button was not already held down
  bool wasLeftPressed = initialLeftPressed;
  bool wasRightPressed = initialRightPressed;

  while (pressed == Kywy::Events::KywyEvents::INPUT) {
    bool leftPressed = engine.input.dPadLeftPressed;
    bool rightPressed = engine.input.dPadRightPressed;

    // Detect button press: current is pressed AND previous was not pressed
    if (leftPressed && !wasLeftPressed) {
      pressed = Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED;
    } else if (rightPressed && !wasRightPressed) {
      pressed = Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED;
    }

    wasLeftPressed = leftPressed;
    wasRightPressed = rightPressed;
    engine.display.update();
    delay(10);
  }
  return pressed;
}
```

**What it does:**
- Waits for the player to press either the left or right d-pad button
- Uses **edge detection** to only register when a button is first pressed (not held down)
- Returns which button was pressed

**Why it's a helper function:**
- Input handling logic is complex and reusable
- Keeps the main game loop clean and focused on game logic
- Handles the tricky edge detection automatically

**Key concepts:**
- **Edge detection**: Only triggers when button state changes from not pressed to pressed
- **State tracking**: Remembers previous button states
- **Return values**: Functions can send information back to where they were called

### 2. `drawInstructionText()` - The UI Drawer

```cpp
void drawInstructionText() {
  // Right text
  engine.display.getTextSize("Go Right", textWidth, textHeight);
  engine.display.fillRectangle(centerX + 5, centerY - 25, textWidth + 10, textHeight + 10, Display::Object2DOptions().color(0xFF));
  engine.display.drawRectangle(centerX + 5, centerY - 25, textWidth + 10, textHeight + 10, Display::Object2DOptions().color(0x00));
  engine.display.drawText(centerX + 10, centerY - 20, "Go Right");

  // Left text
  engine.display.getTextSize("Go Left", textWidth, textHeight);
  engine.display.fillRectangle(centerX - 65, centerY - 25, 60, 20, Display::Object2DOptions().color(0xFF));
  engine.display.drawRectangle(centerX - 65, centerY - 25, 60, 20, Display::Object2DOptions().color(0x00));
  engine.display.drawText(centerX - 60, centerY - 20, "Go Left");
}
```

**What it does:**
- Draws the "Go Left" and "Go Right" instruction boxes on screen
- Creates white backgrounds with black borders for the text
- Positions the text boxes in the center of the screen

**Why it's a helper function:**
- Drawing UI elements requires many steps
- Used on multiple screens, so it's reused
- Separates UI concerns from game logic

**Key concepts:**
- **Text measurement**: Getting the size of text before drawing
- **Rectangle drawing**: Creating backgrounds and borders
- **Positioning**: Calculating where to place elements on screen

### 3. `drawBottomText()` - The Story Teller

```cpp
void drawBottomText(const char* text) {
  engine.display.getTextSize(text, textWidth, textHeight);
  engine.display.fillRectangle(0, KYWY_DISPLAY_HEIGHT - textHeight - 10, KYWY_DISPLAY_WIDTH, textHeight + 10, Display::Object2DOptions().color(0xFF));
  engine.display.drawRectangle(0, KYWY_DISPLAY_HEIGHT - textHeight - 10, KYWY_DISPLAY_WIDTH, textHeight + 10, Display::Object2DOptions().color(0x00));
  engine.display.drawText(5, KYWY_DISPLAY_HEIGHT - textHeight - 5, text);
}
```

**What it does:**
- Draws story text in a box at the bottom of the screen
- Automatically sizes the box based on the text content
- Creates a white background with black border

**Why it's a helper function:**
- Story text appears on multiple screens
- Handles text layout automatically
- Makes it easy to add narrative elements

**Key concepts:**
- **Parameters**: Functions can receive information (the `text` parameter)
- **Dynamic sizing**: Box size adjusts to fit the content
- **Screen coordinates**: Using display dimensions for positioning

## How Helper Functions Work Together

In the main game loop, these helper functions work together seamlessly:

```cpp
void loop() {
  // Show start screen
  engine.display.clear();
  engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen);
  drawInstructionText();  // Helper function draws UI

  ButtonEvent choice = waitForInput();  // Helper function gets input

  if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
    engine.display.clear();
    engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, leftScreen);
    drawBottomText("You found a reward!");  // Helper function shows story
  }
  // ... more game logic
}
```

Notice how the main loop focuses on **what** the game should do, while the helper functions handle **how** to do it.

## Creating Your Own Helper Functions

Here are some tips for creating effective helper functions:

### 1. Single Responsibility
Each function should do one thing well:

```cpp
// Good: One clear purpose
void drawPlayer(int x, int y) {
  // Draw player sprite
}

// Bad: Multiple responsibilities
void handleEverything() {
  // Draw player, handle input, update score, play sound...
}
```

### 2. Descriptive Names
Choose names that clearly describe what the function does:

```cpp
// Good names
void calculatePlayerDamage(int damage);
void renderHealthBar();
void playVictorySound();

// Bad names
void func1();
void doStuff();
void helper();
```

### 3. Parameters and Return Values
Use parameters to receive information and return values to send results:

```cpp
// Function that receives information and returns a result
int calculateScore(int points, int multiplier) {
  return points * multiplier;
}

// Function that receives information but doesn't return anything
void displayMessage(const char* message, int x, int y) {
  // Display the message at the given position
}
```

### 4. Keep Them Short
Aim for functions that fit on your screen. If a function gets too long, break it into smaller helper functions.

## Common Helper Function Patterns

### Input Handlers
```cpp
bool isButtonJustPressed(bool currentState, bool& previousState) {
  bool justPressed = currentState && !previousState;
  previousState = currentState;
  return justPressed;
}
```

### Drawing Functions
```cpp
void drawCenteredText(const char* text, int y) {
  uint16_t textWidth, textHeight;
  engine.display.getTextSize(text, textWidth, textHeight);
  int x = (KYWY_DISPLAY_WIDTH - textWidth) / 2;
  engine.display.drawText(x, y, text);
}
```

### Math Utilities
```cpp
int clamp(int value, int min, int max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}
```

## Practice Exercise

Try creating a helper function for the Adventure game:

1. Create a function called `showGameOverScreen()` that displays a game over message
2. Create a function called `resetGame()` that resets the score and health
3. Modify the main loop to use these new helper functions

## Summary

Helper functions are the building blocks of clean, maintainable code. They:

- Make your code more organized and readable
- Allow code reuse
- Make debugging easier
- Help you think about problems in smaller pieces

Remember: If you find yourself writing the same code multiple times, or if a function is getting too long and complex, it's time to create a helper function!

In the next tutorial, we'll explore more advanced game mechanics. Keep practicing with helper functions - they're essential for any serious programming project!
