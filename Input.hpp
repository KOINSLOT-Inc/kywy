// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_INPUT
#define KYWY_LIB_INPUT 1

#include "Actor.hpp"

namespace Kywy {

#define KYWY_LEFT_BUTTON      12
#define KYWY_RIGHT_BUTTON      2
#define KYWY_D_PAD_UP          9
#define KYWY_D_PAD_DOWN        3
#define KYWY_D_PAD_LEFT        6
#define KYWY_D_PAD_RIGHT       7
#define KYWY_D_PAD_CENTER      8

class Input : public Actor::Actor {
private:
  bool buttonLeftPressed;
  bool buttonRightPressed;
  bool dPadLeftPressed;
  bool dPadRightPressed;
  bool dPadUpPressed;
  bool dPadDownPressed;
  bool dPadCenterPressed;

public:
  void initialize();
  void handle(::Actor::Message *message);

  ::Actor::Message inputMessage;
  ::Actor::Message dPadMessage;

  ::Actor::Message buttonLeftPressedMessage;
  ::Actor::Message buttonLeftReleasedMessage;
  ::Actor::Message buttonRightPressedMessage;
  ::Actor::Message buttonRightReleasedMessage;
  ::Actor::Message dPadLeftPressedMessage;
  ::Actor::Message dPadLeftReleasedMessage;
  ::Actor::Message dPadRightPressedMessage;
  ::Actor::Message dPadRightReleasedMessage;
  ::Actor::Message dPadUpPressedMessage;
  ::Actor::Message dPadUpReleasedMessage;
  ::Actor::Message dPadDownPressedMessage;
  ::Actor::Message dPadDownReleasedMessage;
  ::Actor::Message dPadCenterPressedMessage;
  ::Actor::Message dPadCenterReleasedMessage;
};

} // namespace Kywy

#endif
