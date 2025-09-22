// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"
#include "Scene.hpp"
#include <vector>

using namespace Kywy;

class SnakeScene : public Scene {
  struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
  };

  std::vector<Point> snake;
  Point food;
  Point direction;
  int score;
  bool gameOver;
  bool gameStarted;

  static const int GRID_SIZE = 8;
  static const int SCREEN_WIDTH = 120;
  static const int SCREEN_HEIGHT = 64;

  class GameActor : public Actor::Actor {
    SnakeScene* scene;
    unsigned long lastMoveTime;
  public:
    GameActor(SnakeScene* s) : scene(s), lastMoveTime(0) {}
    void handle(::Actor::Message* message) override {
      switch (message->signal) {
        case Kywy::Events::TICK:
          scene->handleInput();
          if (scene->gameStarted && !scene->gameOver) {
            unsigned long currentTime = millis();
            if (currentTime - lastMoveTime > 200) {
              scene->updateGame();
              lastMoveTime = currentTime;
            }
          }
          scene->renderGame();
          break;
      }
    }
  };

  GameActor* gameActor;

  void initializeGame() {
    snake.clear();
    snake.push_back(Point(5, 5));
    direction = Point(1, 0);
    generateFood();
    score = 0;
    gameOver = false;
    gameStarted = false;
  }

  void generateFood() {
    do {
      food.x = rand() % (SCREEN_WIDTH / GRID_SIZE);
      food.y = rand() % (SCREEN_HEIGHT / GRID_SIZE);
    } while (std::find(snake.begin(), snake.end(), food) != snake.end());
  }

  void handleInput() {
    auto& input = Scene::getEngine()->input;
    if (!gameStarted && (input.buttonRightPressed || input.dPadUpPressed || input.dPadDownPressed || input.dPadLeftPressed || input.dPadRightPressed)) {
      gameStarted = true;
    }
    if (input.buttonLeftPressed) {
      Scene::triggerExit();
      return;
    }
    if (!gameStarted || gameOver) return;
    if (input.dPadUpPressed && direction.y != 1) direction = Point(0, -1);
    if (input.dPadDownPressed && direction.y != -1) direction = Point(0, 1);
    if (input.dPadLeftPressed && direction.x != 1) direction = Point(-1, 0);
    if (input.dPadRightPressed && direction.x != -1) direction = Point(1, 0);
    if (input.buttonRightPressed && gameOver) {
      initializeGame();
    }
  }

  void updateGame() {
    Point newHead = snake.front();
    newHead.x += direction.x;
    newHead.y += direction.y;
    if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH / GRID_SIZE || newHead.y < 0 || newHead.y >= SCREEN_HEIGHT / GRID_SIZE || std::find(snake.begin(), snake.end(), newHead) != snake.end()) {
      gameOver = true;
      return;
    }
    snake.insert(snake.begin(), newHead);
    if (newHead == food) {
      score++;
      generateFood();
    } else {
      snake.pop_back();
    }
  }

  void renderGame() {
    auto& display = Scene::getEngine()->display;
    display.clear();
    if (!gameStarted) {
      Display::TextOptions textOptions;
      textOptions._color = 0x00;
      textOptions._origin = Display::Origin::Text::BASELINE_LEFT;
      textOptions._font = Display::Font::intel_one_mono_8_pt;
      display.drawText(30, 20, "SNAKE", textOptions);
      display.drawText(10, 35, "Use D-PAD to move", textOptions);
      display.drawText(10, 48, "Press any key to start", textOptions);
      display.drawText(10, 61, "LEFT to exit", textOptions);
      display.update();
      return;
    }
    for (const Point& segment : snake) {
      display.fillRectangle(segment.x * GRID_SIZE, segment.y * GRID_SIZE, GRID_SIZE - 1, GRID_SIZE - 1, Display::Object2DOptions().color(0x00));
    }
    display.fillCircle(food.x * GRID_SIZE + GRID_SIZE/2, food.y * GRID_SIZE + GRID_SIZE/2, GRID_SIZE/2, Display::Object2DOptions().color(0x00));
    Display::TextOptions textOptions;
    textOptions._color = 0x00;
    textOptions._origin = Display::Origin::Text::BASELINE_LEFT;
    textOptions._font = Display::Font::intel_one_mono_8_pt;
    char scoreStr[20];
    sprintf(scoreStr, "Score: %d", score);
    display.drawText(5, 10, scoreStr, textOptions);
    if (gameOver) {
      display.drawText(25, 35, "Game Over!", textOptions);
      display.drawText(15, 48, "RIGHT to restart", textOptions);
      display.drawText(15, 61, "LEFT to exit", textOptions);
    }
    display.update();
  }

public:
  SnakeScene() : Scene(false, true) {
    gameActor = new GameActor(this);
  }
  ~SnakeScene() { delete gameActor; }
  void onEnter() override {
    initializeGame();
    add(gameActor);
  }
  void onExit() override {
    remove(gameActor);
  }
};

// Export a global instance for the launcher
define SNAKE_SCENE_CPP
SnakeScene snakeScene;
