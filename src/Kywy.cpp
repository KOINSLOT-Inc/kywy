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

  input.subscribe(&clock);  // get inputs for every tick
  input.start();

  // Subscribe the engine to clock events so we can call hooks
  this->subscribe(&clock);
  // Subscribe the engine to input events so we can call input hooks
  this->subscribe(&input);

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
    case Events::TICK:
      callOnTick(message); // call user-defined onTick hook
      break;
    case Events::BUTTON_LEFT_PRESSED:
    case Events::BUTTON_LEFT_RELEASED:
    case Events::BUTTON_RIGHT_PRESSED:
    case Events::BUTTON_RIGHT_RELEASED:
    case Events::D_PAD_LEFT_PRESSED:
    case Events::D_PAD_LEFT_RELEASED:
    case Events::D_PAD_RIGHT_PRESSED:
    case Events::D_PAD_RIGHT_RELEASED:
    case Events::D_PAD_UP_PRESSED:
    case Events::D_PAD_UP_RELEASED:
    case Events::D_PAD_DOWN_PRESSED:
    case Events::D_PAD_DOWN_RELEASED:
    case Events::D_PAD_CENTER_PRESSED:
    case Events::D_PAD_CENTER_RELEASED:
    case Events::INPUT_PRESSED:
    case Events::INPUT:
      callOnInput(message); // call user-defined onInput hook
      break;
    default:
      break;
  }
  
  // Always forward to subcomponent actors
  clock.dispatch(message);
  input.dispatch(message);
};

void Engine::callOnTick(::Actor::Message *message) {
  if (onTick) {
    onTick(message);
  }
}

void Engine::callOnInput(::Actor::Message *message) {
  if (onInput) {
    onInput(message);
  }
}

}  // namespace Kywy
