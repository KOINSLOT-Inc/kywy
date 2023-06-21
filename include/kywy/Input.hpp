// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "Actor.hpp"
#include "kywy/HAL.hpp"

#include "freertos/timers.h"

namespace Kywy {

// Dispatches a GET_INPUTS event to Input
void getInputsCallback(TimerHandle_t timer);

class Input : public Actor::Actor {
private:
#if HAL_BUTTON_LEFT_PIN >= 0
  bool buttonLeftPressed;
#endif
#if HAL_BUTTON_RIGHT_PIN >= 0
  bool buttonRightPressed;
#endif

#if HAL_D_PAD_LEFT_PIN >= 0
  bool dPadLeftPressed;
#endif
#if HAL_D_PAD_RIGHT_PIN >= 0
  bool dPadRightPressed;
#endif
#if HAL_D_PAD_UP_PIN >= 0
  bool dPadUpPressed;
#endif
#if HAL_D_PAD_DOWN_PIN >= 0
  bool dPadDownPressed;
#endif

  // FreeRTOS ticks between each GET_INPUTS event
  int pollingPeriod = pdMS_TO_TICKS(50);

  // Software timer used to trigger GET_INPUTS events
  TimerHandle_t timer;

public:
  const char *getName() { return "input"; };
  void initialize();
  void handle(::Actor::Message *message);

  // Returns the milliseconds between each GET_INPUTS event
  int getPollingPeriod();

  // Sets the milliseconds between each GET_INPUTS event
  void setPollingPeriod(int milliseconds);

private:
  bool isTTY = false; // used for running vs testing on host
};

} // namespace Kywy
