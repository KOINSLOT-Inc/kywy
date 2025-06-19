// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_GAME_SCENES_WRAPPER_HPP
#define KYWY_GAME_SCENES_WRAPPER_HPP

#include "Kywy.hpp"
#include "SnakeGameScene.hpp"
#include "TennisGameScene.hpp"
#include "AsteroidsGameScene.hpp"
#include "PongGameScene.hpp"
#include "SlimeJumperGameScene.hpp"
#include "SpelunkerGameScene.hpp"

namespace Kywy {

/**
 * Factory class for creating game scenes for the multi-launcher
 * This wraps each individual game as a Scene that can be used with SceneManager
 */
class GameScenesWrapper {
public:
  
  static std::shared_ptr<Scene> createSnakeGameScene() {
    return std::make_shared<SnakeGameScene>();
  }
  
  static std::shared_ptr<Scene> create2DTennisGameScene() {
    return std::make_shared<TennisGameScene>();
  }
  
  static std::shared_ptr<Scene> createAsteroidsGameScene() {
    return std::make_shared<AsteroidsGameScene>();
  }
  
  static std::shared_ptr<Scene> createPongGameScene() {
    return std::make_shared<PongGameScene>();
  }
  
  static std::shared_ptr<Scene> createSlimeJumperGameScene() {
    return std::make_shared<SlimeJumperGameScene>();
  }
  
  static std::shared_ptr<Scene> createSpelunkerGameScene() {
    return std::make_shared<SpelunkerGameScene>();
  }
};

}  // namespace Kywy

#endif  // KYWY_GAME_SCENES_WRAPPER_HPP
