// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * SceneTemplate.ino
 * 
 * A simple template demonstrating basic Scene Manager usage.
 * Copy this file and modify it to create your own scenes.
 */

#include "Kywy.hpp"

using namespace Kywy;

Engine engine;
std::unique_ptr<SceneManager> sceneManager;

//==============================================================================
// Custom Scene Implementation
//==============================================================================

class MyCustomScene : public Scene {
private:
  float timer = 0.0f;
  bool animationRunning = true;
  
public:
  MyCustomScene() : Scene("MyCustomScene") {}
  
  void initialize() override {
    Scene::initialize();
    // Initialize your scene resources here
    // Example: Load sprites, create actors, etc.
  }
  
  void enter() override {
    Scene::enter();
    timer = 0.0f;
    animationRunning = true;
    
    Serial.println("Entered MyCustomScene");
  }
  
  void update(float deltaTime) override {
    Scene::update(deltaTime);
    
    if (animationRunning) {
      timer += deltaTime;
    }
    
    // Update your scene logic here
  }
  
  void render(Display::Display& display) override {
    display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    // Render your scene content here
    display.drawText(20, 40, "Custom Scene", textOpt);
    
    // Show timer
    char timerStr[32];
    snprintf(timerStr, sizeof(timerStr), "Time: %.1fs", timer / 1000.0f);
    display.drawText(20, 60, timerStr, textOpt);
    
    // Show controls
    display.drawText(10, 100, "LEFT: Back", textOpt);
    display.drawText(10, 120, "RIGHT: Toggle", textOpt);
    
    // Show animation state
    display.drawText(10, 140, animationRunning ? "Running" : "Paused", textOpt);
    
    display.update();
  }
  
  void handleInput(int signal, void* data) override {
    switch (signal) {
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        // Go back to previous scene
        sceneManager->popScene(SceneTransition(TransitionType::SLIDE_RIGHT, 300));
        break;
        
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        // Toggle animation
        animationRunning = !animationRunning;
        break;
        
      case Kywy::Events::D_PAD_UP_PRESSED:
        // Add your custom input handling here
        break;
        
      case Kywy::Events::D_PAD_DOWN_PRESSED:
        // Add your custom input handling here
        break;
    }
  }
  
  void pause() override {
    Scene::pause();
    animationRunning = false;
    Serial.println("MyCustomScene paused");
  }
  
  void resume() override {
    Scene::resume();
    animationRunning = true;
    Serial.println("MyCustomScene resumed");
  }
  
  void exit() override {
    Scene::exit();
    Serial.println("Exited MyCustomScene");
  }
};

//==============================================================================
// Main Menu Scene
//==============================================================================

std::shared_ptr<MenuScene> createMainMenu() {
  std::vector<MenuSystem::MenuItem> menuItems = {
    MenuSystem::createAction("Custom Scene", []() {
      auto customScene = std::make_shared<MyCustomScene>();
      sceneManager->pushScene(customScene, SceneTransition(TransitionType::SLIDE_LEFT, 400));
    }),
    
    MenuSystem::createAction("Exit", []() {
      engine.display.clear();
      Display::TextOptions textOpt;
      textOpt._color = BLACK;
      textOpt._font = Display::Font::intel_one_mono_8_pt;
      engine.display.drawText(40, 80, "Goodbye!", textOpt);
      engine.display.update();
      delay(2000);
    })
  };
  
  MenuSystem::MenuOptions menuOptions;
  menuOptions.x = 10;
  menuOptions.y = 20;
  menuOptions.itemHeight = 15;
  menuOptions.pointer = '>';
  menuOptions.font = Display::Font::intel_one_mono_8_pt;
  
  auto menuSystem = std::make_unique<MenuSystem>(engine.display, menuItems, menuOptions);
  return SceneFactory::createMenuScene("MainMenu", std::move(menuSystem));
}

//==============================================================================
// Setup and Loop
//==============================================================================

void setup() {
  Serial.begin(9600);
  Serial.println("Scene Template Starting...");
  
  // Initialize engine
  engine.start();
  
  // Create scene manager
  sceneManager = std::make_unique<SceneManager>(engine);
  sceneManager->initialize();
  sceneManager->start();
  
  // Subscribe to engine events
  sceneManager->subscribe(&engine.input);
  sceneManager->subscribe(&engine.clock);
  
  // Start with main menu
  auto mainMenu = createMainMenu();
  sceneManager->pushScene(mainMenu);
  
  Serial.println("Scene Template Ready!");
}

void loop() {
  // Scene manager handles everything through the Actor system
  delay(16);  // ~60 FPS
}
