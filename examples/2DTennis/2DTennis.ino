// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

const uint8_t splashScreen[] = {};

class gameManager : public Actor::Actor {
public:
  bool startScreen = true;
  int highScore = 0;

  int score, opponentScore;
  bool gameOver, inPoint;
  int yDirection;

  int ballX, ballY, ballXVelocity, ballYVelocity;

  static const int movesPerTick = 4;
  static const int ballRadius = 5;
  static const int paddleLength = 30;
  static const int paddleWidth = 5;
  static const int screenBorder = 2;

  int paddleY, opponentPaddleY;

  void preparePoint() {
    engine.display.clear();
    unsubscribe(&engine.clock);
    inPoint = false;
    paddleY = KYWY_DISPLAY_HEIGHT / 2 - paddleLength / 2;
    opponentPaddleY = paddleY;
    ballX = KYWY_DISPLAY_WIDTH / 2;
    ballY = KYWY_DISPLAY_HEIGHT / 2;
    ballXVelocity = 0;
    ballYVelocity = 0;
    yDirection = 0;
    drawPaddlesAndBall();
    drawScore();
  }

  void startGame() {
    gameOver = false;
    score = 0;
    opponentScore = 0;
    preparePoint();
    countdown();
  };

  void endGame() {
    unsubscribe(&engine.clock);
    gameOver = true;
    engine.display.clear();
    if (score >= 5) {
      engine.display.drawText(5, 5, "You win!");
    } else {
      engine.display.drawText(5, 5, "You lose...");
    }
    char msg[32];
    snprintf(msg, sizeof(msg), "Score: %d - %d", (uint16_t)score, (uint16_t)opponentScore);
    engine.display.drawText(5, 18, msg);
    engine.display.drawText(5, 45, "Press left button");
    engine.display.drawText(5, 55, "to try again.");
    engine.display.update();
  }

  void initialize() {
    engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, (uint8_t *)splashScreen);
    engine.display.update();
    subscribe(&engine.input);
  }

  void drawPaddlesAndBall() {
    engine.display.fillRectangle(screenBorder, paddleY, paddleWidth, paddleLength);
    engine.display.fillRectangle(KYWY_DISPLAY_WIDTH - screenBorder - paddleWidth, opponentPaddleY, paddleWidth, paddleLength);
    if (inPoint) {
      engine.display.fillCircle(ballX, ballY, ballRadius * 2 + 1, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
    }
  }

  void drawScore() {
    char msg[24];
    snprintf(msg, sizeof(msg), "%d - %d", (uint16_t)score, (uint16_t)opponentScore);
    engine.display.drawText(KYWY_DISPLAY_WIDTH / 2, 8, msg, Display::TextOptions().origin(Display::Origin::Text::CENTER));
  }

  void countdown() {
    engine.display.fillRectangle(KYWY_DISPLAY_WIDTH / 2, KYWY_DISPLAY_HEIGHT / 2, 10, 10, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER).color(WHITE));
    engine.display.drawText(KYWY_DISPLAY_WIDTH / 2, KYWY_DISPLAY_HEIGHT / 2, "3", Display::TextOptions().origin(Display::Origin::Text::CENTER));
    engine.display.update();
    delay(500);
    engine.display.fillRectangle(KYWY_DISPLAY_WIDTH / 2, KYWY_DISPLAY_HEIGHT / 2, 10, 10, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER).color(WHITE));
    engine.display.drawText(KYWY_DISPLAY_WIDTH / 2, KYWY_DISPLAY_HEIGHT / 2, "2", Display::TextOptions().origin(Display::Origin::Text::CENTER));
    engine.display.update();
    delay(500);
    engine.display.fillRectangle(KYWY_DISPLAY_WIDTH / 2, KYWY_DISPLAY_HEIGHT / 2, 10, 10, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER).color(WHITE));
    engine.display.drawText(KYWY_DISPLAY_WIDTH / 2, KYWY_DISPLAY_HEIGHT / 2, "1", Display::TextOptions().origin(Display::Origin::Text::CENTER));
    engine.display.update();
    delay(500);
    engine.display.fillRectangle(KYWY_DISPLAY_WIDTH / 2, KYWY_DISPLAY_HEIGHT / 2, 10, 10, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER).color(WHITE));
    engine.display.update();
    ballXVelocity = random(2) == 1 ? -5 : 5;
    ballYVelocity = (random(2) == 1 ? -1 : 1) * random(3);
    inPoint = true;
    subscribe(&engine.clock);
  }

  void handle(::Actor::Message *message) {
    int distanceFromMiddleOfOpponentPaddle;

    switch (message->signal) {
      case Kywy::Events::D_PAD_UP_PRESSED:
        yDirection = -1;
        break;
      case Kywy::Events::D_PAD_UP_RELEASED:
        yDirection = 0;
        break;
      case Kywy::Events::D_PAD_DOWN_PRESSED:
        yDirection = 1;
        break;
      case Kywy::Events::D_PAD_DOWN_RELEASED:
        yDirection = 0;
        break;
      case Kywy::Events::TICK:
        // move paddle
        paddleY += yDirection * movesPerTick;

        if (paddleY < screenBorder) {
          paddleY = screenBorder;
        }

        if (paddleY > KYWY_DISPLAY_HEIGHT - screenBorder - paddleLength) {
          paddleY = KYWY_DISPLAY_HEIGHT - screenBorder - paddleLength;
        }

        // move opponent paddle
        distanceFromMiddleOfOpponentPaddle = ballY - (opponentPaddleY + paddleLength / 2);
        if (distanceFromMiddleOfOpponentPaddle > 5) {
          opponentPaddleY += movesPerTick;
        } else if (distanceFromMiddleOfOpponentPaddle < -5) {
          opponentPaddleY -= movesPerTick;
        }

        // move ball
        ballX += ballXVelocity;
        ballY += ballYVelocity;

        if ((ballX - ballRadius <= screenBorder + paddleWidth) && (ballY >= paddleY - ballRadius) && (ballY <= paddleY + paddleLength + ballRadius)) {  // paddle collision
          ballX = screenBorder + paddleWidth + ballRadius - 2;                                                                                          // look like it hit the paddle

          ballXVelocity = abs(ballXVelocity);                                                                      // bounce back
          ballYVelocity = (int)(((float)(ballY - (paddleY + paddleLength / 2)) / (float)(paddleLength / 2)) * 5);  // change angle based on y velocity
        }

        if ((ballX + ballRadius >= KYWY_DISPLAY_WIDTH - screenBorder - paddleWidth) && (ballY >= opponentPaddleY - ballRadius) && (ballY <= opponentPaddleY + paddleLength + ballRadius)) {  // opponent paddle collision
          ballX = KYWY_DISPLAY_WIDTH - screenBorder - paddleWidth - ballRadius + 2;                                                                                                          // look like it hit the paddle

          ballXVelocity = -1 * abs(ballXVelocity);                                                                         // bounce back
          ballYVelocity = (int)(((float)(ballY - (opponentPaddleY + paddleLength / 2)) / (float)(paddleLength / 2)) * 5);  // change angle based on y velocity
        }

        if (ballX - ballRadius <= screenBorder) {  // left wall collision
          ballX = screenBorder + ballRadius;
          ballXVelocity = abs(ballXVelocity);
          opponentScore += 1;
          if (opponentScore >= 5) {
            endGame();
            return;
          }
          preparePoint();
          countdown();
          return;
        }

        if (ballX >= KYWY_DISPLAY_WIDTH - screenBorder - ballRadius) {  // right wall collision
          ballX = KYWY_DISPLAY_WIDTH - screenBorder - ballRadius;
          ballXVelocity = -1 * abs(ballXVelocity);
          score += 1;
          if (score >= 5) {
            endGame();
            return;
          }
          preparePoint();
          countdown();
          return;
        }

        if (ballY - ballRadius <= screenBorder) {  // top wall collision
          ballY = screenBorder + ballRadius;
          ballYVelocity = abs(ballYVelocity);
        }

        if (ballY >= KYWY_DISPLAY_HEIGHT - screenBorder - ballRadius) {  // bottom wall collision
          ballY = KYWY_DISPLAY_HEIGHT - screenBorder - ballRadius;
          ballYVelocity = -1 * abs(ballYVelocity);
        }


        engine.display.clear();
        drawScore();
        drawPaddlesAndBall();
        engine.display.update();
        break;
      case Kywy::Events::INPUT_PRESSED:
        if (!startScreen) {
          break;
        }

        startScreen = false;
        startGame();
        break;
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        if (!gameOver) {
          break;
        }
        startGame();
        break;
    }
  }

} gameManager;

void setup() {
  engine.start();
  gameManager.start();
}

void loop() {
  delay(1000);
}
