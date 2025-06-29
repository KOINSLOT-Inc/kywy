// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// A basic example of "scenes"
//
// Notes:
//   - scenes are just groupings of actors
//   - more than one scene can be active at a time
//   - scenes manage enabling and disabling groupings of actors
//
// This example:
//   - has two scenes
//   - one that has two actors that each draw a circle on the top half of the screen
//   - the other has one actor that draws two circles on the bottom half of the screen
//   - the manager enables or disables the scenes based on button input
//   - the actors respond to a SCENE_EXIT event by erasing their circles

#include "Kywy.hpp"

Kywy::Engine engine;

class TopLeftCircle : public Actor::Actor {
public:
  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::TICK:
        engine.display.fillCircle(30, 30, 50, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
        break;
      case Kywy::Events::SCENE_EXIT:  // erase circles on exit
        engine.display.fillCircle(30, 30, 50, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER).color(WHITE));
        break;
    }
  }

} topLeftCircle;

class TopRightCircle : public Actor::Actor {
public:
  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::TICK:
        engine.display.fillCircle(KYWY_DISPLAY_WIDTH - 30, 30, 50, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
        break;
      case Kywy::Events::SCENE_EXIT:  // erase circles on exit
        engine.display.fillCircle(KYWY_DISPLAY_WIDTH - 30, 30, 50, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER).color(WHITE));
        break;
    }
  }

} topRightCircle;

class BottomCircles : public Actor::Actor {
public:
  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::TICK:
        engine.display.fillCircle(30, KYWY_DISPLAY_HEIGHT - 30, 50, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
        engine.display.fillCircle(KYWY_DISPLAY_WIDTH - 30, KYWY_DISPLAY_HEIGHT - 30, 50, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
        break;
      case Kywy::Events::SCENE_EXIT:  // erase circles on exit
        engine.display.fillCircle(30, KYWY_DISPLAY_HEIGHT - 30, 50, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER).color(WHITE));
        engine.display.fillCircle(KYWY_DISPLAY_WIDTH - 30, KYWY_DISPLAY_HEIGHT - 30, 50, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER).color(WHITE));
        break;
    }
  }

} bottomCircles;

Scene topCirclesScene;
Scene bottomCirclesScene;

class manager : public Actor::Actor {
public:
  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::TICK:
        engine.display.update();
        break;
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        topCirclesScene.enter();
        break;
      case Kywy::Events::BUTTON_LEFT_RELEASED:
        topCirclesScene.exit();
        break;
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        bottomCirclesScene.enter();
        break;
      case Kywy::Events::BUTTON_RIGHT_RELEASED:
        bottomCirclesScene.exit();
        break;
    }
  }

} manager;

void setup() {
  engine.start();
  engine.display.clear();

  topLeftCircle.subscribe(&engine.clock);
  topRightCircle.subscribe(&engine.clock);
  bottomCircles.subscribe(&engine.clock);

  manager.subscribe(&engine.input);
  manager.subscribe(&engine.clock);

  topCirclesScene.add(&topLeftCircle);
  topCirclesScene.add(&topRightCircle);

  bottomCirclesScene.add(&bottomCircles);

  topLeftCircle.start();
  topRightCircle.start();
  bottomCircles.start();
  manager.start();
}

void loop() {
  delay(1000);
}
