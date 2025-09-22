// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Kywy.hpp>

// Include all game scenes
#include "games/Snake.cpp"
#include "games/2DTennis.cpp"
#include "games/SlimeJumper.cpp"
#include "games/Spelunker.cpp"

using namespace Kywy;

// Create game scene instances
SnakeScene snakeScene;
TennisScene tennisScene;
ClickerScene clickerScene;
SlimeJumperScene slimeJumperScene;
SpelunkerScene spelunkerScene;

Kywy::Engine engine;  // This starts the kywy firmware and engine
MenuSystem* mainMenu;  // Global menu system pointer

void setup() {
  // Initialize engine
  engine.start();
  
  // Set static engine reference for scenes
  Scene::setEngine(&engine);
  
  // Initialize all scenes
  snakeScene.initialize();
  tennisScene.initialize();
  clickerScene.initialize();
  slimeJumperScene.initialize();
  spelunkerScene.initialize();
  
  // Create menu system with default options
  mainMenu = new MenuSystem(engine.display);
  
  // Build menu using the helper functions
  mainMenu->addLabelItem("Mini Game Launcher");
  mainMenu->addLabelItem("");
  mainMenu->addSceneItem("Snake", &snakeScene);
  mainMenu->addSceneItem("2D Tennis", &tennisScene);
  mainMenu->addSceneItem("Cookie Clicker", &clickerScene);
  mainMenu->addSceneItem("Slime Jumper", &slimeJumperScene);
  mainMenu->addSceneItem("Spelunker", &spelunkerScene);
  
  // Start the menu system directly
  mainMenu->start(engine);
}

void loop() {
  delay(1000);
}