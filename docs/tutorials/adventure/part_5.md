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

### Step 1: Include Your Sprite File

Add this at the top of your `Adventure.ino` file:

```cpp
// Include your generated sprite file
#include "my_hero.hpp"
```

### Step 2: Use Your Sprite in the Game

The HPP file will contain:
- `my_hero_data[]` - The sprite image data
- `MY_HERO_WIDTH` - The sprite width
- `MY_HERO_HEIGHT` - The sprite height

Use it in your game like this:

```cpp
void loop() {
  // Show start screen with your custom sprite
  engine.display.clear();
  engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen);

  // Draw your custom hero sprite
  engine.display.drawBitmap(50, 100, MY_HERO_WIDTH, MY_HERO_HEIGHT, my_hero_data);

  drawInstructionText();
  engine.display.update();

  ButtonEvent choice = waitForInput();
  // ... rest of your game logic
}
```

## Advanced Sprite Techniques

### Creating Animated Sprites

Make multiple versions of your sprite for animation:

```cpp
// Hero walking animation frames
uint8_t heroWalk1_data[8192] = { /* frame 1 data */ };
uint8_t heroWalk2_data[8192] = { /* frame 2 data */ };
uint8_t heroWalk3_data[8192] = { /* frame 3 data */ };

#define HERO_WIDTH 32
#define HERO_HEIGHT 32

void drawAnimatedHero(int x, int y, int frame) {
  if (frame == 0) {
    engine.display.drawBitmap(x, y, HERO_WIDTH, HERO_HEIGHT, heroWalk1_data);
  } else if (frame == 1) {
    engine.display.drawBitmap(x, y, HERO_WIDTH, HERO_HEIGHT, heroWalk2_data);
  } else {
    engine.display.drawBitmap(x, y, HERO_WIDTH, HERO_HEIGHT, heroWalk3_data);
  }
}
```

### Sprite State System

Combine sprites with your game states:

```cpp
// Different hero sprites for different states
uint8_t heroHealthy_data[8192] = { /* healthy appearance */ };
uint8_t heroWounded_data[8192] = { /* wounded appearance */ };
uint8_t heroDead_data[8192] = { /* dead appearance */ };

void drawHeroBasedOnHealth(int x, int y) {
  if (health > 60) {
    engine.display.drawBitmap(x, y, HERO_WIDTH, HERO_HEIGHT, heroHealthy_data);
  } else if (health > 0) {
    engine.display.drawBitmap(x, y, HERO_WIDTH, HERO_HEIGHT, heroWounded_data);
  } else {
    engine.display.drawBitmap(x, y, HERO_WIDTH, HERO_HEIGHT, heroDead_data);
  }
}
```

## Integrating Sprites with Game Logic

### Dynamic Sprite Display

Use sprites in your nested if statements:

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

### Sprite Animation in Choices

Add animation to make choices more engaging:

```cpp
int animationFrame = 0;
unsigned long lastFrameTime = 0;

void drawAnimatedChoice() {
  if (millis() - lastFrameTime > 200) { // Change frame every 200ms
    animationFrame = (animationFrame + 1) % 3; // Cycle through 3 frames
    lastFrameTime = millis();
  }

  // Draw animated sprite
  drawAnimatedHero(50, 100, animationFrame);
}
```

## Creating a Complete Sprite Set

### Step-by-Step Sprite Creation

1. **Design your concept**: Sketch what you want on paper
2. **Create base sprite**: Main character or object
3. **Make variations**: Different states (happy, sad, angry)
4. **Add animations**: Walking, running, idle frames
5. **Test in game**: See how they look on the actual device

### Example: Complete Character Sprite Set

```cpp
// Character sprites
uint8_t characterIdle_data[2048] = { /* 32x32 idle pose */ };
uint8_t characterHappy_data[2048] = { /* 32x32 happy pose */ };
uint8_t characterSad_data[2048] = { /* 32x32 sad pose */ };

// Environment sprites
uint8_t forestBackground_data[19200] = { /* 400x240 background */ };
uint8_t caveBackground_data[19200] = { /* 400x240 background */ };

// Item sprites
uint8_t sword_data[512] = { /* 16x16 sword */ };
uint8_t potion_data[512] = { /* 16x16 potion */ };
```

## Performance Considerations

### Memory Usage
- **Smaller sprites** use less memory
- **Reuse sprites** when possible
- **Unload unused sprites** if memory gets tight

### Drawing Performance
- **Draw less frequently** - only when things change
- **Use smaller areas** for partial screen updates
- **Batch drawing operations** together

## Your Challenge: Add Custom Sprites

1. **Create a character sprite** using the HPP generator
2. **Make multiple versions** (healthy, wounded, victorious)
3. **Add environment sprites** for different game states
4. **Integrate sprites** with your nested if logic
5. **Add simple animation** to one of your sprites

### Questions to Explore:
- How do custom sprites change the game feel?
- What's the best size for game sprites?
- How can sprites enhance your story choices?
- What performance trade-offs do you notice?

## Sprite Design Tips

### For Characters:
- **Clear silhouettes** - easy to see on small screens
- **Expressive faces** - show emotion clearly
- **Consistent proportions** - same character looks consistent

### For Environments:
- **Layered design** - background + foreground elements
- **Color coding** - use colors to indicate areas
- **Scale appropriately** - don't make details too small

### For Items:
- **Icon-style design** - clear, recognizable shapes
- **High contrast** - stand out against backgrounds
- **Consistent style** - match your game's art direction

## What's Next?

You've now learned:
- ✅ **Game loops** - Multiple sequential screens
- ✅ **Win/lose states** - Score and health systems
- ✅ **Nested if statements** - Complex conditional logic
- ✅ **Custom sprites** - Visual game elements

Your adventure game now has:
- Engaging progression through multiple loops
- Meaningful win/lose conditions
- Dynamic choices based on player state
- Custom visual elements

The sky's the limit! You can now create rich, interactive stories with professional-quality visuals. What epic adventure will you create next?
```

## Creating a Complete Sprite Set

### Step-by-Step Sprite Creation

1. **Design your concept**: Sketch what you want on paper
2. **Create base sprite**: Main character or object
3. **Make variations**: Different states (happy, sad, angry)
4. **Add animations**: Walking, running, idle frames
5. **Test in game**: See how they look on the actual device

### Example: Complete Character Sprite Set

```cpp
// Character sprites
uint8_t characterIdle_data[2048] = { /* 32x32 idle pose */ };
uint8_t characterHappy_data[2048] = { /* 32x32 happy pose */ };
uint8_t characterSad_data[2048] = { /* 32x32 sad pose */ };

// Environment sprites
uint8_t forestBackground_data[19200] = { /* 400x240 background */ };
uint8_t caveBackground_data[19200] = { /* 400x240 background */ };

// Item sprites
uint8_t sword_data[512] = { /* 16x16 sword */ };
uint8_t potion_data[512] = { /* 16x16 potion */ };
```

## Performance Considerations

### Memory Usage
- **Smaller sprites** use less memory
- **Reuse sprites** when possible
- **Unload unused sprites** if memory gets tight

### Drawing Performance
- **Draw less frequently** - only when things change
- **Use smaller areas** for partial screen updates
- **Batch drawing operations** together

## Your Challenge: Add Custom Sprites

1. **Create a character sprite** using the HPP generator
2. **Make multiple versions** (healthy, wounded, victorious)
3. **Add environment sprites** for different game states
4. **Integrate sprites** with your nested if logic
5. **Add simple animation** to one of your sprites

### Questions to Explore:
- How do custom sprites change the game feel?
- What's the best size for game sprites?
- How can sprites enhance your story choices?
- What performance trade-offs do you notice?

## Sprite Design Tips

### For Characters:
- **Clear silhouettes** - easy to see on small screens
- **Expressive faces** - show emotion clearly
- **Consistent proportions** - same character looks consistent

### For Environments:
- **Layered design** - background + foreground elements
- **Color coding** - use colors to indicate areas
- **Scale appropriately** - don't make details too small

### For Items:
- **Icon-style design** - clear, recognizable shapes
- **High contrast** - stand out against backgrounds
- **Consistent style** - match your game's art direction

## What's Next?

You've now learned:
- ✅ **Game loops** - Multiple sequential screens
- ✅ **Win/lose states** - Score and health systems
- ✅ **Nested if statements** - Complex conditional logic
- ✅ **Custom sprites** - Visual game elements

Your adventure game now has:
- Engaging progression through multiple loops
- Meaningful win/lose conditions
- Dynamic choices based on player state
- Custom visual elements

The sky's the limit! You can now create rich, interactive stories with professional-quality visuals. What epic adventure will you create next?

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
