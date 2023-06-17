// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

class GameManager : public Actor::Actor {
public:
  const char *getName() { return "gameManager"; };

  void initialize() {}

  int counter = 0;

  void handle(::Actor::Message *message) {
    switch (message->signal) {
    case Kywy::Events::TICK: {
      counter++;
      engine.display.clear();

      char count[10];
      sprintf(count, "%d", counter);
      engine.display.drawText(Display::Origin::Text::BASELINE_CENTER, 32, 40, Display::Font::bailleul_16_pt, count,
                              0xff, {.transparent = true});

      engine.display.update();
      break;
    }
    }
  }
} gameManager;

extern "C" void app_main(void) {
  engine.start();
  engine.clock.setTickDuration(500);

  gameManager.subscribe(&engine.clock);
  gameManager.start();
}
