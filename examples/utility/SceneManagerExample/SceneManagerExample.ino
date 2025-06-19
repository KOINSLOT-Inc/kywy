// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * SceneManagerExample.ino
 * 
 * This example demonstrates how to use the Scene Manager system to create
 * a game with multiple scenes including splash screen, main menu, settings,
 * and gameplay scenes with smooth transitions.
 * 
 * The example is optimized for the black-on-white memory LCD display.
 */

#include "Kywy.hpp"

using namespace Kywy;

// Global engine instance
Engine engine;
std::unique_ptr<SceneManager> sceneManager;

// Forward declarations
std::shared_ptr<SplashScene> createSplashScene();
std::shared_ptr<MenuScene> createMainMenuScene();
std::shared_ptr<MenuScene> createSettingsMenuScene();
std::shared_ptr<Scene> createAboutScene();

// Game state variables
bool soundEnabled = true;
bool vibrationEnabled = false;
bool autoSave = true;
std::string difficulty = "Medium";

// Splash screen data (simplified - replace with actual splash image)
const uint8_t splashScreenData[] = {
  // This would be your actual splash screen bitmap data
  // For demonstration, this is just placeholder data
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  // ... more bitmap data
};

//==============================================================================
// Game Scene Implementation
//==============================================================================

class GameScene : public Scene {
private:
  bool gameRunning = false;
  float gameTime = 0.0f;
  
public:
  GameScene() : Scene("GameScene") {}
  
  void initialize() override {
    Scene::initialize();
    // Initialize game objects, sprites, etc.
  }
  
  void enter() override {
    Scene::enter();
    gameRunning = true;
    gameTime = 0.0f;
    
    engine.display.clear();
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    engine.display.drawText(20, 60, "Game Started!", textOpt);
    engine.display.drawText(10, 80, "Press LEFT to return", textOpt);
    engine.display.update();
  }
  
  void update(float deltaTime) override {
    Scene::update(deltaTime);
    
    if (gameRunning) {
      gameTime += deltaTime;
      // Update game logic here
    }
  }
  
  void render(Display::Display& display) override {
    display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(30, 40, "GAME SCENE", textOpt);
    
    // Show game time
    char timeStr[32];
    snprintf(timeStr, sizeof(timeStr), "Time: %.1fs", gameTime / 1000.0f);
    display.drawText(20, 60, timeStr, textOpt);
    
    display.drawText(10, 100, "LEFT: Back to Menu", textOpt);
    display.drawText(10, 120, "RIGHT: Pause", textOpt);
    
    display.update();
  }
  
  void handleInput(int signal, void* data) override {
    switch (signal) {
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        // Return to main menu
        sceneManager->popScene(SceneTransition(TransitionType::SLIDE_RIGHT, 300));
        break;
        
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        // Pause game
        gameRunning = !gameRunning;
        break;
    }
  }
  
  void pause() override {
    Scene::pause();
    gameRunning = false;
  }
  
  void resume() override {
    Scene::resume();
    gameRunning = true;
  }
};

//==============================================================================
// Scene Creation Functions
//==============================================================================

std::shared_ptr<SplashScene> createSplashScene() {
  auto splash = SceneFactory::createSplashScene(
    "SplashScreen",
    splashScreenData,
    64, 32,  // width, height
    2000     // 2 second duration
  );
  
  // Set callback to transition to main menu when splash is complete
  splash->onSplashComplete = []() {
    // Create and show main menu
    auto mainMenu = createMainMenuScene();
    sceneManager->changeScene(mainMenu, SceneTransition(TransitionType::FADE_TO_BLACK, 500));
  };
  
  return splash;
}

std::shared_ptr<MenuScene> createMainMenuScene() {
  // Create menu items
  std::vector<MenuSystem::MenuItem> menuItems = {
    MenuSystem::createAction("Start Game", []() {
      auto gameScene = std::make_shared<GameScene>();
      sceneManager->pushScene(gameScene, SceneTransition(TransitionType::SLIDE_LEFT, 400));
    }),
    
    MenuSystem::createAction("Settings", []() {
      auto settingsMenu = createSettingsMenuScene();
      sceneManager->pushScene(settingsMenu, SceneTransition(TransitionType::SLIDE_LEFT, 300));
    }),
    
    MenuSystem::createLabel("-- Options --"),
    
    MenuSystem::createToggle("Sound", &soundEnabled),
    MenuSystem::createToggle("Auto Save", &autoSave),
    
    MenuSystem::createOption("Difficulty", difficulty, []() {
      // Cycle through difficulty options
      if (difficulty == "Easy") {
        difficulty = "Medium";
      } else if (difficulty == "Medium") {
        difficulty = "Hard";
      } else {
        difficulty = "Easy";
      }
    }, []() {
      return difficulty;
    }),
    
    MenuSystem::createAction("About", []() {
      // Show about screen as overlay
      auto aboutScene = createAboutScene();
      sceneManager->pushScene(aboutScene, SceneTransition(TransitionType::SLIDE_UP, 300));
    }),
    
    MenuSystem::createAction("Exit", []() {
      // In a real game, this might save state and exit
      engine.display.clear();
      Display::TextOptions textOpt;
      textOpt._color = BLACK;
      textOpt._font = Display::Font::intel_one_mono_8_pt;
      engine.display.drawText(40, 80, "Goodbye!", textOpt);
      engine.display.update();
      delay(1000);
    })
  };
  
  // Create menu options
  MenuSystem::MenuOptions menuOptions;
  menuOptions.x = 10;
  menuOptions.y = 15;
  menuOptions.itemHeight = 12;
  menuOptions.pointer = '>';
  menuOptions.font = Display::Font::intel_one_mono_8_pt;
  
  // Create menu system
  auto menuSystem = std::make_unique<MenuSystem>(engine.display, menuItems, menuOptions);
  menuSystem->scrollOptions.setVisibleItems(8);
  
  // Create menu scene
  auto menuScene = SceneFactory::createMenuScene("MainMenu", std::move(menuSystem));
  
  return menuScene;
}

std::shared_ptr<MenuScene> createSettingsMenuScene() {
  std::vector<MenuSystem::MenuItem> settingsItems = {
    MenuSystem::createLabel("-- Settings --"),
    
    MenuSystem::createToggle("Sound Effects", &soundEnabled),
    MenuSystem::createToggle("Vibration", &vibrationEnabled),
    MenuSystem::createToggle("Auto Save", &autoSave),
    
    MenuSystem::createOption("Difficulty", difficulty, []() {
      if (difficulty == "Easy") {
        difficulty = "Medium";
      } else if (difficulty == "Medium") {
        difficulty = "Hard";
      } else {
        difficulty = "Easy";
      }
    }, []() {
      return difficulty;
    }),
    
    MenuSystem::createLabel(""),  // Spacer
    
    MenuSystem::createAction("Save Settings", []() {
      // In a real game, save settings to persistent storage
      engine.display.clear();
      Display::TextOptions textOpt;
      textOpt._color = BLACK;
      textOpt._font = Display::Font::intel_one_mono_8_pt;
      engine.display.drawText(20, 80, "Settings Saved!", textOpt);
      engine.display.update();
      delay(1000);
    }),
    
    MenuSystem::createAction("Back", []() {
      sceneManager->popScene(SceneTransition(TransitionType::SLIDE_RIGHT, 300));
    })
  };
  
  MenuSystem::MenuOptions menuOptions;
  menuOptions.x = 10;
  menuOptions.y = 15;
  menuOptions.itemHeight = 12;
  menuOptions.pointer = '>';
  menuOptions.font = Display::Font::intel_one_mono_8_pt;
  
  auto menuSystem = std::make_unique<MenuSystem>(engine.display, settingsItems, menuOptions);
  menuSystem->scrollOptions.setVisibleItems(8);
  
  auto settingsScene = SceneFactory::createMenuScene("SettingsMenu", std::move(menuSystem));
  
  return settingsScene;
}

std::shared_ptr<Scene> createAboutScene() {
  class AboutScene : public Scene {
  public:
    AboutScene() : Scene("AboutScene") {
      isOverlay = true;  // Render over previous scene
      pauseUnderlying = true;  // Pause underlying scene
    }
    
    void render(Display::Display& display) override {
      // Draw semi-transparent background for overlay effect
      // Since this is a memory LCD, we'll use a simple border
      display.clear();
      
      Display::TextOptions textOpt;
      textOpt._color = BLACK;
      textOpt._font = Display::Font::intel_one_mono_8_pt;
      
      // Draw border
      for (int x = 10; x < KYWY_DISPLAY_WIDTH - 10; x++) {
        display.drawPixel(x, 20, BLACK);
        display.drawPixel(x, KYWY_DISPLAY_HEIGHT - 20, BLACK);
      }
      for (int y = 20; y < KYWY_DISPLAY_HEIGHT - 20; y++) {
        display.drawPixel(10, y, BLACK);
        display.drawPixel(KYWY_DISPLAY_WIDTH - 11, y, BLACK);
      }
      
      // Draw about text
      display.drawText(20, 40, "Scene Manager Demo", textOpt);
      display.drawText(20, 55, "Version 1.0", textOpt);
      display.drawText(20, 70, "Built with Kywy", textOpt);
      display.drawText(20, 90, "Press any button", textOpt);
      display.drawText(20, 105, "to close", textOpt);
      
      display.update();
    }
    
    void handleInput(int signal, void* data) override {
      // Any input closes the about screen
      sceneManager->popScene(SceneTransition(TransitionType::SLIDE_DOWN, 300));
    }
  };
  
  return std::make_shared<AboutScene>();
}

//==============================================================================
// Scene Manager Event Handler
//==============================================================================

class SceneEventHandler : public Actor::Actor {
public:
  void handle(::Actor::Message* message) override {
    switch (message->signal) {
      case Kywy::SceneEvents::SCENE_CHANGED:
        Serial.println("Scene changed");
        break;
        
      case Kywy::SceneEvents::TRANSITION_STARTED:
        Serial.println("Transition started");
        break;
        
      case Kywy::SceneEvents::TRANSITION_COMPLETED:
        Serial.println("Transition completed");
        break;
        
      default:
        break;
    }
  }
} sceneEventHandler;

//==============================================================================
// Arduino Setup and Loop
//==============================================================================

void setup() {
  Serial.begin(9600);
  
  // Start the engine
  engine.start();
  
  // Create scene manager
  sceneManager = std::make_unique<SceneManager>(engine);
  sceneManager->initialize();
  sceneManager->start();
  
  // Subscribe to scene events
  sceneEventHandler.subscribe(sceneManager.get());
  sceneEventHandler.start();
  
  // Subscribe scene manager to engine for input and updates
  sceneManager->subscribe(&engine.input);
  sceneManager->subscribe(&engine.clock);
  
  // Start with splash screen
  auto splashScene = createSplashScene();
  sceneManager->pushScene(splashScene);
  
  Serial.println("Scene Manager Demo Started!");
}

void loop() {
  // The scene manager handles everything through the Actor system
  delay(16);  // ~60 FPS
}
