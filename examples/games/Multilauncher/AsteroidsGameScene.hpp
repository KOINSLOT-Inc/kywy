// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_ASTEROIDS_GAME_SCENE_HPP
#define KYWY_ASTEROIDS_GAME_SCENE_HPP

#include "Kywy.hpp"

namespace Kywy {

/**
 * Asteroids Game Scene - Wraps the original Asteroids game as a Scene
 * TODO: Implement the full Asteroids game logic from the original .ino file
 */
class AsteroidsGameScene : public Scene {
private:
  bool startScreen = true;
  std::function<void()> exitCallback;

public:
  AsteroidsGameScene() : Scene("AsteroidsGame") {}
  
  void setExitCallback(std::function<void()> callback) {
    exitCallback = callback;
  }
  
  void enter() override {
    Scene::enter();
    startScreen = true;
    showPlaceholder();
  }
  
  void showPlaceholder() {
    extern Engine engine;
    engine.display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    engine.display.drawText(50, 40, "ASTEROIDS", textOpt);
    engine.display.drawText(20, 60, "Coming Soon!", textOpt);
    engine.display.drawText(20, 80, "Press B to return", textOpt);
    // Don't call display.update() here - let SceneManager handle it
  }

  void render(Display::Display& display) override {
    // Show placeholder content
    display.clear();
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(50, 40, "ASTEROIDS", textOpt);
    display.drawText(20, 60, "Coming Soon!", textOpt);
    display.drawText(20, 80, "Press B to return", textOpt);
  }

  void handleInput(int signal, void* data) override {
    if (signal == Events::BUTTON_LEFT_PRESSED) {
      if (exitCallback) {
        exitCallback();
      }
    }
  }

  void update(float deltaTime) override {
    // Placeholder implementation
  }
};

}  // namespace Kywy

#endif  // KYWY_ASTEROIDS_GAME_SCENE_HPP
