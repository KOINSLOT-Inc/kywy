// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_SNAKE_GAME_SCENE_HPP
#define KYWY_SNAKE_GAME_SCENE_HPP

#include "Kywy.hpp"

namespace Kywy {

/**
 * Snake Game Scene - Wraps the original Snake game as a Scene
 * This allows the Snake game to be launched from the multi-launcher
 */
class SnakeGameScene : public Scene {
private:
  // Debug control (you can set this to false to disable debug output)
  static const bool debugMode = true;
  
  // Game state variables (copied from original Snake.ino)
  bool startScreen = true;
  bool gameOver = false;
  int highScore = 0;

  int initialTicksPerMove = 8;
  int ticksPerMove = initialTicksPerMove;
  float timeSinceLastMove = 0.0f;  // Use float for deltaTime accumulation
  float moveInterval = 0.2f;  // Move every 200ms

  int xDirection = 1;
  int yDirection = 0;
  int newXDirection = 1;
  int newYDirection = 0;

  static const int sectionSize = 8;
  static const int sectionSeparation = 3;
  static const int screenBorder = 2;
  static const int blockSize = sectionSize + sectionSeparation;
  static const int rows = KYWY_DISPLAY_HEIGHT / blockSize;
  static const int columns = KYWY_DISPLAY_WIDTH / blockSize;
  static const int numBlocks = rows * columns;
  static const int minBlockX = screenBorder;
  static const int minBlockY = screenBorder;
  static const int maxBlockX = screenBorder + blockSize * (columns - 1);
  static const int maxBlockY = screenBorder + blockSize * (rows - 1);

  int sectionX[numBlocks + 1] = { 0 };
  int sectionY[numBlocks + 1] = { 0 };

  int foodX = 0;
  int foodY = 0;
  int length = 2;
  
  // Clock subscription for game timing
  bool subscribedToClock = false;
  
  // Exit callback for returning to launcher
  std::function<void()> exitCallback;

public:
  SnakeGameScene() : Scene("SnakeGame") {}
  
  void setExitCallback(std::function<void()> callback) {
    exitCallback = callback;
  }
  
  void enter() override {
    if (debugMode) Serial.println("SnakeGameScene: Entering scene");
    Scene::enter();
    if (debugMode) {
      Serial.print("SnakeGameScene: After Scene::enter(), state is: ");
      Serial.println(static_cast<int>(getState()));
    }
    startScreen = true;
    gameOver = false;
    subscribedToClock = false;
    if (debugMode) {
      Serial.print("SnakeGameScene: Scene entered, startScreen=");
      Serial.println(startScreen ? "true" : "false");
      Serial.print("SnakeGameScene: isActive() = ");
      Serial.println(isActive() ? "true" : "false");
    }
    
    // Don't show splash screen here - let render() handle it
  }
  
  void showSplashScreen(Display::Display& display) {
    display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(60, 30, "SNAKE", textOpt);
    display.drawText(20, 50, "Arrow keys to move", textOpt);
    display.drawText(20, 65, "Press RIGHT (A) to start", textOpt);
    display.drawText(20, 80, "Press LEFT (B) to exit", textOpt);
    // Don't call display.update() here
  }
  
  void exit() override {
    Scene::exit();
    // Unsubscribe from clock if subscribed
    if (subscribedToClock) {
      extern Engine engine;
      // Note: We'll need a way to unsubscribe from clock
      // This may require extending the Engine or Actor API
      subscribedToClock = false;
    }
  }

  void drawHead() {
    extern Engine engine;
    for (int x = sectionX[length - 1]; x < sectionX[length - 1] + sectionSize; x++) {
      for (int y = sectionY[length - 1]; y < sectionY[length - 1] + sectionSize; y++) {
        engine.display.drawPixel(x, y, BLACK);
      }
    }
  }

  void drawTail() {
    extern Engine engine;
    for (int x = sectionX[0]; x < sectionX[0] + sectionSize; x++) {
      for (int y = sectionY[0]; y < sectionY[0] + sectionSize; y++) {
        engine.display.drawPixel(x, y, WHITE);
      }
    }
  }

  void drawFood() {
    extern Engine engine;
    for (int x = foodX; x < foodX + sectionSize; x++) {
      for (int y = foodY; y < foodY + sectionSize; y++) {
        engine.display.drawPixel(x, y, BLACK);
      }
    }
  }

  void dropFood() {
    extern Engine engine;
    bool validLocation = false;
    while (!validLocation) {
      foodX = minBlockX + (rand() % columns) * blockSize;
      foodY = minBlockY + (rand() % rows) * blockSize;
      validLocation = true;
      
      // Check if food overlaps with snake
      for (int i = 0; i < length; i++) {
        if (foodX == sectionX[i] && foodY == sectionY[i]) {
          validLocation = false;
          break;
        }
      }
    }
    drawFood();
  }

  void moveSnake() {
    extern Engine engine;
    
    // Move head
    int newHeadX = sectionX[length - 1] + xDirection * blockSize;
    int newHeadY = sectionY[length - 1] + yDirection * blockSize;
    
    // Check boundaries
    if (newHeadX < minBlockX || newHeadX > maxBlockX || 
        newHeadY < minBlockY || newHeadY > maxBlockY) {
      gameOver = true;
      return;
    }
    
    // Check self collision
    for (int i = 0; i < length; i++) {
      if (newHeadX == sectionX[i] && newHeadY == sectionY[i]) {
        gameOver = true;
        return;
      }
    }
    
    // Check food collision
    bool ateFood = (newHeadX == foodX && newHeadY == foodY);
    
    if (!ateFood) {
      // Remove tail
      drawTail();
      // Shift all sections forward
      for (int i = 0; i < length - 1; i++) {
        sectionX[i] = sectionX[i + 1];
        sectionY[i] = sectionY[i + 1];
      }
    } else {
      // Grow snake
      length++;
      // Increase speed slightly by reducing move interval
      if (moveInterval > 0.1f) {
        moveInterval -= 0.01f;  // Get 10ms faster each time
      }
    }
    
    // Add new head
    sectionX[length - 1] = newHeadX;
    sectionY[length - 1] = newHeadY;
    drawHead();
    
    if (ateFood) {
      dropFood();
    }
    
    if (gameOver) {
      // Game over state is handled in render()
    }
  }

  void showGameOver(Display::Display& display) {
    display.clear();
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(50, 60, "GAME OVER", textOpt);
    display.drawText(30, 80, "Press A to restart", textOpt);
    display.drawText(30, 95, "Press B to exit", textOpt);
    // Don't call display.update() here
  }

  void startGame() {
    extern Engine engine;
    
    startScreen = false;
    gameOver = false;
    moveInterval = 0.2f;  // Reset to initial speed
    timeSinceLastMove = 0.0f;  // Reset timing
    xDirection = 1;
    yDirection = 0;
    newXDirection = 1;
    newYDirection = 0;

    // Set up initial head and tail
    sectionX[0] = screenBorder;  // tail
    sectionY[0] = screenBorder;
    sectionX[1] = screenBorder + blockSize;  // head
    sectionY[1] = screenBorder;

    length = 2;

    // Initial game state setup - no need to update display here
    // since render() will be called by SceneManager
    
    // Place initial food
    dropFood();
    
    // Subscribe to clock for game timing
    if (!subscribedToClock) {
      // Note: This will need to be implemented properly
      // subscribe(&engine.clock);
      subscribedToClock = true;
    }
  }

  void render(Display::Display& display) override {
    if (startScreen) {
      showSplashScreen(display);
      return;
    }
    
    if (gameOver) {
      showGameOver(display);
      return;
    }
    
    // Render the current game state
    display.clear();
    
    // Draw all snake segments
    for (int i = 0; i < length; i++) {
      for (int x = sectionX[i]; x < sectionX[i] + sectionSize; x++) {
        for (int y = sectionY[i]; y < sectionY[i] + sectionSize; y++) {
          display.drawPixel(x, y, BLACK);
        }
      }
    }
    
    // Draw food
    if (foodX > 0 && foodY > 0) {
      for (int x = foodX; x < foodX + sectionSize; x++) {
        display.drawPixel(x, foodY, BLACK);
        display.drawPixel(x, foodY + sectionSize - 1, BLACK);
      }
      for (int y = foodY; y < foodY + sectionSize; y++) {
        display.drawPixel(foodX, y, BLACK);
        display.drawPixel(foodX + sectionSize - 1, y, BLACK);
      }
    }
    
    // Don't call display.update() here - let the SceneManager handle display updates
  }

  void handleInput(int signal, void* data) override {
    switch (signal) {
      case Events::D_PAD_LEFT_PRESSED:
        if (startScreen || gameOver) break;
        if (xDirection != 1) {
          newXDirection = -1;
          newYDirection = 0;
        }
        break;
        
      case Events::D_PAD_RIGHT_PRESSED:
        if (startScreen || gameOver) break;
        if (xDirection != -1) {
          newXDirection = 1;
          newYDirection = 0;
        }
        break;
        
      case Events::D_PAD_UP_PRESSED:
        if (startScreen || gameOver) break;
        if (yDirection != 1) {
          newXDirection = 0;
          newYDirection = -1;
        }
        break;
        
      case Events::D_PAD_DOWN_PRESSED:
        if (startScreen || gameOver) break;
        if (yDirection != -1) {
          newXDirection = 0;
          newYDirection = 1;
        }
        break;
        
      case Events::BUTTON_RIGHT_PRESSED:  // A button
        if (startScreen) {
          startGame();
        } else if (gameOver) {
          startGame();
        }
        break;
        
      case Events::BUTTON_LEFT_PRESSED:  // B button
        if (gameOver || startScreen) {
          // Exit to launcher
          if (exitCallback) {
            exitCallback();
          }
        }
        break;
    }
  }

  void update(float deltaTime) override {
    // DEBUG: Uncomment for debugging
    Serial.println("Snake update called");
    
    if (gameOver || startScreen) {
      Serial.println("Snake: early return (gameOver or startScreen)");
      return;
    }
    
    timeSinceLastMove += deltaTime;
    
    if (timeSinceLastMove >= moveInterval) {
      Serial.println("Snake: moving!");
      xDirection = newXDirection;
      yDirection = newYDirection;
      moveSnake();
      timeSinceLastMove = 0.0f;
      
      // Don't call display.update() here - let the SceneManager handle rendering
    }
  }
};

}  // namespace Kywy

#endif  // KYWY_SNAKE_GAME_SCENE_HPP
