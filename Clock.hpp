// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_CLOCK
#define KYWY_LIB_CLOCK 1

#include "Actor.hpp"

namespace Kywy {

class Clock : public Actor::Actor {
private:
  // The number of ms between a Clock tick
  int tickDuration = 33;

  // Software queue used to trigger ticks periodically
  events::EventQueue clock;
  // Thread used to run clock, separate from actor thread
  rtos::Thread clockThread;

public:
  const char *getName() { return "clock"; };
  void initialize();
  void handle(::Actor::Message *message);

  // Returns the milliseconds between each Clock tick
  int getTickDuration();

  // Sets the milliseconds between each Clock tick
  void setTickDuration(int milliseconds);

  ::Actor::Message tickMessage;
};

// Tick callback that publishes the tick event to subscribers
void clockTickCallback(Clock *clock);

} // namespace Kywy

#endif