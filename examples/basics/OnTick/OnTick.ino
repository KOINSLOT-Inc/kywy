// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// OnTick Hook Example
// Simple ball movement with input velocity control

#include "Kywy.hpp"

Kywy::Engine engine;

// Simple game state
float ballX = 72;  // Center of screen
float ballY = 84;  // Center of screen
float velocityX = 0;
float velocityY = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting OnTick Example");
  engine.clock.setTickDuration(33);  // Set tick duration to ~30 FPS
  engine.start();
  Serial.println("Engine started");
}

void loop() {
  // Engine handles everything automatically!
  Serial.println("still looping...");
  delay(100);
}

// Called every frame - handle movement and drawing
void onTick(Actor::Message *message) {
  // Update ball position based on velocity
  ballX += velocityX;
  ballY += velocityY;

  // Keep ball on screen
  if (ballX < 5) ballX = 5;
  if (ballX > KYWY_DISPLAY_WIDTH - 5) ballX = KYWY_DISPLAY_WIDTH - 5;
  if (ballY < 5) ballY = 5;
  if (ballY > KYWY_DISPLAY_HEIGHT - 5) ballY = KYWY_DISPLAY_HEIGHT - 5;

  // Clear and draw
  engine.display.clear();
  engine.display.fillCircle((int)ballX, (int)ballY, 5);

  engine.display.update();
}

// Called when input changes - control velocity
void onInput(Actor::Message *message) {
  Serial.print("Input event received: ");
  Serial.println(message->signal);

  // Handle specific input events using switch case
  switch (message->signal) {
    case Kywy::Events::D_PAD_LEFT_PRESSED:
      velocityX = -2;
      Serial.println("Moving left");
      break;

    case Kywy::Events::D_PAD_RIGHT_PRESSED:
      velocityX = 2;
      Serial.println("Moving right");
      break;

    case Kywy::Events::D_PAD_UP_PRESSED:
      velocityY = -2;
      Serial.println("Moving up");
      break;

    case Kywy::Events::D_PAD_DOWN_PRESSED:
      velocityY = 2;
      Serial.println("Moving down");
      break;

    case Kywy::Events::D_PAD_LEFT_RELEASED:
      velocityX = 0;
      Serial.println("Stopped horizontal movement");
      break;

    case Kywy::Events::D_PAD_RIGHT_RELEASED:
      velocityX = 0;
      Serial.println("Stopped horizontal movement");
      break;

    case Kywy::Events::D_PAD_UP_RELEASED:
      velocityY = 0;
      Serial.println("Stopped vertical movement");
      break;

    case Kywy::Events::D_PAD_DOWN_RELEASED:
      velocityY = 0;
      Serial.println("Stopped vertical movement");
      break;

    case Kywy::Events::BUTTON_LEFT_PRESSED:
      ballX = 72;
      ballY = 84;
      velocityX = 0;
      velocityY = 0;
      Serial.println("Reset position");
      break;

    default:
      // Show unhandled input events for debugging
      Serial.print("Unhandled input event: ");
      Serial.println(message->signal);
      break;
  }
}
