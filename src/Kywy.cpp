// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"
#include "SPIBus.hpp"
#include "Display.hpp"

namespace Kywy {

void Engine::start(EngineOptions options) {
  this->options = options;
  Serial.begin(9600);

  // Initialize SPI bus with display pins
  SPIBus::initialize();

  displayDriver = new Display::Driver::KYWY_DISPLAY_DRIVER();
  display = Display::Display(displayDriver);

  Actor::Actor::start();

  clock.options.clickToTick(options.getClickToClick());
  clock.start();

  this->subscribe(&clock); // Subscribe engine to clock to receive TICK events

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
    case Kywy::Events::TICK:
      {
        display.checkPendingUpdate();
        break;
      }
    default:
      {  // forward to subcomponent actors
        clock.dispatch(message);
        input.dispatch(message);
        break;
      }
  }
  
};

}  // namespace Kywy
