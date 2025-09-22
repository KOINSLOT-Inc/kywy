// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef TENNIS_SCENE_HPP
#define TENNIS_SCENE_HPP

#include <Kywy.hpp>

using namespace Kywy;

class TennisScene : public Scene, public Actor::Actor {
private:
  bool gameStarted = false;
  int playerScore = 0;
  int opponentScore = 0;
  int paddleY = 20;
  int ballX = 60;
  int ballY = 32;
  
public:
  TennisScene() : Scene(false, true) {}

  virtual void onEnter() override {
    gameStarted = false;
    playerScore = 0;
    opponentScore = 0;
    paddleY = 20;
    ballX = 60;
    ballY = 32;
    add(this);
    
    Display& display = Scene::getEngine()->display;
    display.clear();
    
    Display::TextOptions textOptions;
    textOptions._color = 0x00;
    textOptions._origin = Display::Origin::Text::BASELINE_LEFT;
    textOptions._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(20, 20, "2D TENNIS", textOptions);
    display.drawText(10, 35, "Press any button", textOptions);
    display.drawText(10, 45, "to start", textOptions);
    display.drawText(10, 55, "Left: Exit to menu", textOptions);
    display.update();
  }

  void handle(::Actor::Message* message) override {
    switch (message->signal) {
      case Kywy::Events::D_PAD_LEFT_PRESSED:
        Scene::triggerExit();
        return;
        
      case Kywy::Events::D_PAD_UP_PRESSED:
        if (gameStarted) {
          paddleY = max(5, paddleY - 3);
          updateDisplay();
        } else {
          startGame();
        }
        break;
        
      case Kywy::Events::D_PAD_DOWN_PRESSED:
        if (gameStarted) {
          paddleY = min(50, paddleY + 3);
          updateDisplay();
        } else {
          startGame();
        }
        break;
        
      case Kywy::Events::D_PAD_RIGHT_PRESSED:
        if (!gameStarted) {
          startGame();
        }
        break;
        
      case Kywy::Events::TICK:
        if (gameStarted) {
          // Simple ball movement
          ballX += 1;
          if (ballX > 110) ballX = 10;
          updateDisplay();
        }
        break;
    }
  }

  void startGame() {
    gameStarted = true;
    updateDisplay();
  }

  void updateDisplay() {
    Display& display = Scene::getEngine()->display;
    display.clear();
    
    Display::TextOptions textOptions;
    textOptions._color = 0x00;
    textOptions._origin = Display::Origin::Text::BASELINE_LEFT;
    textOptions._font = Display::Font::intel_one_mono_8_pt;
    
    // Draw score
    char scoreStr[20];
    sprintf(scoreStr, "%d - %d", playerScore, opponentScore);
    display.drawText(45, 12, scoreStr, textOptions);
    
    // Draw game elements
    Display::Object2DOptions options;
    options._color = 0x00;
    
    // Player paddle
    display.fillRectangle(5, paddleY, 3, 20, options);
    
    // Opponent paddle
    display.fillRectangle(112, ballY - 10, 3, 20, options);
    
    // Ball
    display.fillCircle(ballX, ballY, 3, options);
    
    // Instructions
    display.drawText(5, 60, "Up/Down: Move  Left: Exit", textOptions);
    
    display.update();
  }

  virtual void onExit() override {
    remove(this);
  }
};

// Global instance for the launcher
TennisScene tennisScene;

#endif // TENNIS_SCENE_HPP
