// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "sdkconfig.h"

#include "Kywy.hpp"

namespace Kywy {

void Engine::start() {
#ifndef CONFIG_IDF_TARGET_LINUX
  displayDriver = new Display::Driver::SSD1327_128X128_SPI_DRIVER( //
      {.spi = {
           .cs = HAL_DISPLAY_PIN_SPI_CS,
           .rst = HAL_DISPLAY_PIN_SPI_RST,
           .dc = HAL_DISPLAY_PIN_SPI_DC,
           .sclk = HAL_DISPLAY_PIN_SPI_SCLK,
           .mosi = HAL_DISPLAY_PIN_SPI_MOSI,
       }});
#else
  displayDriver = new Display::Driver::SERIAL_128X128_DRIVER();
#endif

  display = Display::Display(displayDriver);

  Actor::Actor::start();

  clock.start();
  input.start();

  display.setup();
#ifndef CONFIG_IDF_TARGET_LINUX
  display.setRotation(Display::Rotation::CLOCKWISE_180);
#endif
}

void Engine::initialize() {}

void Engine::handle(::Actor::Message *message) {
  switch (message->signal) {
  default: { // forward to subcomponent actors
    clock.dispatch(message);
    input.dispatch(message);
    break;
  }
  }
};

} // namespace Kywy
