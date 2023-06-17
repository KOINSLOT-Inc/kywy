// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "esp_random.h"
#include <math.h>

#include <Kywy.hpp>

Kywy::Engine engine;

// binary data for slime frames, 3x1 sprite sheet encoded with 1 bit per pixel (monochrome)
const static uint8_t spriteSheetData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00,
    0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00,
    0x00, 0x01, 0x80, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x0c, 0x30,
    0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x02,
    0x40, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00,
    0x0c, 0x30, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0x00, 0x00, 0xe0, 0x00, 0x0b, 0xd0, 0x00, 0x00, 0x05, 0xa0, 0x00,
    0x0c, 0x02, 0x40, 0x30, 0x00, 0x1a, 0x58, 0x00, 0x00, 0x0d, 0xb0, 0x00, 0x18, 0x0e, 0x70, 0x18, 0x00, 0x36, 0x6c,
    0x00, 0x00, 0x08, 0x10, 0x00, 0x10, 0x38, 0x1c, 0x08, 0x00, 0xe4, 0x27, 0x00, 0x00, 0x18, 0x18, 0x00, 0x10, 0x00,
    0x00, 0x08, 0x01, 0x80, 0x01, 0x80, 0x00, 0x30, 0x0c, 0x00, 0x30, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x00, 0x80, 0x00,
    0x20, 0x04, 0x00, 0x20, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0xc0, 0x00, 0x60, 0x06, 0x00, 0x20, 0x00, 0x00, 0x04,
    0x06, 0x00, 0x00, 0x60, 0x00, 0xc0, 0x03, 0x00, 0x20, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x20, 0x00, 0x80, 0x01,
    0x00, 0x20, 0x00, 0x00, 0x04, 0x0c, 0x00, 0x00, 0x30, 0x00, 0x80, 0x01, 0x00, 0x20, 0x00, 0x00, 0x04, 0x08, 0x00,
    0x00, 0x10, 0x01, 0x80, 0x01, 0x80, 0x20, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x80, 0x30,
    0x00, 0x00, 0x0c, 0x08, 0x00, 0x00, 0x10, 0x00, 0x80, 0x01, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xe0,
    0x00, 0x7f, 0xfe, 0x00};

// define slime sprite
SpriteSheet slimeSheet = ::SpriteSheet(spriteSheetData, 96, 32, 3);
const uint8_t *slimeFrames[] = {};
Sprite slime(slimeFrames, 3, 32, 32);

// define game specific events
typedef enum : uint16_t {
  START_SCREEN = Kywy::Events::USER_EVENTS, // start numbering after engine events
  PLATFORM_COLLISION,
  JUMP,
  GAME_OVER,
} SlimeJumperSignal;

// create actor that manages the slime sprite
class SlimeManager : public Actor::Actor {
public:
  int xMaxVelocity = 5;
  int xVelocity = 0;
  int xPosition = 48;

  int yMaxVelocity = -10; // note that origin is top left so negative velocity
                          // is up on the screen
  int yVelocity = yMaxVelocity;
  int yPosition = 90;

  int padding = 5;

  bool buttonLeftPressed = false;
  bool buttonRightPressed = false;

  const char *getName() { return "spriteManager"; };

  void initialize() {
    slime.setPosition(xPosition, yPosition);
    slime.setVisible(true);
    slime.render();
  }

  void handle(::Actor::Message *message) {
    switch (message->signal) {
    case Kywy::Events::D_PAD_LEFT_PRESSED:
      buttonLeftPressed = true;
      xVelocity = -1 * xMaxVelocity;
      break;
    case Kywy::Events::D_PAD_RIGHT_PRESSED:
      buttonRightPressed = true;
      xVelocity = xMaxVelocity;
      break;
    case Kywy::Events::D_PAD_LEFT_RELEASED:
      buttonLeftPressed = false;
      if (buttonRightPressed) {
        xVelocity = xMaxVelocity;
      } else {
        xVelocity = 0;
      }
      break;
    case Kywy::Events::D_PAD_RIGHT_RELEASED:
      buttonRightPressed = false;
      if (buttonLeftPressed) {
        xVelocity = -1 * xMaxVelocity;
      } else {
        xVelocity = 0;
      }
      break;
    case PLATFORM_COLLISION:
      if (yVelocity > 0) {
        yVelocity = yMaxVelocity;

        ::Actor::Message message(JUMP); // publish jump event so game manager can increment score
        publish(&message);
      }
      break;
    case Kywy::Events::TICK:
      // side to side motion
      xPosition += xVelocity;

      if (xPosition < padding) // don't allow past left edge of screen
        xPosition = padding;

      if (xPosition > 128 - 32 - padding) // don't allow past right edge of screen
        xPosition = 128 - 32 - padding;

      // falling
      yPosition += yVelocity;
      if (yVelocity < 7)
        yVelocity += 1;

      if (yPosition > 110) {
        ::Actor::Message message(GAME_OVER); // publish game over event for game manager to handle
        publish(&message);
      }

      // animation, set frame based on velocity to give the impression of jumping
      if (yVelocity < -5) {
        slime.setFrame(2);
      } else if (yVelocity < -2) {
        slime.setFrame(1);
      } else {
        slime.setFrame(0);
      }

      // apply changes from above
      slime.setPosition(xPosition, yPosition);
      slime.render();
      break;
    }
  }

} slimeManager;

// create actor to manage platforms
typedef struct Platform {
  int x, y, width;
} Platform;

class PlatformManager : public Actor::Actor {
public:
  const char *getName() { return "platformManager"; };

  const static int numPlatforms = 3;

  int yVelocity = 2;
  int platformWidth = 40;

  Platform platforms[numPlatforms];

  void initialize() {
    // start each platform at a random x value and space evenly along the y axis, all platforms start above the screen
    for (int i = 0; i < numPlatforms; i++) {
      platforms[i] = Platform{.x = (int)fmax(5, fmin(esp_random() & 0b1111111, 128 - 5 - platformWidth)),
                              .y = 110 - (50 * i),
                              .width = platformWidth};
    }
  }

  void drawPlatforms(uint16_t color) {
    for (int i = 0; i < numPlatforms; i++) {
      if (platforms[i].y >= 0 && platforms[i].y <= 128)
        engine.display.drawLine(platforms[i].x, platforms[i].y, platforms[i].x + platforms[i].width, platforms[i].y,
                                color);
    }
  }

  void handle(::Actor::Message *message) {
    switch (message->signal) {
    case Kywy::Events::TICK:
      drawPlatforms(0); // erase platforms

      // check for collision between bottom of slime sprite and any platform
      for (int i = 0; i < numPlatforms; i++) {

        int slimeBottom = slime.y + slime.height;
        int slimeSide = slime.x;

        if (slimeBottom > platforms[i].y + 3) // too far below platform
          continue;

        if (slimeBottom < platforms[i].y - 3) // too far above platform
          continue;

        if (slimeSide > platforms[i].x + platformWidth - 10) // too far to the right of platform
          continue;

        if (slimeSide < platforms[i].x - platformWidth + 10) // too far to the left of platform
          continue;

        ::Actor::Message message(PLATFORM_COLLISION); // publish collision event to be handled by slime manager
        publish(&message);
      }

      for (int i = 0; i < numPlatforms; i++) {
        platforms[i].y += yVelocity; // move platforms downwards

        // move any platforms too far below the screen back to the top and give a new random x position
        if (platforms[i].y > 150) {
          platforms[i].y = -5;
          platforms[i].x = (int)fmax(5, fmin(esp_random() & 0b1111111, 128 - 5 - platformWidth));
        }
      }

      drawPlatforms(8); // draw platforms
    }
  }

} platformManager;

// create an actor to manage the high level game loop
class GameManager : public Actor::Actor {
public:
  const char *getName() { return "gameManager"; };

  int score = 0;
  int highScore = 0;

  void drawScore(uint16_t color) {
    char msg[16];
    snprintf(msg, sizeof(msg), "%d", (uint16_t)score);
    engine.display.drawText(Display::Origin::Text::TOP_LEFT, 5, 5, Display::Font::intel_one_mono_8_pt, msg, color);
  }

  void initialize() {}

  void handle(::Actor::Message *message) {
    switch (message->signal) {
    case START_SCREEN: {
      // no clock events should trigger anything in the start screen
      slimeManager.unsubscribe(&engine.clock);
      platformManager.unsubscribe(&engine.clock);
      unsubscribe(&engine.clock);
      engine.display.clear();

      // draw title card
      char slimeJumper[] = "SLIME JUMPER";
      char pressLeftButton[] = "Press left button";
      char toStart[] = "to start.";
      engine.display.drawText(Display::Origin::Text::BASELINE_CENTER, 64, 54, Display::Font::intel_one_mono_12_pt,
                              slimeJumper, 0xf);
      engine.display.drawText(Display::Origin::Text::BASELINE_CENTER, 64, 82, Display::Font::intel_one_mono_8_pt,
                              pressLeftButton, 0xf);
      engine.display.drawText(Display::Origin::Text::BASELINE_CENTER, 64, 94, Display::Font::intel_one_mono_8_pt,
                              toStart, 0xf);
      engine.display.update();

      subscribe(&engine.input); // listen for user input
      break;
    }
    case Kywy::Events::TICK: // draw score and update the screen every tick
      drawScore(0xf);
      engine.display.update();
      break;
    case JUMP:
      drawScore(0x0); // erase current score, it will be redrawn next TICK
      score += 1;
      break;
    case GAME_OVER: {
      // no clock events shouldn't trigger anything in the game over screen
      slimeManager.unsubscribe(&engine.clock);
      platformManager.unsubscribe(&engine.clock);
      unsubscribe(&engine.clock);
      subscribe(&engine.input);

      if (score > highScore)
        highScore = score;

      // draw game over screen
      engine.display.fillRectangle(Display::Origin::Object2D::TOP_LEFT, 0, 0, 128, 128, 0x0);
      char gameOver[] = "GAME OVER";
      engine.display.drawText(Display::Origin::Text::TOP_LEFT, 5, 5, Display::Font::intel_one_mono_8_pt, gameOver, 0xf);
      char msg[32];
      snprintf(msg, sizeof(msg), "Score: %d", (uint16_t)score);
      engine.display.drawText(Display::Origin::Text::TOP_LEFT, 5, 15, Display::Font::intel_one_mono_8_pt, msg, 0xf);
      snprintf(msg, sizeof(msg), "High Score: %d", highScore);
      engine.display.drawText(Display::Origin::Text::TOP_LEFT, 5, 25, Display::Font::intel_one_mono_8_pt, msg, 0xf);
      char pressLeftButton[] = "Press left button";
      char toTryAgain[] = "to try again.";
      engine.display.drawText(Display::Origin::Text::TOP_LEFT, 5, 45, Display::Font::intel_one_mono_8_pt,
                              pressLeftButton, 0xf);
      engine.display.drawText(Display::Origin::Text::TOP_LEFT, 5, 55, Display::Font::intel_one_mono_8_pt, toTryAgain,
                              0xf);
      engine.display.update();
      break;
    }
    case Kywy::Events::D_PAD_LEFT_PRESSED: { // user event to start a new game
      unsubscribe(&engine.input);            // no longer need to listen for user input

      // reset platforms, slime, and score
      platformManager.initialize();
      slime.setPosition(48, 90);
      slimeManager.yVelocity = -12;
      score = 0;

      engine.display.clear();

      slimeManager.subscribe(&engine.clock);
      platformManager.subscribe(&engine.clock);

      // game manager MUST be subscribed to the clock last so that when a tick event is recieved and the display is
      // updated the other actors have already handled the even
      subscribe(&engine.clock);
      break;
    }
    }
  }
} gameManager;

extern "C" void app_main(void) {
  engine.start();

  // load sprite frames
  slimeSheet.addFrames(0, 0, 32, 32, 3);
  slime.frames = slimeSheet.frames;
  slime.setDisplay(&engine.display);

  // wire up and start actors
  slimeManager.subscribe(&engine.input);
  slimeManager.subscribe(&engine.clock);
  slimeManager.subscribe(&platformManager);
  slimeManager.start();

  platformManager.subscribe(&engine.clock);
  platformManager.start();

  gameManager.subscribe(&slimeManager);
  gameManager.subscribe(&engine.clock);
  gameManager.start();

  // send event to game manager to start the game
  ::Actor::Message message(START_SCREEN);
  gameManager.handle(&message);

  // "zero" cost loop, actors handle the rest of the game
  while (true) {
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
