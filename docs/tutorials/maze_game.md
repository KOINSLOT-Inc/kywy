---
title: "Maze Game Tutorial: Custom Images & Collision Detection"
---

<!-- hide previous page button that links outside of tutorial -->
<style>
  .md-footer__link--prev:not([hidden]) { display: none }
</style>

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Maze Game Tutorial: Custom Images & Collision Detection

Welcome to the maze game tutorial! This tutorial will teach you how to create and customize maze games with the Kywy engine. We'll cover:

- How maze collision detection works with bit-packed images
- Creating custom maze layouts
- Adding your own sprites and images
- Understanding the game structure

<a href="https://discord.gg/zAYym57Fy6"><img alt="Discord" src="https://img.shields.io/discord/1172988360063725629?style=for-the-badge&logo=discord" /></a>

## What You'll Learn

By the end of this tutorial, you'll understand:
- How to create maze games with pixel-perfect collision detection
- How to convert images into Kywy-compatible sprite data
- How to customize game sprites and maze layouts
- How the bit-packed collision system works

## The Maze Game Structure

Let's start by examining the maze game code structure:

```c++
// Game constants - these control the game behavior
const int PLAYER_SIZE = 8;  // 8x8 pixel player sprite
const int MOVE_SPEED = 8;   // Move 8 pixels per frame (1 tile)

// Start and finish positions
const int START_X = 0;      // Top-left corner
const int START_Y = 0;
const int FINISH_X = KYWY_DISPLAY_WIDTH - PLAYER_SIZE;   // Bottom-right
const int FINISH_Y = KYWY_DISPLAY_HEIGHT - PLAYER_SIZE;
```

## Understanding Bit-Packed Collision Detection

The maze game uses a special collision detection system that works with bit-packed images. Here's how it works:

### How Images Are Stored

Kywy images are stored as arrays of bytes where each **bit** represents one pixel:

```c++
// Example: 8 pixels stored in 1 byte
// Bit:     7 6 5 4 3 2 1 0
// Pixel:   0 1 0 1 0 1 0 1  = 0x55 in hex
uint8_t imageData[8] = {0x55, /* ... more bytes ... */};
```

- **0 bits** = walls (black pixels, can't walk through)
- **1 bits** = open spaces (white pixels, can walk through)

### The Collision Detection Function

```c++
bool isWall(int x, int y, int width = 1, int height = 1) {
  // Check all pixels in the rectangular area
  for (int checkY = y; checkY < y + height; checkY++) {
    for (int checkX = x; checkX < x + width; checkX++) {
      // Calculate which byte contains this pixel
      int bytesPerRow = MAZE_EXAMPLE_WIDTH / 8;
      int byteIndex = checkY * bytesPerRow + (checkX / 8);

      // Get the bit position within the byte
      int bitPosition = checkX % 8;
      uint8_t mask = 1 << (7 - bitPosition);

      // Check if this bit is 0 (wall) or 1 (open)
      if ((maze_example_data[byteIndex] & mask) == 0) {
        return true; // Found a wall!
      }
    }
  }
  return false; // No walls in this area
}
```

## Creating Custom Maze Images

### Step 1: Design Your Maze

Create a simple black and white image (144×168 pixels) where:
- **Black pixels** (#000000) = walls
- **White pixels** (#FFFFFF) = open paths

### Step 2: Convert to Kywy Format

Use the Kywy Drawing Editor or online tools to convert your image:

1. Go to the [Kywy Drawing Editor](https://kywy.org/draw)
2. Upload your maze image
3. Set dimensions to 144×168
4. Export as C++ header file

### Step 3: Replace the Maze Data

Replace the `maze_example.hpp` file with your custom maze:

```c++
// Your custom maze data
uint8_t my_maze_data[3024] = {
  // Your maze bytes here...
};

// Update dimensions if different
#define MY_MAZE_WIDTH 144
#define MY_MAZE_HEIGHT 168
```

### Step 4: Update the Game Code

Update the maze game to use your custom data:

```c++
#include "my_maze.hpp"  // Your custom maze

// Update the draw call
engine.display.drawBitmap(0, 0, MY_MAZE_WIDTH, MY_MAZE_HEIGHT, my_maze_data);

// Update collision detection
int bytesPerRow = MY_MAZE_WIDTH / 8;
```

## Customizing Sprites

### Player Sprite

The player uses an 8×8 sprite stored as 64 bytes (8 bytes per row):

```c++
uint8_t playerSprite[64] = {
    0xC3, 0xBD, 0x5A, 0x7E, 0x5A, 0x66, 0xBD, 0xC3
};
```

Each byte represents 8 pixels in a row. To create custom sprites:

1. Design an 8×8 pixel image
2. Convert each row to a byte value
3. Replace the `playerSprite` array

### Start and Finish Flags

The game includes start and finish flag sprites:

```c++
#include "start_flag.hpp"
#include "finish_flag.hpp"

// Draw flags
engine.display.drawBitmap(START_X, START_Y, START_FLAG_WIDTH, START_FLAG_HEIGHT, start_flag_data);
engine.display.drawBitmap(FINISH_X, FINISH_Y, FINISH_FLAG_WIDTH, FINISH_FLAG_HEIGHT, finish_flag_data);
```

Create custom flags using the same process as maze images.

## Game Logic Breakdown

### Input Handling

The game samples input at the start of each frame for consistency:

```c++
void loop() {
  // Sample input at frame start
  bool leftPressed = engine.input.dPadLeftPressed;
  bool rightPressed = engine.input.dPadRightPressed;
  bool upPressed = engine.input.dPadUpPressed;
  bool downPressed = engine.input.dPadDownPressed;

  // Use these values throughout the frame
  // ... game logic ...
}
```

### Movement and Collision

```c++
int oldX = playerX;
int oldY = playerY;

// Try to move
if (leftPressed) playerX -= MOVE_SPEED;

// Check collision with entire player area
if (isWall(playerX, playerY, PLAYER_SIZE, PLAYER_SIZE)) {
  // Hit a wall - revert position
  playerX = oldX;
  playerY = oldY;
}
```

### Win Condition

```c++
if (reachedGoal(playerX, playerY)) {
  // Show win screen
  engine.display.drawText(35, 75, "YOU WIN!");

  // Wait for button press to restart
  while (!engine.input.buttonRightPressed) {
    delay(10);
  }

  // Reset game
  playerX = START_X;
  playerY = START_Y;
}
```

## Tips for Custom Mazes

### Design Principles

1. **Clear Paths**: Ensure there's always a path from start to finish
2. **Visual Clarity**: Use high contrast between walls and paths
3. **Size Constraints**: Keep within 144×168 pixel display limits
4. **Performance**: More complex mazes = more collision checks

### Common Issues

1. **Wrong Dimensions**: Ensure your image is exactly 144×168
2. **Color Issues**: Only use pure black (#000000) and white (#FFFFFF)
3. **Bit Order**: The conversion tool handles bit ordering automatically

### Testing Your Maze

1. **Compile and Upload**: Test on actual hardware
2. **Walk Through**: Manually test all paths
3. **Edge Cases**: Check corners and tight spaces
4. **Performance**: Ensure smooth movement

## Advanced Customizations

### Variable Player Size

```c++
const int PLAYER_SIZE = 16;  // 16x16 player
const int MOVE_SPEED = 4;    // Slower movement for larger sprite
```

### Multiple Mazes

```c++
const uint8_t* currentMaze = maze1_data;
// Switch mazes based on level
if (level == 2) currentMaze = maze2_data;
```

### Animated Sprites

```c++
int frame = (millis() / 200) % 4;  // 4 frames, 200ms each
engine.display.drawBitmap(x, y, 8, 8, playerFrames[frame]);
```

## Next Steps

Now that you understand how the maze game works:

1. **Create** your own maze image
2. **Convert** it using the Kywy tools
3. **Customize** the sprites and gameplay
4. **Test** on your Kywy device
5. **Share** your creations on Discord!

The maze game demonstrates advanced concepts like bit manipulation, collision detection, and image processing - skills that apply to many types of games!

## Troubleshooting

### Common Issues

**"Maze doesn't load"**
- Check image dimensions (must be 144×168)
- Ensure pure black/white colors only

**"Player clips through walls"**
- Verify bit ordering in your image data
- Check collision detection coordinates

**"Game runs slowly"**
- Reduce maze complexity
- Optimize collision detection loops

**"Sprites don't appear"**
- Check sprite dimensions match draw calls
- Verify header file includes

Remember: The Kywy community is here to help! Join our Discord for support and to share your maze game creations.
