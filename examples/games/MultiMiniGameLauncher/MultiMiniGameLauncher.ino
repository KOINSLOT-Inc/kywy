// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Kywy.hpp>

// Include all game scenes (single-file, self-contained)
#include "games/SnakeScene.hpp"
#include "games/TennisScene.hpp"
#include "games/ClickerScene.hpp"
#include "games/SpelunkerScene.hpp"

using namespace Kywy;



Kywy::Engine engine;  // This starts the kywy firmware and engine
MenuSystem* mainMenu;  // Global menu system pointer

void setup() {
  // Initialize engine
  engine.start();
  
  // Set static engine reference for scenes
  Scene::setEngine(&engine);
  
  // Create menu system with default options
  mainMenu = new MenuSystem(engine.display);
  
  // Build menu using the helper functions
  mainMenu->addLabelItem("Mini Game Launcher");
  mainMenu->addLabelItem("");
  mainMenu->addSceneItem("2D Tennis", &tennisScene);
  mainMenu->addSceneItem("Cookie Clicker", &clickerScene);

  mainMenu->addSceneItem("Snake", &snakeScene);
  mainMenu->addSceneItem("Spelunker", &spelunkerScene);
  
  // Start the menu system directly
  mainMenu->start(engine);
}

void loop() {
}