// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_KYWY
#define KYWY_LIB_KYWY 1

#include "Actor.hpp"
#include "Scene.hpp"
#include "Display.hpp"
#include "Battery.hpp"
#include "Font.hpp"
#include "Fonts.hpp"
#include "Clock.hpp"
#include "Events.hpp"
#include "Input.hpp"
#include "Sprite.hpp"
#include "SpriteSheet.hpp"

namespace Kywy {

struct EngineOptions {
  bool _clickToTick = false;

  EngineOptions clickToTick(bool setClickToTick) {
    _clickToTick = setClickToTick;
    return *this;
  };
  bool getClickToClick() {
    return _clickToTick;
  };
};

class Engine : public ::Actor::Actor {
public:
  void start(EngineOptions options = EngineOptions());
  void initialize();
  void handle(::Actor::Message *message);

  Clock clock;
  Input input;
  Display::Display display;
  Battery battery;
  EngineOptions options;

private:
  Display::Driver::Driver *displayDriver;
};

}  // namespace Kywy

#endif
