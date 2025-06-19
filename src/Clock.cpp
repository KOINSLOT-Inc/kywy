// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Clock.hpp"
#include "Events.hpp"

namespace Kywy {

// Static debug flag
bool Clock::debugMode = false;

// Debug macro for Clock
#define CLOCK_DEBUG(...) do { if (Clock::debugMode) { Serial.print("Clock: "); Serial.println(__VA_ARGS__); } } while(0)

void clockTickCallback(Clock *clock) {
  if (clock->options.getClickToClick()) {
    if (Serial.available()) {
      String input = Serial.readString();
      if (!(input == "tick\n")) {
        return;
      }
    } else {
      return;
    }
  }  
  CLOCK_DEBUG("Publishing TICK message");
  clock->publish(&clock->tickMessage);
};

int Clock::getTickDuration() {
  return tickDuration.count();
}

void Clock::setTickDuration(int milliseconds) {
  tickDuration = std::chrono::milliseconds(milliseconds);
  // TODO: this is still broken
  // this->clock.break_dispatch();
  // this->clock.call_every(tickDuration, mbed::callback(&clockTickCallback,
  // this));
}

void Clock::initialize() {
  Serial.println("Clock: Initializing with tickDuration: " + String((int)tickDuration.count()) + "ms");
  this->tickMessage.signal = Kywy::Events::TICK;
  this->clock.call_every(tickDuration,
                         mbed::callback(&clockTickCallback, this));
  clockThread.start(
    mbed::callback(&(this->clock), &events::EventQueue::dispatch_forever));
  Serial.println("Clock: Initialization complete");
}

void Clock::handle(::Actor::Message *message) {
  switch (message->signal) {
    case Events::SET_TICK_DURATION:
      {
        setTickDuration(*(int *)message->data);
        break;
      }
    default:
      {
        break;
      }
  }
};

}  // namespace Kywy
