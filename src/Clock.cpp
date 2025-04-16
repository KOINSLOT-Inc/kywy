// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Clock.hpp"
#include "Events.hpp"

namespace Kywy {

void clockTickCallback(Clock *clock) {
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
  this->tickMessage.signal = Kywy::Events::TICK;
  this->clock.call_every(tickDuration,
                         mbed::callback(&clockTickCallback, this));
  clockThread.start(
    mbed::callback(&(this->clock), &events::EventQueue::dispatch_forever));
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
