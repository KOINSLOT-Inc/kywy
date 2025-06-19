// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_PONG_GAME_SCENE_HPP
#define KYWY_PONG_GAME_SCENE_HPP

#include "Kywy.hpp"

namespace Kywy {

class PongGameScene : public Scene {
private:
  bool startScreen = true;
  std::function<void()> exitCallback;

public:
  PongGameScene() : Scene("PongGame") {}
  
  void setExitCallback(std::function<void()> callback) {
    exitCallback = callback;
  }
  
  void enter() override {
    Scene::enter();
    startScreen = true;
  }
  
  void render(Display::Display& display) override {
    display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(60, 40, "PONG", textOpt);
    display.drawText(20, 60, "Coming Soon!", textOpt);
    display.drawText(20, 80, "Press B to return", textOpt);
    // Don't call display.update() here - let SceneManager handle it
  }
  void handleInput(int signal, void* data) override {
    if (signal == Events::BUTTON_LEFT_PRESSED && exitCallback) {
      exitCallback();
    }
  }
  void update(float deltaTime) override {}
};

}  // namespace Kywy

#endif  // KYWY_PONG_GAME_SCENE_HPP
