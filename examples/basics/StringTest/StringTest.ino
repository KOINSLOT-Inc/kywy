// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

void setup() {
  engine.start();
  engine.display.clear();

  // Test with C-style string (original method)
  engine.display.drawText(
    KYWY_DISPLAY_WIDTH / 2, 20,
    "C-style string",
    Display::TextOptions().origin(Display::Origin::Text::CENTER));

  // Test with Arduino String object (new method)
  String dynamicText = "Arduino String";
  engine.display.drawText(
    KYWY_DISPLAY_WIDTH / 2, 40,
    dynamicText,  // No need for .c_str() anymore!
    Display::TextOptions().origin(Display::Origin::Text::CENTER));

  // Test with String concatenation
  String name = "Kywy";
  String version = "2.0";
  String fullText = name + " v" + version;
  engine.display.drawText(
    KYWY_DISPLAY_WIDTH / 2, 60,
    fullText,
    Display::TextOptions().origin(Display::Origin::Text::CENTER));

  engine.display.update();
}

void loop() {
  delay(1000);
}
