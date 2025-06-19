// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_SLIME_JUMPER_GAME_SCENE_HPP
#define KYWY_SLIME_JUMPER_GAME_SCENE_HPP

#include "Kywy.hpp"

namespace Kywy {

class SlimeJumperGameScene : public Scene {
private:
  bool startScreen = true;
  std::function<void()> exitCallback;

public:
  SlimeJumperGameScene() : Scene("SlimeJumperGame") {}
  
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
    
    engine.display.drawText(35, 40, "SLIME JUMPER", textOpt);
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
    
    display.drawText(35, 40, "SLIME JUMPER", textOpt);
    display.drawText(20, 60, "Coming Soon!", textOpt);
    display.drawText(20, 80, "Press B to return", textOpt);
  }
  void handleInput(int signal, void* data) override {
    if (signal == Events::BUTTON_LEFT_PRESSED && exitCallback) {
      exitCallback();
    }
  }
  void update(float deltaTime) override {}
};

}  // namespace Kywy

#endif  // KYWY_SLIME_JUMPER_GAME_SCENE_HPP
