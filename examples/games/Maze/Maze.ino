// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// Maze Game
//
// This example shows:
//   -How to create a maze game with bitmap collision detection
//   -How to move an 8x8 sprite around the screen
//   -How to use d-pad input for movement
//   -How to check for wall collisions using bit-packed bitmap data
//   -How to implement win/lose conditions
//   -How to use start and finish flag sprites
//
// This game is a bit more complex! Start with generating your own maze hpp file!

#include "Kywy.hpp"
#include "maze_example.hpp"  // This will contain our maze bitmap
#include "start_flag.hpp"    // Start flag sprite
#include "finish_flag.hpp"   // Finish flag sprite

Kywy::Engine engine;

// Maze dimensions (in tiles, each tile is 8x8 pixels)
const int MAZE_WIDTH = 18;   // 144 / 8 = 18 tiles wide
const int MAZE_HEIGHT = 21;  // 168 / 8 = 21 tiles tall

// Game variables
const int PLAYER_SIZE = 8;  // 8x8 pixel sprite
const int MOVE_SPEED = 8;   // Move 8 pixels at a time (1 tile)

// Start and finish positions (top-left and bottom-right)
const int START_X = 0;      // Top-left corner
const int START_Y = 0;
const int FINISH_X = KYWY_DISPLAY_WIDTH - PLAYER_SIZE;   // Bottom-right corner
const int FINISH_Y = KYWY_DISPLAY_HEIGHT - PLAYER_SIZE;  // Bottom-right corner

// Player position variables
int playerX = START_X;      // Starting X position (in pixels)
int playerY = START_Y;      // Starting Y position (in pixels)

// Simple 8x8 player sprite (Smiley face)
uint8_t playerSprite[64] = {
    0xC3, 0xBD, 0x5A, 0x7E, 0x5A, 0x66, 0xBD, 0xC3
};

// Helper function to check if a rectangular area has any walls
bool isWall(int x, int y, int width = 1, int height = 1) {
  // Check all pixels in the rectangular area
  for (int checkY = y; checkY < y + height; checkY++) {
    for (int checkX = x; checkX < x + width; checkX++) {
      // Check bounds
      if (checkX < 0 || checkX >= MAZE_EXAMPLE_WIDTH || checkY < 0 || checkY >= MAZE_EXAMPLE_HEIGHT) {
        return true; // Out of bounds is treated as wall
      }

      // Calculate byte index in the bitmap array
      // Each row has 144 pixels = 18 bytes (144 ÷ 8 = 18)
      int bytesPerRow = MAZE_EXAMPLE_WIDTH / 8;
      int byteIndex = checkY * bytesPerRow + (checkX / 8);

      // Get the bit position within the byte (0-7, from left to right)
      int bitPosition = checkX % 8;

      // Check if the bit is clear (0 = wall, 1 = open space)
      // We shift 1 left by (7 - bitPosition) because bits are stored MSB first
      uint8_t mask = 1 << (7 - bitPosition);
      if ((maze_example_data[byteIndex] & mask) == 0) {
        return true; // Found a wall pixel
      }
    }
  }
  return false; // No walls found in the area
}

// Helper function to check if player reached the goal
bool reachedGoal(int x, int y) {
  // Goal is at the finish flag position
  // Check if player overlaps with the finish flag area
  return (x >= FINISH_X && x <= FINISH_X + PLAYER_SIZE && y >= FINISH_Y && y <= FINISH_Y + PLAYER_SIZE);
}

void setup() {
  engine.start();
}

void loop() {
  // This game works a little differently than the other examples
  // We each time we loop, we generate one frame of the game
  // We check input, update game state, and redraw everything
  // This is how many modern games work too!

  // Sample input at the start of the frame for consistent detection
  bool leftPressed = engine.input.dPadLeftPressed;
  bool rightPressed = engine.input.dPadRightPressed;
  bool upPressed = engine.input.dPadUpPressed;
  bool downPressed = engine.input.dPadDownPressed;

  // Store original position for collision detection
  int oldX = playerX;
  int oldY = playerY;

  // Move player based on input
  if (leftPressed) {
    playerX -= MOVE_SPEED;
  } else if (rightPressed) {
    playerX += MOVE_SPEED;
  } else if (upPressed) {
    playerY -= MOVE_SPEED;
  } else if (downPressed) {
    playerY += MOVE_SPEED;
  }

  // Check for wall collisions - check entire player area
  if (isWall(playerX, playerY, PLAYER_SIZE, PLAYER_SIZE)) {
    // Collision detected - revert to old position
    playerX = oldX;
    playerY = oldY;
    // If we want to make the game harder, could we try editing this block?
  }

  // Keep player within screen bounds
  if (playerX < 0) playerX = 0;
  if (playerX > KYWY_DISPLAY_WIDTH - PLAYER_SIZE) playerX = KYWY_DISPLAY_WIDTH - PLAYER_SIZE;
  if (playerY < 0) playerY = 0;
  if (playerY > KYWY_DISPLAY_HEIGHT - PLAYER_SIZE) playerY = KYWY_DISPLAY_HEIGHT - PLAYER_SIZE;

  // Draw everything
  engine.display.clear();

  // Draw the maze background
  engine.display.drawBitmap(0, 0, MAZE_EXAMPLE_WIDTH, MAZE_EXAMPLE_HEIGHT, maze_example_data);

  // Draw the start flag at the starting position (only if player is not there)
  if (playerX != START_X || playerY != START_Y) {
    engine.display.drawBitmap(START_X, START_Y, START_FLAG_WIDTH, START_FLAG_HEIGHT, start_flag_data);
  }

  // Draw the finish flag at the goal position
  engine.display.drawBitmap(FINISH_X, FINISH_Y, FINISH_FLAG_WIDTH, FINISH_FLAG_HEIGHT, finish_flag_data);

  // Draw the player sprite (on top of everything)
  engine.display.drawBitmap(playerX, playerY, PLAYER_SIZE, PLAYER_SIZE, playerSprite);

  // Check win condition
  if (reachedGoal(playerX, playerY)) {
    // Win screen - clear and draw centered box
    engine.display.clear();
    
    // Draw centered white box with black border
    int boxWidth = 120;
    int boxHeight = 60;
    int boxX = (KYWY_DISPLAY_WIDTH - boxWidth) / 2;
    int boxY = (KYWY_DISPLAY_HEIGHT - boxHeight) / 2;
    
    // White background
    engine.display.fillRectangle(boxX, boxY, boxWidth, boxHeight, Display::Object2DOptions().color(0xFF));
    // Black border
    engine.display.drawRectangle(boxX, boxY, boxWidth, boxHeight, Display::Object2DOptions().color(0x00));
    
    // Center the text
    int textY = boxY + 15;
    engine.display.drawText(boxX + 35, textY, "YOU WIN!");
    engine.display.drawText(boxX + 5, textY + 15, "RIGHT to restart");
    
    engine.display.update();

    // First, wait for right button to be released (in case it's currently pressed)
    while (engine.input.buttonRightPressed) {
      delay(10);
    }

    // Then wait for a new right button press to restart
    while (!engine.input.buttonRightPressed) {
      delay(10);
    }

    // Reset player position
    playerX = START_X;
    playerY = START_Y;
    return; // Restart the game
  }

  engine.display.update();
  delay(100); // Small delay for smooth movement
}
