// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// A basic example of "scenes"
//
// Notes:
//   - scenes are classes that manage groupings of actors
//   - more than one scene can be active at a time
//   - scenes manage enabling and disabling groupings of actors
//
// This example:
//   - has two scenes that can be active simultaneously
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
};

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
};

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
};

class TopCirclesScene : public Scene {
private:
  TopLeftCircle topLeftCircle;
  TopRightCircle topRightCircle;

public:
  TopCirclesScene() : Scene(false, false) {}  // not persistent, don't auto-clear display

  void onEnter() {
    // Add actors to the scene
    add(&topLeftCircle, false, false);  // don't own (stack allocated), don't need input
    add(&topRightCircle, false, false);
  }
};

class BottomCirclesScene : public Scene {
private:
  BottomCircles bottomCircles;

public:
  BottomCirclesScene() : Scene(false, false) {}  // not persistent, don't auto-clear display

  void onEnter() {
    // Add actors to the scene
    add(&bottomCircles, false, false);  // don't own (stack allocated), don't need input
  }
};

TopCirclesScene topCirclesScene;
BottomCirclesScene bottomCirclesScene;

class Manager : public Actor::Actor {
public:
  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::TICK:
        engine.display.update();
        break;
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        if (!topCirclesScene.isActive()) {
          topCirclesScene.enter();
        }
        break;
      case Kywy::Events::BUTTON_LEFT_RELEASED:
        if (topCirclesScene.isActive()) {
          topCirclesScene.exit();
        }
        break;
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        if (!bottomCirclesScene.isActive()) {
          bottomCirclesScene.enter();
        }
        break;
      case Kywy::Events::BUTTON_RIGHT_RELEASED:
        if (bottomCirclesScene.isActive()) {
          bottomCirclesScene.exit();
        }
        break;
    }
  }
} manager;

void setup() {
  engine.start();
  engine.display.clear();

  // Set the engine reference for scenes
  Scene::setEngine(&engine);

  // Subscribe manager to input and clock
  manager.subscribe(&engine.input);
  manager.subscribe(&engine.clock);

  // Start the manager
  manager.start();
}

void loop() {
  delay(1000);
}