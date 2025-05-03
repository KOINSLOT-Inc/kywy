// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Actor.hpp>
#include <Battery.hpp>
#include <Clock.hpp>
#include <Display.hpp>
#include <Events.hpp>
#include <Font.hpp>
#include <Fonts.hpp>
#include <GraphicsObject.hpp>
#include <Input.hpp>
#include <Kywy.hpp>
#include <Sprite.hpp>
#include <SpriteSheet.hpp>
#include <StateMachine.hpp>

#include "Kywy.hpp"
#include <pico/bootrom.h>

Kywy::Engine engine;

void setup() {
  engine.start();
}

void loop() {
}
