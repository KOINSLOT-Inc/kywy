// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SPELUNKER_SCE  // Splash screen dimensions
  static const int SPLASH_WIDTH = 144;
  static const int SPLASH_HEIGHT = 168;
#define SPELUNKER_SCENE_HPP

#include <Kywy.hpp>

using namespace Kywy;

class SpelunkerScene : public Scene {
private:
  // Game state
  bool startScreen = true;
  bool gameOver = true;
  bool inMenu = true;
  int score = 0;
  int highScore = 0;

  // Spelunker character state
  int spelunkerX = 10;
  int spelunkerY = 64;
  int spelunkerYVelocity = -5;
  int padding = 5;
  bool buttonPressed = false;

  // Column management
  static const uint8_t numColumns = 16;
  static const uint8_t columnWidth = KYWY_DISPLAY_WIDTH / numColumns;
  uint8_t topColumns[numColumns + 1];
  uint8_t bottomColumns[numColumns + 1];
  uint8_t numEntranceColumns = 6;
  uint8_t startIndex = 0;
  uint8_t pixelsPerTick = 4;
  uint8_t ticksPerColumnWidth = columnWidth / pixelsPerTick;
  uint8_t tickCounter = 0;

  // Internal actor for game logic
  class SpelunkerActor : public Actor::Actor {
  private:
    SpelunkerScene* scene;
    
  public:
    SpelunkerActor(SpelunkerScene* s) : scene(s) {}
    
    void initialize() override {
      // Actor initialization if needed
    }
    
    void handle(::Actor::Message *message) override {
      switch (message->signal) {
        case Kywy::Events::BUTTON_LEFT_PRESSED:
          scene->buttonPressed = true;
          if (scene->inMenu) {
            if (scene->startScreen) {
              // Exit from splash screen
              scene->triggerExit();
              return;
            }
          }
          break;
        case Kywy::Events::BUTTON_LEFT_RELEASED:
          scene->buttonPressed = false;
          break;
        case Kywy::Events::BUTTON_RIGHT_PRESSED:
          if (scene->startScreen) {
            // Start game from splash screen
            scene->startScreen = false;
            scene->startGame();
          } else if (scene->gameOver) {
            // Restart game when game over
            scene->startGame();
          } else {
            // During gameplay, right button exits to menu
            scene->triggerExit();
            return;
          }
          break;
        case Kywy::Events::TICK:
          // Don't process ticks during splash screen
          if (scene->startScreen || scene->gameOver) {
            break;
          }
          
          // Update game state
          scene->updateSpelunker();
          scene->updateColumns();
          
          // Check for collision
          if (scene->checkCollision()) {
            scene->endGame();
            break;
          }
          
          // Increase score
          scene->score += 1;
          
          // Draw everything
          Display::Display& display = Scene::getEngine()->display;
          display.clear();
          scene->drawColumns();
          scene->drawSpelunker();
          scene->drawScore();
          display.update();
          break;
      }
    }
  };
  
  SpelunkerActor spelunkerActor;

  // Spelunker sprite data (8x8 pixel character)
  const uint8_t spelunkerBMP[8] = {
    0b00000000,  //
    0b00111110,  //   ####
    0b10001000,  // #   #
    0b11111110,  // ######
    0b00111111,  //  #####
    0b00100010,  //  #   #
    0b00000000,  //
    0b00000000,  //
  };

  // Splash screen dimensions
  static const int SPLASH_WIDTH = 144;
  static const int SPLASH_HEIGHT = 168;

  // Spelunker splash screen bitmap
  const uint8_t splashScreen[3024] = {
    // Splash screen bitmap data (omitted for brevity)
};

  void initializeColumns() {
    // Initialize column lists to zero
    memset(topColumns, 0, sizeof(topColumns));
    memset(bottomColumns, 0, sizeof(bottomColumns));

    // Set up cave entrance columns with gradual slope
    for (int i = 0; i < numEntranceColumns; i++) {
      topColumns[(numColumns + 1) - numEntranceColumns + i] = 3 * i;
      bottomColumns[(numColumns + 1) - numEntranceColumns + i] = KYWY_DISPLAY_HEIGHT - 40 - (3 * i);
    }
    startIndex = 0;
    tickCounter = 0;
  }

  void drawColumns() {
    Display::Display& display = Scene::getEngine()->display;
    int offset = tickCounter * pixelsPerTick;

    for (int i = 0; i < numColumns; i++) {
      int currentIndex = (startIndex + i) % (numColumns + 1);
      int xPos = i * columnWidth - offset;

      if (xPos >= -columnWidth && xPos < KYWY_DISPLAY_WIDTH) {
        // Draw top column (white)
        if (topColumns[currentIndex] > 0) {
          display.fillRectangle(xPos, 0, columnWidth, topColumns[currentIndex]);
        }
        
        // Draw clear tunnel in middle (black - no drawing needed as display is cleared)
        
        // Draw bottom column (white)
        if (bottomColumns[currentIndex] > 0) {
          display.fillRectangle(xPos, KYWY_DISPLAY_HEIGHT - bottomColumns[currentIndex], 
                              columnWidth, bottomColumns[currentIndex]);
        }
      }
    }
  }

  void updateColumns() {
    tickCounter++;
    
    if (tickCounter >= ticksPerColumnWidth) {
      tickCounter = 0;
      startIndex = (startIndex + 1) % (numColumns + 1);
      
      // Generate new column at the end using original algorithm
      int newColumnIndex = (startIndex + numColumns) % (numColumns + 1);
      int lastColumnIndex = (newColumnIndex - 1 + (numColumns + 1)) % (numColumns + 1);
      uint8_t lastColumnHeight = topColumns[lastColumnIndex];
      
      // If first run, start with some height
      if (lastColumnHeight == 0) {
        lastColumnHeight = 20;
      }
      
      uint8_t newColumnHeight = lastColumnHeight;
      
      // Randomly adjust height
      if (random(2) == 1) {  // randomly choose up or down
        if (random(2) == 1) {
          newColumnHeight += random(1, 4);
        } else {
          newColumnHeight -= random(1, 4);
        }
      }
      
      // Keep height within bounds
      if (newColumnHeight < 5) newColumnHeight = 5;
      if (newColumnHeight > 35) newColumnHeight = 35;
      
      topColumns[newColumnIndex] = newColumnHeight;
      bottomColumns[newColumnIndex] = KYWY_DISPLAY_HEIGHT - 40 - newColumnHeight;  // pin tunnel width at 40
    }
  }

  void drawSpelunker() {
    Display::Display& display = Scene::getEngine()->display;
    
    // Draw 8x8 helicopter sprite
    for (int y = 0; y < 8; y++) {
      uint8_t row = spelunkerBMP[y];
      for (int x = 0; x < 8; x++) {
        if (row & (0x80 >> x)) {
          display.drawPixel(spelunkerX + x, spelunkerY + y, WHITE);
        }
      }
    }
  }

  void updateSpelunker() {
    // Apply movement
    spelunkerY += spelunkerYVelocity;

    // Bounds checking
    if (spelunkerY < padding) {
      spelunkerY = padding;
      spelunkerYVelocity = 0;
    }

    if (spelunkerY > KYWY_DISPLAY_HEIGHT - 8) {
      spelunkerY = KYWY_DISPLAY_HEIGHT - 8;
    }

    // Apply gravity
    if (spelunkerYVelocity < 7 && !buttonPressed) {
      spelunkerYVelocity += 1;
    }

    // Button input makes character fly up
    if (buttonPressed) {
      spelunkerYVelocity = -5;
    }
  }

  bool checkCollision() {
    // Simple collision detection with columns
    int spelunkerRight = spelunkerX + 8;
    int spelunkerBottom = spelunkerY + 8;
    
    // Check against all visible columns
    for (int i = 0; i < numColumns; i++) {
      int currentIndex = (startIndex + i) % (numColumns + 1);
      int offset = tickCounter * pixelsPerTick;
      int xPos = i * columnWidth - offset;
      
      if (xPos <= spelunkerRight && (xPos + columnWidth) >= spelunkerX) {
        // Check top column collision
        if (topColumns[currentIndex] > 0 && spelunkerY <= topColumns[currentIndex]) {
          return true;
        }
        
        // Check bottom column collision
        if (bottomColumns[currentIndex] > 0 && 
            spelunkerBottom >= (KYWY_DISPLAY_HEIGHT - bottomColumns[currentIndex])) {
          return true;
        }
      }
    }
    
    return false;
  }

  void drawScore() {
    Display::Display& display = Scene::getEngine()->display;
    char msg[16];
    snprintf(msg, sizeof(msg), "%d", score);
    display.fillRectangle(KYWY_DISPLAY_WIDTH - 40, 0, 40, 14);
    display.drawText(KYWY_DISPLAY_WIDTH - 33, 3, msg);
  }

  void startGame() {
    gameOver = false;
    inMenu = false;
    score = 0;
    
    // Reset spelunker position
    spelunkerX = 10;
    spelunkerY = 32;
    spelunkerYVelocity = -5;
    buttonPressed = false;
    
    // Initialize columns
    initializeColumns();
    
    // Clear display and start clock
    Display::Display& display = Scene::getEngine()->display;
    display.clear();
  }

  void endGame() {
    gameOver = true;
    inMenu = true;
    
    if (score > highScore) {
      highScore = score;
    }

    Display::Display& display = Scene::getEngine()->display;
    display.clear();
    display.drawText(5, 5, "GAME OVER");
    
    char msg[32];
    snprintf(msg, sizeof(msg), "Score: %d", score);
    display.drawText(5, 15, msg);
    snprintf(msg, sizeof(msg), "High Score: %d", highScore);
    display.drawText(5, 25, msg);
    display.drawText(5, 45, "Press right button");
    display.drawText(5, 55, "to try again.");
    display.update();
  }

public:
  SpelunkerScene() : spelunkerActor(this) {}

  void onEnter() override {
    startScreen = true;
    gameOver = false;
    
    // Add the actor to the scene and start it
    add(&spelunkerActor);
    spelunkerActor.start();
    
    // Subscribe to input and clock
    spelunkerActor.subscribe(&Scene::getEngine()->input);
    spelunkerActor.subscribe(&Scene::getEngine()->clock);
    
    // Show splash screen
    Display::Display& display = Scene::getEngine()->display;
    display.clear();
    
    // Draw simple text splash screen
    display.drawText(20, 20, "SPELUNKER");
    display.drawText(10, 35, "Press RIGHT to start");
    display.drawText(10, 45, "Press LEFT to exit");
    
    display.update();
  }

  void onExit() override {
    // Unsubscribe from input and clock
    spelunkerActor.unsubscribe(&Scene::getEngine()->input);
    spelunkerActor.unsubscribe(&Scene::getEngine()->clock);
    
    // Stop the actor and remove it from the scene
    spelunkerActor.stop();
    remove(&spelunkerActor);
  }
};

// Global instance for the launcher
SpelunkerScene spelunkerScene;

#endif // SPELUNKER_SCENE_HPP
