// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HARDWARE_TEST_SCENE_HPP
#define HARDWARE_TEST_SCENE_HPP

#include <Kywy.hpp>

using namespace Kywy;

class HardwareTestScene : public Scene {
private:
  // Counters for press events
  int leftCount = 0;
  int rightCount = 0;
  int upCount = 0;
  int downCount = 0;
  int leftDCount = 0;   // dpad left
  int rightDCount = 0;  // dpad right
  int centerCount = 0;

  // Completed (pressed then released) flags
  bool leftDone = false;
  bool rightDone = false;
  bool upDone = false;
  bool downDone = false;
  bool leftDDone = false;
  bool rightDDone = false;
  bool centerDone = false;

  // Last states for edge detection
  bool lastLeft = false;
  bool lastRight = false;
  bool lastDUp = false;
  bool lastDDown = false;
  bool lastDLeft = false;
  bool lastDRight = false;
  bool lastDCenter = false;

  unsigned long lastUpdate = 0;
  const unsigned long UPDATE_INTERVAL = 100;  // ms

  // Actor to poll on TICK messages
  class PollActor : public Actor::Actor {
  private:
    HardwareTestScene* scene;
  public:
    PollActor(HardwareTestScene* s)
      : Actor::Actor(), scene(s) {
      scene->Scene::add(this, false, false);
    }

    void handle(::Actor::Message* message) {
      if (!scene->isActive()) return;

      if (message->signal == Kywy::Events::TICK) {
        // Poll input and battery
        Engine* eng = Scene::getEngine();
        if (!eng) return;

        Input& input = eng->input;
        Battery& battery = eng->battery;

        // Edge detection for presses
        if (input.buttonLeftPressed && !scene->lastLeft) scene->leftCount++;
        if (input.buttonRightPressed && !scene->lastRight) scene->rightCount++;
        if (input.dPadUpPressed && !scene->lastDUp) scene->upCount++;
        if (input.dPadDownPressed && !scene->lastDDown) scene->downCount++;
        if (input.dPadLeftPressed && !scene->lastDLeft) scene->leftDCount++;
        if (input.dPadRightPressed && !scene->lastDRight) scene->rightDCount++;
        if (input.dPadCenterPressed && !scene->lastDCenter) scene->centerCount++;

        // Release detection (pressed then released => done)
        if (!input.buttonLeftPressed && scene->lastLeft) scene->leftDone = true;
        if (!input.buttonRightPressed && scene->lastRight) scene->rightDone = true;
        if (!input.dPadUpPressed && scene->lastDUp) scene->upDone = true;
        if (!input.dPadDownPressed && scene->lastDDown) scene->downDone = true;
        if (!input.dPadLeftPressed && scene->lastDLeft) scene->leftDDone = true;
        if (!input.dPadRightPressed && scene->lastDRight) scene->rightDDone = true;
        if (!input.dPadCenterPressed && scene->lastDCenter) scene->centerDone = true;

        // Update last state
        scene->lastLeft = input.buttonLeftPressed;
        scene->lastRight = input.buttonRightPressed;
        scene->lastDUp = input.dPadUpPressed;
        scene->lastDDown = input.dPadDownPressed;
        scene->lastDLeft = input.dPadLeftPressed;
        scene->lastDRight = input.dPadRightPressed;
        scene->lastDCenter = input.dPadCenterPressed;

        unsigned long now = millis();
        if (now - scene->lastUpdate >= scene->UPDATE_INTERVAL) {
          scene->lastUpdate = now;
          scene->updateDisplay();
        }
      }
    }
  } pollActor{ this };

public:
  HardwareTestScene()
    : Scene(true), pollActor(this) {}

  void onEnter() override {
    // Reset counters and states
    leftCount = rightCount = upCount = downCount = leftDCount = rightDCount = centerCount = 0;
    lastLeft = lastRight = lastDUp = lastDDown = lastDLeft = lastDRight = lastDCenter = false;
    leftDone = rightDone = upDone = downDone = leftDDone = rightDDone = centerDone = false;
    lastUpdate = 0;
    updateDisplay();
  }

  void updateDisplay() {
    Engine* eng = Scene::getEngine();
    if (!eng) return;
    Display::Display& display = eng->display;

    display.clear();
    int y = 5;
    display.drawText(2, y, "Hardware Test", Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 12;

    char buf[32];
    Input& input = eng->input;

    snprintf(buf, sizeof(buf), "Left:   %c  (%d)   %c", input.buttonLeftPressed ? 'P' : ' ', leftCount, leftDone ? 'V' : ' ');
    display.drawText(2, y, buf, Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 10;

    snprintf(buf, sizeof(buf), "Right:  %c  (%d)   %c", input.buttonRightPressed ? 'P' : ' ', rightCount, rightDone ? 'V' : ' ');
    display.drawText(2, y, buf, Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 10;

    snprintf(buf, sizeof(buf), "D-Up:   %c  (%d)   %c", input.dPadUpPressed ? 'P' : ' ', upCount, upDone ? 'V' : ' ');
    display.drawText(2, y, buf, Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 10;

    snprintf(buf, sizeof(buf), "D-Down: %c  (%d)   %c", input.dPadDownPressed ? 'P' : ' ', downCount, downDone ? 'V' : ' ');
    display.drawText(2, y, buf, Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 10;

    snprintf(buf, sizeof(buf), "D-Left: %c  (%d)   %c", input.dPadLeftPressed ? 'P' : ' ', leftDCount, leftDDone ? 'V' : ' ');
    display.drawText(2, y, buf, Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 10;

    snprintf(buf, sizeof(buf), "D-Right:%c  (%d)   %c", input.dPadRightPressed ? 'P' : ' ', rightDCount, rightDDone ? 'V' : ' ');
    display.drawText(2, y, buf, Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 10;

    snprintf(buf, sizeof(buf), "Center: %c  (%d)   %c", input.dPadCenterPressed ? 'P' : ' ', centerCount, centerDone ? 'V' : ' ');
    display.drawText(2, y, buf, Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 12;

    // Battery status
    Battery& battery = eng->battery;
    bool charging = battery.isCharging();
    bool charged = battery.isCharged();
    int pct = battery.percentCharged();

    // Show charging state; while charging, display "Charging" instead of percentage
    snprintf(buf, sizeof(buf), "Charging: %s", charging ? "YES" : "NO ");
    display.drawText(2, y, buf, Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 10;

    snprintf(buf, sizeof(buf), "Charged:  %s", charged ? "YES" : "NO ");
    display.drawText(2, y, buf, Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 10;

    if (charging) {
      snprintf(buf, sizeof(buf), "Status:   Charging");
    } else {
      snprintf(buf, sizeof(buf), "Percent:  %d%%", pct);
    }
    display.drawText(2, y, buf, Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));
    y += 10;

    display.update();
  }
};

// Global instance for launcher
HardwareTestScene hardwareTestScene;

#endif  // HARDWARE_TEST_SCENE_HPP
