// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * Sprite Animation Example for Kywy
 * 
 * LEARNING OBJECTIVES:
 * - Create and control sprites
 * - Implement custom animation sequences
 * - Handle user input via event system
 * - Object-oriented design with Actors
 * 
 * WHAT THIS EXAMPLE DOES:
 * - Walking animation: Frames 1,4,5 when moving
 * - Button animation: Frames 2,3,2,3 when right button pressed
 * - Movement blocked during button animation
 * 
 * CONTROLS:
 * - D-Pad: Move with walking animation
 * - Right Button: Play action animation (blocks movement)
 * 
 * LIBRARY FEATURES:
 * - Kywy::Engine: Manages display, input, clock subsystems
 * - Actor::Actor: Event-driven base class for message handling
 * - Sprite: Handles animation frames, positioning, rendering
 * - Event system: Asynchronous messages for input and timing
 * 
 * FLEXIBILITY:
 * - Actors compose objects (e.g., SpriteController manages Sprite)
 * - Easy to extend with new events, animations, or multiple actors
 * - Modular: Separate movement and animation logic
 * - Reusable: Components work across projects
 */

#include "Kywy.hpp"
#include "my_animation.hpp"

// === CONFIGURATION ===
const int SPRITE_MOVE_SPEED = 1;          // Pixels to move per frame
const int ANIMATION_TICKS_PER_FRAME = 5;  // Update animation every 5 ticks

// === GLOBAL VARIABLES ===
// Kywy Engine: Manages display, input, clock subsystems
Kywy::Engine engine;

// === SPRITE CONTROLLER ACTOR ===
// Inherits from Actor::Actor for event handling. Manages Sprite and coordinates movement/animation.
class SpriteController : public Actor::Actor {
public:
  Sprite *sprite = nullptr;  // Handles animation frames, positioning, rendering

  // Movement state: Tracks pressed D-pad directions
  bool movingUp = false;
  bool movingDown = false;
  bool movingLeft = false;
  bool movingRight = false;

  bool playAnimation = false;  // Controls animation sequence

  // Animation timing
  uint8_t animationTicks = 0;  // Frame progression counter
  uint8_t frameTick = 0;       // Tick counter between frame changes

  void initialize() {
    // Create sprite with animation frames
    sprite = new Sprite(
      my_animation_frames,
      MY_ANIMATION_FRAME_COUNT,
      MY_ANIMATION_WIDTH,
      MY_ANIMATION_HEIGHT);

    // Connect to display and position centrally
    sprite->setDisplay(&engine.display);
    sprite->setPosition(
      (KYWY_DISPLAY_WIDTH - MY_ANIMATION_WIDTH) / 2,
      (KYWY_DISPLAY_HEIGHT - MY_ANIMATION_HEIGHT) / 2);
    sprite->setVisible(true);
  }

  void handle(::Actor::Message *message) {
    // Event-driven message handling for input and timing
    switch (message->signal) {
      // === D-PAD MOVEMENT EVENTS ===
      // Input events for D-pad presses/releases
      case Kywy::Events::D_PAD_UP_PRESSED:
        movingUp = true;
        break;

      case Kywy::Events::D_PAD_UP_RELEASED:
        movingUp = false;
        break;

      case Kywy::Events::D_PAD_DOWN_PRESSED:
        movingDown = true;
        break;

      case Kywy::Events::D_PAD_DOWN_RELEASED:
        movingDown = false;
        break;

      case Kywy::Events::D_PAD_LEFT_PRESSED:
        movingLeft = true;
        break;

      case Kywy::Events::D_PAD_LEFT_RELEASED:
        movingLeft = false;
        break;

      case Kywy::Events::D_PAD_RIGHT_PRESSED:
        movingRight = true;
        break;

      case Kywy::Events::D_PAD_RIGHT_RELEASED:
        movingRight = false;
        break;

      // === ANIMATION CONTROL ===
      // Button events trigger animation state changes
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        playAnimation = true;
        animationTicks = 0;
        frameTick = 0;
        break;

      case Kywy::Events::BUTTON_RIGHT_RELEASED:
        playAnimation = false;
        sprite->setFrame(0);
        break;

      // === TICK EVENT ===
      // Clock events for game logic updates
      case Kywy::Events::TICK:
        updateMovement();
        updateAnimation();
        sprite->render();
        engine.display.update();
        break;
    }
  }

private:
  void updateMovement() {
    // Block movement during button animation
    if (playAnimation) {
      return;
    }

    // Calculate new position based on current D-pad state
    int16_t newX = sprite->x;
    int16_t newY = sprite->y;

    // Accumulate movement deltas
    int8_t deltaX = 0;
    int8_t deltaY = 0;
    if (movingLeft) deltaX -= SPRITE_MOVE_SPEED;
    if (movingRight) deltaX += SPRITE_MOVE_SPEED;
    if (movingUp) deltaY -= SPRITE_MOVE_SPEED;
    if (movingDown) deltaY += SPRITE_MOVE_SPEED;

    // Apply movement and clamp to screen bounds
    if (deltaX != 0 || deltaY != 0) {
      newX += deltaX;
      newY += deltaY;

      if (newX < 0) newX = 0;
      if (newX > KYWY_DISPLAY_WIDTH - MY_ANIMATION_WIDTH) newX = KYWY_DISPLAY_WIDTH - MY_ANIMATION_WIDTH;
      if (newY < 0) newY = 0;
      if (newY > KYWY_DISPLAY_HEIGHT - MY_ANIMATION_HEIGHT) newY = KYWY_DISPLAY_HEIGHT - MY_ANIMATION_HEIGHT;

      sprite->setPosition(newX, newY);
    }
  }

  void updateAnimation() {
    // Animation sequences
    static const uint8_t walkSequence[] = { 0, 3, 4 };       // Frames 1, 4, 5
    static const uint8_t actionSequence[] = { 1, 2, 1, 2 };  // Frames 2, 3, 2, 3

    // Change frames every N ticks
    frameTick++;
    if (frameTick >= ANIMATION_TICKS_PER_FRAME) {
      frameTick = 0;
      if (playAnimation) {
        sprite->setFrame(actionSequence[animationTicks % 4]);
        animationTicks++;
      } else if (movingUp || movingDown || movingLeft || movingRight) {
        sprite->setFrame(walkSequence[animationTicks % 3]);
        animationTicks++;
      }
    }

    // Reset to idle when not animating
    if (!playAnimation && !(movingUp || movingDown || movingLeft || movingRight)) {
      sprite->setFrame(0);
      animationTicks = 0;
      frameTick = 0;
    }
  }
};

// Create the sprite controller actor
SpriteController spriteController;

void setup() {
  // Initialize Kywy engine
  engine.start();
  engine.display.clear();

  // Initialize actor and subscribe to input/clock
  spriteController.initialize();
  spriteController.subscribe(&engine.input);
  spriteController.subscribe(&engine.clock);
  spriteController.start();
}

void loop() {
  // Engine handles events asynchronously
}
