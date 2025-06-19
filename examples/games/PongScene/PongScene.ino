// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * PongScene.ino
 * 
 * A complete Pong game implementation using the Scene Manager system.
 * Demonstrates advanced scene usage with sprites, actors, and game logic.
 */

#include "Kywy.hpp"

using namespace Kywy;

Engine engine;
std::unique_ptr<SceneManager> sceneManager;

//==============================================================================
// Pong Game Logic Actor
//==============================================================================

class PongGameLogic : public Actor::Actor {
private:
  // Ball properties
  float ballX = KYWY_DISPLAY_WIDTH / 2.0f;
  float ballY = KYWY_DISPLAY_HEIGHT / 2.0f;
  float ballVelX = 2.0f;
  float ballVelY = 1.5f;
  const float ballSize = 3.0f;
  
  // Paddle properties
  float leftPaddleY = KYWY_DISPLAY_HEIGHT / 2.0f;
  float rightPaddleY = KYWY_DISPLAY_HEIGHT / 2.0f;
  const float paddleWidth = 3.0f;
  const float paddleHeight = 20.0f;
  const float paddleSpeed = 3.0f;
  
  // Game state
  int leftScore = 0;
  int rightScore = 0;
  bool gameRunning = true;
  
  // Input state
  bool leftUpPressed = false;
  bool leftDownPressed = false;
  bool rightUpPressed = false;
  bool rightDownPressed = false;
  
public:
  void handle(::Actor::Message* message) override {
    switch (message->signal) {
      case Kywy::Events::TICK:
        if (gameRunning) {
          updateGame();
        }
        break;
        
      case Kywy::Events::D_PAD_UP_PRESSED:
        leftUpPressed = true;
        break;
      case Kywy::Events::D_PAD_UP_RELEASED:
        leftUpPressed = false;
        break;
      case Kywy::Events::D_PAD_DOWN_PRESSED:
        leftDownPressed = true;
        break;
      case Kywy::Events::D_PAD_DOWN_RELEASED:
        leftDownPressed = false;
        break;
        
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        rightUpPressed = true;
        break;
      case Kywy::Events::BUTTON_RIGHT_RELEASED:
        rightUpPressed = false;
        break;
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        rightDownPressed = true;
        break;
      case Kywy::Events::BUTTON_LEFT_RELEASED:
        rightDownPressed = false;
        break;
    }
  }
  
  void updateGame() {
    // Update paddle positions
    if (leftUpPressed && leftPaddleY > paddleHeight/2) {
      leftPaddleY -= paddleSpeed;
    }
    if (leftDownPressed && leftPaddleY < KYWY_DISPLAY_HEIGHT - paddleHeight/2) {
      leftPaddleY += paddleSpeed;
    }
    
    if (rightUpPressed && rightPaddleY > paddleHeight/2) {
      rightPaddleY -= paddleSpeed;
    }
    if (rightDownPressed && rightPaddleY < KYWY_DISPLAY_HEIGHT - paddleHeight/2) {
      rightPaddleY += paddleSpeed;
    }
    
    // Update ball position
    ballX += ballVelX;
    ballY += ballVelY;
    
    // Ball collision with top/bottom
    if (ballY <= ballSize/2 || ballY >= KYWY_DISPLAY_HEIGHT - ballSize/2) {
      ballVelY = -ballVelY;
    }
    
    // Ball collision with paddles
    if (ballX <= 10 + paddleWidth && 
        ballY >= leftPaddleY - paddleHeight/2 && 
        ballY <= leftPaddleY + paddleHeight/2) {
      ballVelX = -ballVelX;
      ballX = 10 + paddleWidth + ballSize/2;
    }
    
    if (ballX >= KYWY_DISPLAY_WIDTH - 10 - paddleWidth && 
        ballY >= rightPaddleY - paddleHeight/2 && 
        ballY <= rightPaddleY + paddleHeight/2) {
      ballVelX = -ballVelX;
      ballX = KYWY_DISPLAY_WIDTH - 10 - paddleWidth - ballSize/2;
    }
    
    // Ball out of bounds (scoring)
    if (ballX < 0) {
      rightScore++;
      resetBall();
    } else if (ballX > KYWY_DISPLAY_WIDTH) {
      leftScore++;
      resetBall();
    }
    
    // Check for game over
    if (leftScore >= 5 || rightScore >= 5) {
      gameRunning = false;
    }
  }
  
  void resetBall() {
    ballX = KYWY_DISPLAY_WIDTH / 2.0f;
    ballY = KYWY_DISPLAY_HEIGHT / 2.0f;
    ballVelX = (ballVelX > 0) ? -2.0f : 2.0f;  // Reverse direction
    ballVelY = (rand() % 3 - 1) * 1.5f;  // Random Y velocity
  }
  
  void resetGame() {
    leftScore = 0;
    rightScore = 0;
    gameRunning = true;
    resetBall();
  }
  
  // Getters for rendering
  float getBallX() const { return ballX; }
  float getBallY() const { return ballY; }
  float getBallSize() const { return ballSize; }
  float getLeftPaddleY() const { return leftPaddleY; }
  float getRightPaddleY() const { return rightPaddleY; }
  float getPaddleWidth() const { return paddleWidth; }
  float getPaddleHeight() const { return paddleHeight; }
  int getLeftScore() const { return leftScore; }
  int getRightScore() const { return rightScore; }
  bool isGameRunning() const { return gameRunning; }
};

//==============================================================================
// Pong Game Scene
//==============================================================================

class PongGameScene : public Scene {
private:
  std::shared_ptr<PongGameLogic> gameLogic;
  
public:
  PongGameScene() : Scene("PongGame") {
    gameLogic = std::make_shared<PongGameLogic>();
  }
  
  void initialize() override {
    Scene::initialize();
    addActor(gameLogic);
  }
  
  void enter() override {
    Scene::enter();
    gameLogic->resetGame();
    
    // Subscribe game logic to input events
    gameLogic->subscribe(&engine.input);
    gameLogic->subscribe(&engine.clock);
  }
  
  void render(Display::Display& display) override {
    display.clear();
    
    // Draw center line
    for (int y = 0; y < KYWY_DISPLAY_HEIGHT; y += 8) {
      display.drawPixel(KYWY_DISPLAY_WIDTH/2, y, BLACK);
      display.drawPixel(KYWY_DISPLAY_WIDTH/2, y+1, BLACK);
    }
    
    // Draw ball
    int ballX = static_cast<int>(gameLogic->getBallX());
    int ballY = static_cast<int>(gameLogic->getBallY());
    int ballSize = static_cast<int>(gameLogic->getBallSize());
    
    for (int x = ballX - ballSize/2; x <= ballX + ballSize/2; x++) {
      for (int y = ballY - ballSize/2; y <= ballY + ballSize/2; y++) {
        if (x >= 0 && x < KYWY_DISPLAY_WIDTH && y >= 0 && y < KYWY_DISPLAY_HEIGHT) {
          display.drawPixel(x, y, BLACK);
        }
      }
    }
    
    // Draw left paddle
    int leftPaddleY = static_cast<int>(gameLogic->getLeftPaddleY());
    int paddleHeight = static_cast<int>(gameLogic->getPaddleHeight());
    int paddleWidth = static_cast<int>(gameLogic->getPaddleWidth());
    
    for (int x = 10; x < 10 + paddleWidth; x++) {
      for (int y = leftPaddleY - paddleHeight/2; y <= leftPaddleY + paddleHeight/2; y++) {
        if (y >= 0 && y < KYWY_DISPLAY_HEIGHT) {
          display.drawPixel(x, y, BLACK);
        }
      }
    }
    
    // Draw right paddle
    int rightPaddleY = static_cast<int>(gameLogic->getRightPaddleY());
    
    for (int x = KYWY_DISPLAY_WIDTH - 10 - paddleWidth; x < KYWY_DISPLAY_WIDTH - 10; x++) {
      for (int y = rightPaddleY - paddleHeight/2; y <= rightPaddleY + paddleHeight/2; y++) {
        if (y >= 0 && y < KYWY_DISPLAY_HEIGHT) {
          display.drawPixel(x, y, BLACK);
        }
      }
    }
    
    // Draw scores
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    char scoreText[16];
    snprintf(scoreText, sizeof(scoreText), "%d  %d", 
             gameLogic->getLeftScore(), gameLogic->getRightScore());
    display.drawText(KYWY_DISPLAY_WIDTH/2 - 15, 15, scoreText, textOpt);
    
    // Show game over message
    if (!gameLogic->isGameRunning()) {
      display.drawText(30, 80, "GAME OVER!", textOpt);
      display.drawText(20, 100, "Press UP to restart", textOpt);
      display.drawText(20, 120, "Press DOWN for menu", textOpt);
    } else {
      // Show controls
      display.drawText(5, KYWY_DISPLAY_HEIGHT - 10, "^v", textOpt);
      display.drawText(KYWY_DISPLAY_WIDTH - 20, KYWY_DISPLAY_HEIGHT - 10, "<>", textOpt);
    }
    
    display.update();
  }
  
  void handleInput(int signal, void* data) override {
    if (!gameLogic->isGameRunning()) {
      // Game over input handling
      switch (signal) {
        case Kywy::Events::D_PAD_UP_PRESSED:
          gameLogic->resetGame();
          break;
        case Kywy::Events::D_PAD_DOWN_PRESSED:
          sceneManager->popScene(SceneTransition(TransitionType::SLIDE_RIGHT, 300));
          break;
      }
    }
    // During gameplay, input is handled by the game logic actor
  }
  
  void exit() override {
    // Unsubscribe game logic from events
    gameLogic->unsubscribe(&engine.input);
    gameLogic->unsubscribe(&engine.clock);
    Scene::exit();
  }
};

//==============================================================================
// Main Menu
//==============================================================================

std::shared_ptr<MenuScene> createMainMenu() {
  std::vector<MenuSystem::MenuItem> menuItems = {
    MenuSystem::createLabel("-- PONG GAME --"),
    MenuSystem::createAction("Start Game", []() {
      auto pongScene = std::make_shared<PongGameScene>();
      sceneManager->pushScene(pongScene, SceneTransition(TransitionType::FADE_TO_BLACK, 400));
    }),
    MenuSystem::createAction("How to Play", []() {
      // Create instructions overlay
      class InstructionsScene : public Scene {
      public:
        InstructionsScene() : Scene("Instructions") {
          isOverlay = true;
          pauseUnderlying = true;
        }
        
        void render(Display::Display& display) override {
          display.clear();
          
          Display::TextOptions textOpt;
          textOpt._color = BLACK;
          textOpt._font = Display::Font::intel_one_mono_8_pt;
          
          display.drawText(20, 20, "HOW TO PLAY", textOpt);
          display.drawText(10, 40, "Left Player:", textOpt);
          display.drawText(10, 55, "  UP/DOWN arrows", textOpt);
          display.drawText(10, 75, "Right Player:", textOpt);
          display.drawText(10, 90, "  LEFT/RIGHT buttons", textOpt);
          display.drawText(10, 110, "First to 5 wins!", textOpt);
          display.drawText(10, 130, "Press any key", textOpt);
          display.drawText(10, 145, "to continue", textOpt);
          
          display.update();
        }
        
        void handleInput(int signal, void* data) override {
          sceneManager->popScene(SceneTransition(TransitionType::SLIDE_DOWN, 200));
        }
      };
      
      auto instructionsScene = std::make_shared<InstructionsScene>();
      sceneManager->pushScene(instructionsScene, SceneTransition(TransitionType::SLIDE_UP, 200));
    }),
    MenuSystem::createAction("Exit", []() {
      engine.display.clear();
      Display::TextOptions textOpt;
      textOpt._color = BLACK;
      textOpt._font = Display::Font::intel_one_mono_8_pt;
      engine.display.drawText(40, 80, "Thanks for", textOpt);
      engine.display.drawText(45, 95, "playing!", textOpt);
      engine.display.update();
      delay(2000);
    })
  };
  
  MenuSystem::MenuOptions menuOptions;
  menuOptions.x = 15;
  menuOptions.y = 25;
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
  Serial.println("Pong Game Starting...");
  
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
  
  Serial.println("Pong Game Ready!");
}

void loop() {
  delay(16);  // ~60 FPS
}
