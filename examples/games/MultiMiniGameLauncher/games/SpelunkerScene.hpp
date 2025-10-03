// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SPELUNKER_SCENE_HPP
#define SPELUNKER_SCENE_HPP

#include <Kywy.hpp>

using namespace Kywy;

typedef enum : uint16_t {
  START_SCREEN = Kywy::Events::USER_EVENTS,
  GAME_OVER,
} SpelunkerSignal;

const uint8_t splashScreenBMP[] = {

};

const uint8_t spelunkerBMP[] = {
  0b00000000,  //
  0b00111110,  //
  0b10001000,  //
  0b11111110,  //
  0b00111111,  //
  0b00100010,  //
  0b00000000,  //
  0b00000000,  //
};

class SpelunkerScene : public Scene, public Actor::Actor {
public:
  bool startScreen = true;
  bool gameOver = false;
  int score = 0;
  int highScore = 0;

private:
  // Game managers as internal classes
  class spelunkerManager : public Actor::Actor {
  public:
    int xPosition = 10;
    int yVelocity = -5;
    int yPosition = 64;
    int padding = 5;
    bool buttonPressed = false;
    
    const uint8_t *spelunkerFrames[1] = { spelunkerBMP };
    Sprite spelunkerSprite;
    
    spelunkerManager() : spelunkerSprite(spelunkerFrames, 1, 8, 8) {}

    void initialize() {
      spelunkerSprite.setDisplay(&Scene::getEngine()->display);
      spelunkerSprite.setPosition(xPosition, yPosition);
      spelunkerSprite.setVisible(true);
      spelunkerSprite.setNegative(true);
      spelunkerSprite.setColor(WHITE);
      spelunkerSprite.render();
    }

    void handle(::Actor::Message *message) {
      switch (message->signal) {
        case Kywy::Events::BUTTON_RIGHT_PRESSED:
        case Kywy::Events::D_PAD_UP_PRESSED:
        case Kywy::Events::D_PAD_DOWN_PRESSED:
        case Kywy::Events::D_PAD_LEFT_PRESSED:
        case Kywy::Events::D_PAD_RIGHT_PRESSED:
        case Kywy::Events::D_PAD_CENTER_PRESSED:
          buttonPressed = true;
          break;
        case Kywy::Events::BUTTON_RIGHT_RELEASED:
        case Kywy::Events::D_PAD_UP_RELEASED:
        case Kywy::Events::D_PAD_DOWN_RELEASED:
        case Kywy::Events::D_PAD_LEFT_RELEASED:
        case Kywy::Events::D_PAD_RIGHT_RELEASED:
        case Kywy::Events::D_PAD_CENTER_RELEASED:
          buttonPressed = false;
          break;
        case Kywy::Events::TICK:
          // up/down movement
          yPosition += yVelocity;

          if (yPosition < padding) {
            yPosition = padding;
            yVelocity = 0;
          }

          if (yPosition > KYWY_DISPLAY_HEIGHT - 8)
            yPosition = KYWY_DISPLAY_HEIGHT - 8;

          // gravity
          if (yVelocity < 7 && !buttonPressed)
            yVelocity += 1;

          if (buttonPressed)
            yVelocity = -5;

          // apply changes
          spelunkerSprite.setPosition(xPosition, yPosition);
          spelunkerSprite.render();
          break;
      }
    }
  };

  class columnManager : public Actor::Actor {
  public:
    static const uint8_t numColumns = 16;
    static const uint8_t columnWidth = KYWY_DISPLAY_WIDTH / numColumns;

    uint8_t topColumns[(numColumns + 1)];
    uint8_t bottomColumns[(numColumns + 1)];

    uint8_t numEntraceColumns = 6;
    uint8_t numEntraceColumnsLeft = numEntraceColumns;

    uint8_t startIndex = 0;
    uint8_t pixelsPerTick = 4;
    uint8_t ticksPerColumnWidth = columnWidth / pixelsPerTick;
    uint8_t tickCounter = 0;

    ::Actor::Message gameOverMessage = ::Actor::Message(SpelunkerSignal::GAME_OVER);
    spelunkerManager* spelunker;

    columnManager(spelunkerManager* spelunkerRef) : spelunker(spelunkerRef) {}

    void initialize() {
      memset(topColumns, 0, sizeof(topColumns));
      memset(bottomColumns, 0, sizeof(bottomColumns));

      for (int i = 0; i < numEntraceColumns; i++) {
        topColumns[(numColumns + 1) - numEntraceColumns + i] = 6 * i;
        bottomColumns[(numColumns + 1) - numEntraceColumns + i] = 6 * i;
      }
      startIndex = 0;
    }

    void handle(::Actor::Message *message) {
      switch (message->signal) {
        case Kywy::Events::TICK:
          int offset = tickCounter * pixelsPerTick;

          for (int i = 0; i < (numColumns + 1); i++) {
            int columnIndex = (startIndex + i) % (numColumns + 1);

            Scene::getEngine()->display.fillRectangle(i * columnWidth - offset, 0, columnWidth, topColumns[columnIndex], Display::Object2DOptions().color(WHITE));

            Scene::getEngine()->display.fillRectangle(i * columnWidth - offset, topColumns[columnIndex], columnWidth, KYWY_DISPLAY_HEIGHT - bottomColumns[columnIndex] - topColumns[columnIndex]);

            Scene::getEngine()->display.fillRectangle(i * columnWidth - offset, KYWY_DISPLAY_HEIGHT - bottomColumns[columnIndex], columnWidth, bottomColumns[columnIndex], Display::Object2DOptions().color(WHITE));
          }

          int lastColumnIndex = (startIndex - 1) % (numColumns + 1);
          if (lastColumnIndex == -1)
            lastColumnIndex = (numColumns + 1) - 1;

          uint8_t lastColumnHeight = topColumns[lastColumnIndex];

          if (tickCounter < (ticksPerColumnWidth - 1)) {
            tickCounter++;
          } else {
            startIndex = (startIndex + 1) % (numColumns + 1);
            tickCounter = 0;

            int newColumnIndex = (startIndex - 1) % (numColumns + 1);
            if (newColumnIndex == -1)
              newColumnIndex = (numColumns + 1) - 1;

            int8_t newColumnHeight;
            if (random(2) == 1) {
              newColumnHeight = lastColumnHeight + 6;
            } else {
              newColumnHeight = lastColumnHeight - 6;
            }
            newColumnHeight = fmin(KYWY_DISPLAY_HEIGHT - 72, fmax(0, newColumnHeight));

            topColumns[newColumnIndex] = newColumnHeight;
            bottomColumns[newColumnIndex] = KYWY_DISPLAY_HEIGHT - 72 - newColumnHeight;
          }

          // check for collisions
          uint8_t overlapColumnsStart = 1;
          uint8_t overlapColumnsEnd = (8 / columnWidth) + (offset ? 1 : 0);
          bool collided = false;
          for (int i = overlapColumnsStart; i <= overlapColumnsEnd; i++) {
            int columnIndex = (startIndex + i) % (numColumns + 1);

            if (spelunker->yPosition < (topColumns[columnIndex] - 1))
              collided = true;

            if ((spelunker->yPosition + 5) > (KYWY_DISPLAY_HEIGHT - bottomColumns[columnIndex]))
              collided = true;
          }

          if (collided) {
            publish(&gameOverMessage);
          }

          break;
      }
    }
  };

  // Game objects
  spelunkerManager spelunker;
  columnManager columns;

  void startGame() {
    startScreen = false;
    gameOver = false;
    score = 0;
    
    // Reset spelunker state
    spelunker.xPosition = 10;
    spelunker.yPosition = 64;
    spelunker.yVelocity = -5;
    spelunker.buttonPressed = false;
    
    // Reset column state
    columns.startIndex = 0;
    columns.tickCounter = 0;
    columns.numEntraceColumnsLeft = columns.numEntraceColumns;
    for (int i = 0; i < columns.numColumns; i++) {
      columns.topColumns[i] = 20;
      columns.bottomColumns[i] = KYWY_DISPLAY_HEIGHT - 72 - 20;
    }
    
    // Initialize the objects
    spelunker.initialize();
    columns.initialize();
    
    // Subscribe to game events - order matters for rendering layers!
    columns.subscribe(&Scene::getEngine()->clock);
    spelunker.subscribe(&Scene::getEngine()->input);
    spelunker.subscribe(&Scene::getEngine()->clock);
    this->subscribe(&Scene::getEngine()->clock);

    // Start the actors
    spelunker.start();
    columns.start();
    
    Scene::getEngine()->display.update();
  }

  void drawScore() {
    char msg[16];
    snprintf(msg, sizeof(msg), "%d", (uint16_t)score);
    Scene::getEngine()->display.fillRectangle(KYWY_DISPLAY_WIDTH - 40, 0, 40, 14, Display::Object2DOptions().color(WHITE));
    Scene::getEngine()->display.drawText(KYWY_DISPLAY_WIDTH - 33, 3, msg);
  }

public:
  SpelunkerScene() : Scene(false, true), columns(&spelunker) {}

  virtual void initialize() override {
    // Don't subscribe to input here - do it in onEnter to avoid early activation
  }

  virtual void onEnter() override {
    startScreen = true;
    gameOver = true;
    add(this);
    add(&spelunker);
    add(&columns);
    
    // Start the actor (this calls initialize())
    this->start();
    
    // Subscribe to column manager to receive GAME_OVER messages and input
    this->subscribe(&columns);
    this->subscribe(&Scene::getEngine()->input);
    
    Scene::getEngine()->display.clear();
    Scene::getEngine()->display.drawBitmap(0, 0, 144, 168, (uint8_t *)splashScreenBMP);
    Scene::getEngine()->display.update();
  }

  void handle(::Actor::Message *message) override {
    switch (message->signal) {
      case SpelunkerSignal::GAME_OVER:
        gameOver = true;
        highScore = fmax(score, highScore);
        this->unsubscribe(&Scene::getEngine()->clock);
        spelunker.unsubscribe(&Scene::getEngine()->clock);
        spelunker.unsubscribe(&Scene::getEngine()->input);
        columns.unsubscribe(&Scene::getEngine()->clock);
        spelunker.stop();
        columns.stop();
        
        Scene::getEngine()->display.clear();
        Scene::getEngine()->display.drawText(5, 5, "GAME OVER");
        char msg[32];
        snprintf(msg, sizeof(msg), "Score: %d", (uint16_t)score);
        Scene::getEngine()->display.drawText(5, 15, msg);
        snprintf(msg, sizeof(msg), "High Score: %d", highScore);
        Scene::getEngine()->display.drawText(5, 25, msg);
        Scene::getEngine()->display.drawText(5, 45, "Press right button");
        Scene::getEngine()->display.drawText(5, 55, "to try again.");
        Scene::getEngine()->display.drawText(5, 65, "Press left to exit.");
        Scene::getEngine()->display.update();
        break;
        
      case Kywy::Events::TICK:
        // Don't process ticks during splash screen or game over
        if (startScreen || gameOver) {
          break;
        }
        score += 1;
        drawScore();
        Scene::getEngine()->display.update();
        break;
        
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        // Exit with left button regardless of game state
        Scene::triggerExit();
        break;
        
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
      case Kywy::Events::D_PAD_UP_PRESSED:
      case Kywy::Events::D_PAD_DOWN_PRESSED:
      case Kywy::Events::D_PAD_LEFT_PRESSED:
      case Kywy::Events::D_PAD_RIGHT_PRESSED:
      case Kywy::Events::D_PAD_CENTER_PRESSED:
        if (startScreen) {
          // Start game from splash screen
          startScreen = false;
          Scene::getEngine()->display.clear();
          startGame();
        } else if (gameOver) {
          // Restart game when game over
          Scene::getEngine()->display.clear();
          startGame();
        }
        break;
    }
  }

  virtual void onExit() override {
    // Immediately clear display before stopping actor
    Scene::getEngine()->display.clear();
    Scene::getEngine()->display.update();
    
    // Unsubscribe from all inputs before stopping
    this->unsubscribe(&Scene::getEngine()->input);
    this->unsubscribe(&Scene::getEngine()->clock);
    this->unsubscribe(&columns);
    spelunker.unsubscribe(&Scene::getEngine()->input);
    spelunker.unsubscribe(&Scene::getEngine()->clock);
    columns.unsubscribe(&Scene::getEngine()->clock);
    
    // Stop the actors
    spelunker.stop();
    columns.stop();
    this->stop();
    
    remove(this);
  }
};

// Global instance for the launcher
SpelunkerScene spelunkerScene;

#endif // SPELUNKER_SCENE_HPP
