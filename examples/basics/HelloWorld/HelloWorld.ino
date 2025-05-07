// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

void setup() {
  engine.start();
  engine.display.clear();

  engine.display.drawText(
    KYWY_DISPLAY_WIDTH / 2, KYWY_DISPLAY_HEIGHT / 2,
    "Hello World!",
    Display::TextOptions().origin(Display::Origin::Text::CENTER));
  engine.display.update();
}

void loop() {
  delay(1000);
}
