// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SNAKE_SCENE_HPP
#define SNAKE_SCENE_HPP

#include <Kywy.hpp>

using namespace Kywy;

class SnakeScene : public Scene, public Actor::Actor {
private:
  bool gameStarted = false;
  int score = 0;
  
public:
  SnakeScene() : Scene(false, true) {}

  virtual void onEnter() override {
    gameStarted = false;
    score = 0;
    add(this);
    
    Display& display = Scene::getEngine()->display;
    display.clear();
    
    Display::TextOptions textOptions;
    textOptions._color = 0x00;
    textOptions._origin = Display::Origin::Text::BASELINE_LEFT;
    textOptions._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(20, 20, "SNAKE GAME", textOptions);
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
        
      case Kywy::Events::D_PAD_RIGHT_PRESSED:
      case Kywy::Events::D_PAD_UP_PRESSED:
      case Kywy::Events::D_PAD_DOWN_PRESSED:
        if (!gameStarted) {
          startGame();
        }
        break;
        
      case Kywy::Events::TICK:
        if (gameStarted) {
          // Game logic would go here
        }
        break;
    }
  }

  void startGame() {
    gameStarted = true;
    
    Display& display = Scene::getEngine()->display;
    display.clear();
    
    Display::TextOptions textOptions;
    textOptions._color = 0x00;
    textOptions._origin = Display::Origin::Text::BASELINE_LEFT;
    textOptions._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(10, 15, "Snake Game Running", textOptions);
    display.drawText(10, 30, "Score: 0", textOptions);
    display.drawText(10, 45, "Use D-Pad to move", textOptions);
    display.drawText(10, 55, "Left: Exit", textOptions);
    
    // Draw simple snake
    Display::Object2DOptions options;
    options._color = 0x00;
    display.fillRectangle(60, 32, 4, 4, options);
    
    display.update();
  }

  virtual void onExit() override {
    remove(this);
  }
};

// Global instance for the launcher
SnakeScene snakeScene;

#endif // SNAKE_SCENE_HPP
