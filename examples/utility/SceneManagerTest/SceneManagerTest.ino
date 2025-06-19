// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * SceneManagerTest.ino
 * 
 * A minimal test of the Scene Manager system to verify basic functionality.
 */

#include "Kywy.hpp"

using namespace Kywy;

Engine engine;
std::unique_ptr<SceneManager> sceneManager;

//==============================================================================
// Simple Test Scene
//==============================================================================

class TestScene : public Scene {
public:
  TestScene() : Scene("TestScene") {}
  
  void render(Display::Display& display) override {
    display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(20, 60, "Test Scene", textOpt);
    display.drawText(10, 80, "Scene Manager Works!", textOpt);
    
    display.update();
  }
  
  void handleInput(int signal, void* data) override {
    // Any input exits
    engine.display.clear();
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    engine.display.drawText(40, 80, "Done!", textOpt);
    engine.display.update();
    delay(1000);
  }
};

//==============================================================================
// Setup and Loop
//==============================================================================

void setup() {
  Serial.begin(9600);
  Serial.println("Scene Manager Test Starting...");
  
  // Initialize engine
  engine.start();
  
  // Create scene manager
  sceneManager = std::make_unique<SceneManager>(engine);
  sceneManager->initialize();
  sceneManager->start();
  
  // Subscribe to engine events
  sceneManager->subscribe(&engine.input);
  sceneManager->subscribe(&engine.clock);
  
  // Create and show test scene
  auto testScene = std::make_shared<TestScene>();
  sceneManager->pushScene(testScene);
  
  Serial.println("Scene Manager Test Ready!");
}

void loop() {
  delay(16);  // ~60 FPS
}
