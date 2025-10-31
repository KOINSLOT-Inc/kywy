// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

void setup() {
  engine.clock.setTickDuration(30);  // Set time between ticks
  engine.start();
}

void loop() {
}

void onTick(Actor::Message *message) {
  // Do stuff every clock tick i.e. draw to display
}

void onInput(Actor::Message *message) {
  // Do stuff when buttons are pressed/released
  switch (message->signal) {
    case Kywy::Events::BUTTON_RIGHT_PRESSED:
      break;
    
    case Kywy::Events::BUTTON_LEFT_PRESSED:
      break;
    
    case Kywy::Events::D_PAD_LEFT_PRESSED:
      break;

    case Kywy::Events::D_PAD_RIGHT_PRESSED:
      break;

    case Kywy::Events::D_PAD_UP_PRESSED:
      break;

    case Kywy::Events::D_PAD_DOWN_PRESSED:
      break;

    case Kywy::Events::BUTTON_RIGHT_RELEASED:
      break;

    case Kywy::Events::BUTTON_LEFT_RELEASED:
      break;

    case Kywy::Events::D_PAD_LEFT_RELEASED:
      break;

    case Kywy::Events::D_PAD_RIGHT_RELEASED:
      break;

    case Kywy::Events::D_PAD_UP_RELEASED:
      break;

    case Kywy::Events::D_PAD_DOWN_RELEASED:
      break;
  }
}
