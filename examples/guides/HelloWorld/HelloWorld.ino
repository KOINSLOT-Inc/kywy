// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

void setup() {
  engine.start();
  engine.display.clear();

  char msg[] = "Hello World!";
  engine.display.drawText(144 / 2, 168 / 2, msg, Display::TextOptions().origin(Display::Origin::Text::CENTER));
  engine.display.update();
}

void loop() {
  delay(1000);
}
