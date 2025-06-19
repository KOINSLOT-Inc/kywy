// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"
#include "GameScenesWrapper.hpp"

// Global engine variable referenced by game scenes - put in Kywy namespace
namespace Kywy {
  Engine engine;
}

std::shared_ptr<Kywy::SceneManager> globalSceneManager;

// Multi-Game Launcher Scene
class MultiLauncherScene : public Kywy::Scene {
private:
  std::unique_ptr<Kywy::MenuSystem> mainMenu;
  Kywy::Engine* engineRef = nullptr;
  
public:
  MultiLauncherScene() : Scene("MultiLauncher") {}
  
  void setEngineReference(Kywy::Engine* engine) override {
    engineRef = engine;
  }
  
  void launchGame(const std::string& gameName) {
    Serial.print("LauncherScene: Launching game: ");
    Serial.println(gameName.c_str());
    
    // Create the appropriate game scene based on the name
    std::shared_ptr<Kywy::Scene> gameScene = nullptr;
    
    if (gameName == "Snake") {
      Serial.println("LauncherScene: Creating Snake game scene");
      gameScene = Kywy::GameScenesWrapper::createSnakeGameScene();
      // Direct cast since we know the type
      if (gameScene) {
        Serial.println("LauncherScene: Snake scene created successfully");
        static_cast<Kywy::SnakeGameScene*>(gameScene.get())->setExitCallback([this]() {
          returnToLauncher();
        });
      } else {
        Serial.println("LauncherScene: ERROR - Failed to create Snake scene");
      }
    } else if (gameName == "2DTennis") {
      gameScene = Kywy::GameScenesWrapper::create2DTennisGameScene();
      if (gameScene) {
        static_cast<Kywy::TennisGameScene*>(gameScene.get())->setExitCallback([this]() {
          returnToLauncher();
        });
      }
    } else if (gameName == "Asteroids") {
      gameScene = Kywy::GameScenesWrapper::createAsteroidsGameScene();
      if (gameScene) {
        static_cast<Kywy::AsteroidsGameScene*>(gameScene.get())->setExitCallback([this]() {
          returnToLauncher();
        });
      }
    } else if (gameName == "PongScene") {
      gameScene = Kywy::GameScenesWrapper::createPongGameScene();
      if (gameScene) {
        static_cast<Kywy::PongGameScene*>(gameScene.get())->setExitCallback([this]() {
          returnToLauncher();
        });
      }
    } else if (gameName == "SlimeJumper") {
      gameScene = Kywy::GameScenesWrapper::createSlimeJumperGameScene();
      if (gameScene) {
        static_cast<Kywy::SlimeJumperGameScene*>(gameScene.get())->setExitCallback([this]() {
          returnToLauncher();
        });
      }
    } else if (gameName == "Spelunker") {
      gameScene = Kywy::GameScenesWrapper::createSpelunkerGameScene();
      if (gameScene) {
        static_cast<Kywy::SpelunkerGameScene*>(gameScene.get())->setExitCallback([this]() {
          returnToLauncher();
        });
      }
    }
    
    if (gameScene && globalSceneManager) {
      Serial.println("LauncherScene: Pushing game scene to SceneManager");
      // Push the game scene to the scene manager
      globalSceneManager->pushScene(gameScene);
      Serial.println("LauncherScene: Game scene pushed successfully");
    } else {
      if (!gameScene) {
        Serial.println("LauncherScene: ERROR - gameScene is null");
      }
      if (!globalSceneManager) {
        Serial.println("LauncherScene: ERROR - globalSceneManager is null");
      }
    }
  }
  
  void returnToLauncher() {
    // Pop back to the launcher scene
    if (globalSceneManager) {
      globalSceneManager->popScene();
    }
  }
  
  void enter() override {
    Scene::enter();
    
    Serial.println("MultiLauncherScene: enter() called");
    
    if (!engineRef) {
      // Try to get engine reference directly
      engineRef = &Kywy::engine;
    }
    
    if (!engineRef) {
      Serial.println("MultiLauncherScene: ERROR - No engine reference!");
      return;
    }
    
    Serial.println("MultiLauncherScene: Creating menu items");
    
    // Create menu items
    std::vector<Kywy::MenuSystem::MenuItem> menuItems = {
      Kywy::MenuSystem::createLabel("=== KYWY GAMES ==="),
      Kywy::MenuSystem::createAction("Snake", [this]() { 
        launchGame("Snake"); 
      }),
      Kywy::MenuSystem::createAction("2D Tennis", [this]() { 
        launchGame("2DTennis"); 
      }),
      Kywy::MenuSystem::createAction("Asteroids", [this]() { 
        launchGame("Asteroids"); 
      }),
      Kywy::MenuSystem::createAction("Pong Scene", [this]() { 
        launchGame("PongScene"); 
      }),
      Kywy::MenuSystem::createAction("Slime Jumper", [this]() { 
        launchGame("SlimeJumper"); 
      }),
      Kywy::MenuSystem::createAction("Spelunker", [this]() { 
        launchGame("Spelunker"); 
      }),
      Kywy::MenuSystem::createAction("Exit", [this]() { 
        showExitMessage();
      })
    };
    
    Kywy::MenuSystem::MenuOptions menuOptions;
    menuOptions.x = 20;
    menuOptions.y = 30;
    menuOptions.itemHeight = 12;
    menuOptions.pointer = '>';
    menuOptions.font = Display::Font::intel_one_mono_8_pt;
    menuOptions.textColor = BLACK;  // Black text on white background
    
    Serial.println("MultiLauncherScene: Creating MenuSystem");
    mainMenu = std::make_unique<Kywy::MenuSystem>(engineRef->display, menuItems, menuOptions);
    
    if (mainMenu) {
      Serial.println("MultiLauncherScene: MenuSystem created successfully");
    } else {
      Serial.println("MultiLauncherScene: ERROR - Failed to create MenuSystem");
    }
    
    // Note: render() will be called by SceneManager, which will update the display
  }
  
  void showExitMessage() {
    if (engineRef) {
      engineRef->display.clear();
      Display::TextOptions textOpt;
      textOpt._color = BLACK;  // Black text on white background
      textOpt._font = Display::Font::intel_one_mono_8_pt;
      engineRef->display.drawText(30, 60, "Thanks for playing!", textOpt);
      engineRef->display.drawText(20, 80, "Reset to play again", textOpt);
      engineRef->display.update();
    }
  }
  
  void render(Display::Display& display) override {
    // Don't clear display here - SceneManager already cleared it
    
    // Draw title
    Display::TextOptions titleOpt;
    titleOpt._color = BLACK;  // Black text on white background
    titleOpt._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(35, 15, "KYWY GAMES", titleOpt);
    
    // Render main menu content only (no clearing or updating)
    if (mainMenu) {
      mainMenu->renderMenuContent();
    } else {
      // Debug fallback
      Display::TextOptions debugOpt;
      debugOpt._color = BLACK;
      debugOpt._font = Display::Font::intel_one_mono_8_pt;
      display.drawText(10, 50, "Menu not initialized!", debugOpt);
    }
    
    // Don't call display.update() here - SceneManager will handle it
  }
  
  void handleInput(int signal, void* data) override {
    Serial.print("MultiLauncherScene: handleInput called with signal: ");
    Serial.println(signal);
    
    // Handle main menu input
    switch (signal) {
      case Kywy::Events::D_PAD_UP_PRESSED:
        Serial.println("MultiLauncherScene: D_PAD_UP_PRESSED");
        if (mainMenu) {
          mainMenu->previousOptionNoRender();
        } else {
          Serial.println("MultiLauncherScene: mainMenu is null!");
        }
        break;
        
      case Kywy::Events::D_PAD_DOWN_PRESSED:
        Serial.println("MultiLauncherScene: D_PAD_DOWN_PRESSED");
        if (mainMenu) {
          mainMenu->nextOptionNoRender();
        } else {
          Serial.println("MultiLauncherScene: mainMenu is null!");
        }
        break;
        
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        Serial.println("MultiLauncherScene: BUTTON_RIGHT_PRESSED");
        if (mainMenu) {
          mainMenu->selectOptionNoRender();
        } else {
          Serial.println("MultiLauncherScene: mainMenu is null!");
        }
        break;
        
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        Serial.println("MultiLauncherScene: BUTTON_LEFT_PRESSED");
        // Back button - could show exit confirmation
        showExitMessage();
        break;
    }
  }
};

void setup() {
  // Enable debug mode for development (set to false for production)
  bool enableDebug = true;  // Change this to false to disable debug output
  Kywy::SceneManager::setDebugMode(enableDebug);
  Kywy::Clock::setDebugMode(enableDebug);
  
  if (enableDebug) {
    Serial.println("=== DEBUG MODE ENABLED ===");
    Serial.println("SceneManager debug: ON");
    Serial.println("Clock debug: ON");
    Serial.println("===========================");
  }
  
  Kywy::engine.start();
  
  // Create the scene manager
  globalSceneManager = std::make_shared<Kywy::SceneManager>(Kywy::engine);
  globalSceneManager->initialize();
  globalSceneManager->start();  // Start the scene manager first
  
  // Subscribe scene manager to engine for input and updates
  Serial.println("Setup: Subscribing SceneManager to input and clock");
  globalSceneManager->subscribe(&Kywy::engine.input);
  globalSceneManager->subscribe(&Kywy::engine.clock);
  Serial.println("Setup: Subscription complete");
  
  // Create and push the launcher scene
  auto launcherScene = std::make_shared<MultiLauncherScene>();
  globalSceneManager->pushScene(launcherScene);
  Serial.println("Setup: Complete");
}

void loop() {
  // The engine and scene manager handle everything through the Actor system
  delay(16);  // ~60 FPS
}
