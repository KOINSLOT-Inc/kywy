// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Actor.hpp"
#include "Display.hpp"

#include "kywy/Clock.hpp"
#include "kywy/Events.hpp"
#include "kywy/HAL.hpp"
#include "kywy/Input.hpp"
#include "kywy/Sprite.hpp"
#include "kywy/SpriteSheet.hpp"

namespace Kywy {

class Engine : public Actor::Actor {
public:
  void start();

  const char *getName() { return "engine"; };
  void initialize();
  void handle(::Actor::Message *message);

  Clock clock;
  Input input;
  Display::Display display;

private:
  Display::Driver::Driver *displayDriver;
};

} // namespace Kywy
