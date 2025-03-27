// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

#define DISPLAY_WIDTH 144
#define DISPLAY_HEIGHT 168

class hardwareTester : public Actor::Actor {
public:
  const char *getName() {
    return "hardwareTester";
  };
  void initialize() {}

  bool testComplete;

  bool buttonLeftPressed, buttonLeftReleased;
  bool buttonRightPressed, buttonRightReleased;
  bool dPadLeftPressed, dPadLeftReleased;
  bool dPadRightPressed, dPadRightReleased;
  bool dPadUpPressed, dPadUpReleased;
  bool dPadDownPressed, dPadDownReleased;
  bool dPadCenterPressed, dPadCenterReleased;

  bool batteryNotChargingDetected, batteryChargingDetected, batteryVoltageRead;

  uint8_t padding = 8;
  uint8_t buttonDiameter = 16;
  uint8_t buttonRadius = buttonDiameter / 2;

  void drawButtonStatus(uint16_t x, uint16_t y, bool pressDetected, bool releaseDetected) {
    engine.display.fillCircle(x, y, buttonDiameter, Display::Object2DOptions().color(0xff).origin(Display::Origin::Object2D::CENTER));  // erase circle

    if (!pressDetected && !releaseDetected) {  // make just a black circle
      engine.display.fillCircle(x, y, buttonDiameter, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
    } else if (pressDetected && releaseDetected) {  // white circle with black border
      engine.display.drawCircle(x, y, buttonDiameter, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
    } else {                                                                                                                                // half filled circle
      engine.display.fillCircle(x, y, buttonDiameter, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));                                     // draw full filled circle
      engine.display.fillRectangle(x - buttonDiameter / 2, y - buttonDiameter / 2, buttonDiameter, buttonDiameter / 2, Display::Object2DOptions().color(0xff));  // erase top half
      engine.display.drawCircle(x, y, buttonDiameter, Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));                                     // draw outline
    }
  }

  void drawCheckStatus(uint16_t x, uint16_t y, bool succeeded) {
    if (succeeded) {
      engine.display.drawRectangle(x, y, 8, 8);
    } else {
      engine.display.fillRectangle(x, y, 8, 8);
    }
  }

  void completeTest() {
    engine.display.clear();

    char msg[] = "test complete!";
    engine.display.drawText(144 / 2, 168 / 2, msg, Display::TextOptions().origin(Display::Origin::Text::CENTER));

    engine.display.update();
    testComplete = true;
  }

  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        buttonLeftPressed = true;
        break;
      case Kywy::Events::BUTTON_LEFT_RELEASED:
        buttonLeftReleased = true;
        break;
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        buttonRightPressed = true;
        break;
      case Kywy::Events::BUTTON_RIGHT_RELEASED:
        buttonRightReleased = true;
        break;
      case Kywy::Events::D_PAD_LEFT_PRESSED:
        dPadLeftPressed = true;
        break;
      case Kywy::Events::D_PAD_LEFT_RELEASED:
        dPadLeftReleased = true;
        break;
      case Kywy::Events::D_PAD_RIGHT_PRESSED:
        dPadRightPressed = true;
        break;
      case Kywy::Events::D_PAD_RIGHT_RELEASED:
        dPadRightReleased = true;
        break;
      case Kywy::Events::D_PAD_UP_PRESSED:
        dPadUpPressed = true;
        break;
      case Kywy::Events::D_PAD_UP_RELEASED:
        dPadUpReleased = true;
        break;
      case Kywy::Events::D_PAD_DOWN_PRESSED:
        dPadDownPressed = true;
        break;
      case Kywy::Events::D_PAD_DOWN_RELEASED:
        dPadDownReleased = true;
        break;
      case Kywy::Events::D_PAD_CENTER_PRESSED:
        dPadCenterPressed = true;
        break;
      case Kywy::Events::D_PAD_CENTER_RELEASED:
        dPadCenterReleased = true;
        break;
      case Kywy::Events::TICK:
        if (testComplete) {
          break;
        }

        if (
          buttonLeftPressed && buttonLeftReleased
          && buttonRightPressed && buttonRightReleased
          && dPadLeftPressed && dPadLeftReleased
          && dPadRightPressed && dPadRightReleased
          && dPadUpPressed && dPadUpReleased
          && dPadDownPressed && dPadDownReleased
          && dPadCenterPressed && dPadCenterReleased
          && batteryNotChargingDetected && batteryChargingDetected && batteryVoltageRead) {
          completeTest();
          break;
        }

        drawButtonStatus(padding + buttonRadius, DISPLAY_HEIGHT - padding - buttonDiameter - padding - buttonRadius, buttonLeftPressed, buttonLeftReleased);
        drawButtonStatus(DISPLAY_WIDTH - padding - buttonRadius, DISPLAY_HEIGHT - padding - buttonDiameter - padding - buttonRadius, buttonRightPressed, buttonRightReleased);

        drawButtonStatus(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - padding - buttonRadius, dPadDownPressed, dPadDownReleased);
        drawButtonStatus(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - padding - buttonDiameter - padding - buttonRadius, dPadCenterPressed, dPadCenterReleased);
        drawButtonStatus(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - padding - buttonDiameter - padding - buttonDiameter - padding - buttonRadius, dPadUpPressed, dPadUpReleased);

        drawButtonStatus(DISPLAY_WIDTH / 2 - buttonRadius - padding - buttonRadius, DISPLAY_HEIGHT - padding - buttonDiameter - padding - buttonRadius, dPadLeftPressed, dPadLeftReleased);
        drawButtonStatus(DISPLAY_WIDTH / 2 + buttonRadius + padding + buttonRadius, DISPLAY_HEIGHT - padding - buttonDiameter - padding - buttonRadius, dPadRightPressed, dPadRightReleased);

        if (!batteryNotChargingDetected && !engine.battery.isCharging()) {
          batteryNotChargingDetected = true;
        }

        if (!batteryChargingDetected && engine.battery.isCharging()) {
          batteryChargingDetected = true;
        }

        if (!batteryVoltageRead && (engine.battery.percentCharged() >= 0) && (engine.battery.percentCharged() <= 100)) {
          batteryVoltageRead = true;
        }

        // write battery and sd card tests
        engine.display.fillRectangle(0, 0, 144, 84, Display::Object2DOptions().color(0xff));

        char msg[32];
        uint16_t textWidth, textHeight;

        memset(msg, 0, sizeof(msg));
        snprintf(msg, sizeof(msg), "battery:");
        engine.display.drawText(10, 10, msg);

        memset(msg, 0, sizeof(msg));
        snprintf(msg, sizeof(msg), "charging:");
        engine.display.drawText(20, 20, msg);
        engine.display.getTextSize(msg, textWidth, textHeight);
        drawCheckStatus(20 + textWidth + 3, 20, batteryChargingDetected);

        memset(msg, 0, sizeof(msg));
        snprintf(msg, sizeof(msg), "not charging:");
        engine.display.drawText(20, 30, msg);
        engine.display.getTextSize(msg, textWidth, textHeight);
        drawCheckStatus(20 + textWidth + 3, 30, batteryNotChargingDetected);

        memset(msg, 0, sizeof(msg));
        snprintf(msg, sizeof(msg), "voltage:");
        engine.display.drawText(20, 40, msg);
        engine.display.getTextSize(msg, textWidth, textHeight);
        drawCheckStatus(20 + textWidth + 3, 40, batteryVoltageRead);

        memset(msg, 0, sizeof(msg));
        snprintf(msg, sizeof(msg), "sd card:");
        engine.display.drawText(10, 50, msg);

        memset(msg, 0, sizeof(msg));
        snprintf(msg, sizeof(msg), "write:");
        engine.display.drawText(20, 60, msg);

        memset(msg, 0, sizeof(msg));
        snprintf(msg, sizeof(msg), "read:");
        engine.display.drawText(20, 70, msg);

        engine.display.update();
        break;
    }
  }

} hardwareTester;

void setup() {
  engine.start();
  hardwareTester.subscribe(&engine.input);
  hardwareTester.subscribe(&engine.clock);
  hardwareTester.start();
}

void loop() {
  delay(1000);
}
