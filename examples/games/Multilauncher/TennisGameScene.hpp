// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_TENNIS_GAME_SCENE_HPP
#define KYWY_TENNIS_GAME_SCENE_HPP

#include "Kywy.hpp"

namespace Kywy {

/**
 * 2D Tennis Game Scene - Wraps the original 2DTennis game as a Scene
 */
class TennisGameScene : public Scene {
private:
  // Game state variables (copied from original 2DTennis.ino)
  bool startScreen = true;
  bool gameOver = false;
  int player1Score = 0;
  int player2Score = 0;

  static const int paddleWidth = 4;
  static const int paddleHeight = 20;
  static const int screenBorder = 2;
  static const int ballRadius = 2;

  int player1PaddleY = KYWY_DISPLAY_HEIGHT / 2 - paddleHeight / 2;
  int player2PaddleY = KYWY_DISPLAY_HEIGHT / 2 - paddleHeight / 2;

  int ballX = KYWY_DISPLAY_WIDTH / 2;
  int ballY = KYWY_DISPLAY_HEIGHT / 2;
  int ballXVelocity = 2;
  int ballYVelocity = 1;

  // Clock subscription for game timing
  bool subscribedToClock = false;
  
  // Game timing
  float gameSpeed = 60.0f; // Target FPS
  float timeSinceLastUpdate = 0.0f;
  float updateInterval = 1.0f / gameSpeed; // Update every ~16ms (60 FPS)
  
  // Exit callback for returning to launcher
  std::function<void()> exitCallback;

  // Tennis splash screen (simplified - could include bitmap from original)
  // For now, we'll create a simple text-based splash

public:
  TennisGameScene() : Scene("TennisGame") {}
  
  void setExitCallback(std::function<void()> callback) {
    exitCallback = callback;
  }
  
  void enter() override {
    Scene::enter();
    startScreen = true;
    gameOver = false;
    subscribedToClock = false;
    
    // Don't show splash screen here - let render() handle it
  }
  
  void exit() override {
    Scene::exit();
    if (subscribedToClock) {
      subscribedToClock = false;
    }
  }

  void showSplashScreen(Display::Display& display) {
    display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    display.drawText(45, 30, "2D TENNIS", textOpt);
    display.drawText(20, 50, "Up/Down to move", textOpt);
    display.drawText(20, 65, "Press RIGHT (A) to start", textOpt);
    display.drawText(20, 80, "Press LEFT (B) to exit", textOpt);
    // Don't call display.update() here - let SceneManager handle it
  }

  void drawScore() {
    extern Engine engine;
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    char scoreText[16];
    snprintf(scoreText, sizeof(scoreText), "%d - %d", player1Score, player2Score);
    engine.display.drawText(KYWY_DISPLAY_WIDTH / 2 - 15, 10, scoreText, textOpt);
  }

  void drawPaddlesAndBall() {
    extern Engine engine;
    
    // Draw player 1 paddle (left side)
    for (int x = screenBorder; x < screenBorder + paddleWidth; x++) {
      for (int y = player1PaddleY; y < player1PaddleY + paddleHeight; y++) {
        engine.display.drawPixel(x, y, BLACK);
      }
    }

    // Draw player 2 paddle (right side)
    int player2PaddleX = KYWY_DISPLAY_WIDTH - screenBorder - paddleWidth;
    for (int x = player2PaddleX; x < player2PaddleX + paddleWidth; x++) {
      for (int y = player2PaddleY; y < player2PaddleY + paddleHeight; y++) {
        engine.display.drawPixel(x, y, BLACK);
      }
    }

    // Draw center line
    for (int y = 0; y < KYWY_DISPLAY_HEIGHT; y += 4) {
      engine.display.drawPixel(KYWY_DISPLAY_WIDTH / 2, y, BLACK);
      engine.display.drawPixel(KYWY_DISPLAY_WIDTH / 2, y + 1, BLACK);
    }

    // Draw ball
    for (int x = ballX - ballRadius; x < ballX + ballRadius; x++) {
      for (int y = ballY - ballRadius; y < ballY + ballRadius; y++) {
        engine.display.drawPixel(x, y, BLACK);
      }
    }
  }

  void updateBall() {
    // Move ball
    ballX += ballXVelocity;
    ballY += ballYVelocity;

    // Ball collision with top/bottom walls
    if (ballY <= screenBorder + ballRadius || 
        ballY >= KYWY_DISPLAY_HEIGHT - screenBorder - ballRadius) {
      ballYVelocity = -ballYVelocity;
    }

    // Ball collision with paddles
    // Player 1 paddle
    if (ballX <= screenBorder + paddleWidth + ballRadius &&
        ballY >= player1PaddleY && ballY <= player1PaddleY + paddleHeight) {
      ballXVelocity = abs(ballXVelocity);
    }

    // Player 2 paddle
    int player2PaddleX = KYWY_DISPLAY_WIDTH - screenBorder - paddleWidth;
    if (ballX >= player2PaddleX - ballRadius &&
        ballY >= player2PaddleY && ballY <= player2PaddleY + paddleHeight) {
      ballXVelocity = -abs(ballXVelocity);
    }

    // Scoring
    if (ballX <= 0) {
      player2Score++;
      resetBall();
      if (player2Score >= 5) {
        gameOver = true;
      }
    } else if (ballX >= KYWY_DISPLAY_WIDTH) {
      player1Score++;
      resetBall();
      if (player1Score >= 5) {
        gameOver = true;
      }
    }
  }

  void resetBall() {
    ballX = KYWY_DISPLAY_WIDTH / 2;
    ballY = KYWY_DISPLAY_HEIGHT / 2;
    ballXVelocity = (ballXVelocity > 0) ? -2 : 2;
    ballYVelocity = (rand() % 3) - 1;  // -1, 0, or 1
  }

  void updatePaddles() {
    // Simple AI for player 2
    if (ballY > player2PaddleY + paddleHeight / 2) {
      player2PaddleY += 2;
    } else if (ballY < player2PaddleY + paddleHeight / 2) {
      player2PaddleY -= 2;
    }

    // Keep paddles on screen
    if (player1PaddleY < screenBorder) {
      player1PaddleY = screenBorder;
    }
    if (player1PaddleY + paddleHeight > KYWY_DISPLAY_HEIGHT - screenBorder) {
      player1PaddleY = KYWY_DISPLAY_HEIGHT - screenBorder - paddleHeight;
    }
    if (player2PaddleY < screenBorder) {
      player2PaddleY = screenBorder;
    }
    if (player2PaddleY + paddleHeight > KYWY_DISPLAY_HEIGHT - screenBorder) {
      player2PaddleY = KYWY_DISPLAY_HEIGHT - screenBorder - paddleHeight;
    }
  }

  void startGame() {
    startScreen = false;
    gameOver = false;
    player1Score = 0;
    player2Score = 0;
    player1PaddleY = KYWY_DISPLAY_HEIGHT / 2 - paddleHeight / 2;
    player2PaddleY = KYWY_DISPLAY_HEIGHT / 2 - paddleHeight / 2;
    resetBall();
    
    if (!subscribedToClock) {
      subscribedToClock = true;
    }
  }

  void showGameOver(Display::Display& display) {
    display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    if (player1Score > player2Score) {
      display.drawText(40, 50, "PLAYER 1 WINS!", textOpt);
    } else {
      display.drawText(40, 50, "PLAYER 2 WINS!", textOpt);
    }
    
    char scoreText[16];
    snprintf(scoreText, sizeof(scoreText), "Final: %d - %d", player1Score, player2Score);
    display.drawText(30, 70, scoreText, textOpt);
    
    display.drawText(30, 90, "Press A to restart", textOpt);
    display.drawText(30, 105, "Press B to exit", textOpt);
    // Don't call display.update() here - let SceneManager handle it
  }

  void render(Display::Display& display) override {
    if (startScreen) {
      showSplashScreen(display);
      return;
    }
    
    if (gameOver) {
      showGameOver(display);
      return;
    }
    
    display.clear();
    drawScore();
    drawPaddlesAndBall();
    // Don't call display.update() here - let SceneManager handle it
  }

  void handleInput(int signal, void* data) override {
    switch (signal) {
      case Events::D_PAD_UP_PRESSED:
        if (!startScreen && !gameOver) {
          player1PaddleY -= 5;
        }
        break;
        
      case Events::D_PAD_DOWN_PRESSED:
        if (!startScreen && !gameOver) {
          player1PaddleY += 5;
        }
        break;
        
      case Events::BUTTON_RIGHT_PRESSED:  // A button
        if (startScreen || gameOver) {
          startGame();
        }
        break;
        
      case Events::BUTTON_LEFT_PRESSED:  // B button
        if (startScreen || gameOver) {
          // Exit to launcher
          if (exitCallback) {
            exitCallback();
          }
        }
        break;
    }
  }

  void update(float deltaTime) override {
    if (startScreen || gameOver) return;
    
    // Add deltaTime to accumulator
    timeSinceLastUpdate += deltaTime;
    
    // Only update at target frame rate
    if (timeSinceLastUpdate >= updateInterval) {
      updateBall();
      updatePaddles();
      timeSinceLastUpdate = 0.0f;
      
      if (gameOver) {
        // Game over state will be handled in render()
      }
    }
  }
};

}  // namespace Kywy

#endif  // KYWY_TENNIS_GAME_SCENE_HPP
