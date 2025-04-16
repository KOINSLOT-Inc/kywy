// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

namespace Kywy {

void Engine::start() {
  displayDriver = new Display::Driver::MBED_SPI_DRIVER();
  display = Display::Display(displayDriver);

  Actor::Actor::start();

  clock.start();

  input.subscribe(&clock);  // get inputs for every tick
  input.start();

  display.setup();
  battery.setup();
}

void Engine::initialize() {}


void Engine::handle(::Actor::Message *message) {
  if (message->directive == ::Actor::DIRECTIVE_EXIT) {
    delete this->displayDriver;
    return;
  }

  switch (message->signal) {
    default:
      {  // forward to subcomponent actors
        clock.dispatch(message);
        input.dispatch(message);
        break;
      }
  }
};

}  // namespace Kywy
