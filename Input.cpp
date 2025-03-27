// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Input.hpp"
#include "Events.hpp"

namespace Kywy {

void Input::initialize() {
  pinMode(KYWY_LEFT_BUTTON, INPUT_PULLUP);
  pinMode(KYWY_RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(KYWY_D_PAD_UP, INPUT_PULLUP);
  pinMode(KYWY_D_PAD_DOWN, INPUT_PULLUP);
  pinMode(KYWY_D_PAD_LEFT, INPUT_PULLUP);
  pinMode(KYWY_D_PAD_RIGHT, INPUT_PULLUP);
  pinMode(KYWY_D_PAD_CENTER, INPUT_PULLUP);

  inputMessage.signal = Events::KywyEvents::INPUT;
  dPadMessage.signal = Events::KywyEvents::D_PAD;

  buttonLeftPressedMessage.signal = Events::KywyEvents::BUTTON_LEFT_PRESSED;
  buttonLeftReleasedMessage.signal = Events::KywyEvents::BUTTON_LEFT_RELEASED;
  buttonRightPressedMessage.signal = Events::KywyEvents::BUTTON_RIGHT_PRESSED;
  buttonRightReleasedMessage.signal = Events::KywyEvents::BUTTON_RIGHT_RELEASED;
  dPadLeftPressedMessage.signal = Events::KywyEvents::D_PAD_LEFT_PRESSED;
  dPadLeftReleasedMessage.signal = Events::KywyEvents::D_PAD_LEFT_RELEASED;
  dPadRightPressedMessage.signal = Events::KywyEvents::D_PAD_RIGHT_PRESSED;
  dPadRightReleasedMessage.signal = Events::KywyEvents::D_PAD_RIGHT_RELEASED;
  dPadUpPressedMessage.signal = Events::KywyEvents::D_PAD_UP_PRESSED;
  dPadUpReleasedMessage.signal = Events::KywyEvents::D_PAD_UP_RELEASED;
  dPadDownPressedMessage.signal = Events::KywyEvents::D_PAD_DOWN_PRESSED;
  dPadDownReleasedMessage.signal = Events::KywyEvents::D_PAD_DOWN_RELEASED;
  dPadCenterPressedMessage.signal = Events::KywyEvents::D_PAD_CENTER_PRESSED;
  dPadCenterReleasedMessage.signal = Events::KywyEvents::D_PAD_CENTER_RELEASED;
}

void Input::handle(::Actor::Message *message) {
  switch (message->signal) {
  case Events::TICK: {
    bool inputEvent = false, dPadEvent = false;

    if ((digitalRead(KYWY_LEFT_BUTTON) == LOW) != buttonLeftPressed) {
      buttonLeftPressed = !buttonLeftPressed;
      publish(buttonLeftPressed ? &buttonLeftPressedMessage : &buttonLeftReleasedMessage);
      inputEvent = true;
    }

    if ((digitalRead(KYWY_RIGHT_BUTTON) == LOW) != buttonRightPressed) {
      buttonRightPressed = !buttonRightPressed;
      publish(buttonRightPressed ? &buttonRightPressedMessage : &buttonRightReleasedMessage);
      inputEvent = true;
    }

    if ((digitalRead(KYWY_D_PAD_LEFT) == LOW) != dPadLeftPressed) {
      dPadLeftPressed = !dPadLeftPressed;
      publish(dPadLeftPressed ? &dPadLeftPressedMessage : &dPadLeftReleasedMessage);
      inputEvent = true;
      dPadEvent = true;
    }

    if ((digitalRead(KYWY_D_PAD_RIGHT) == LOW) != dPadRightPressed) {
      dPadRightPressed = !dPadRightPressed;
      publish(dPadRightPressed ? &dPadRightPressedMessage : &dPadRightReleasedMessage);
      inputEvent = true;
      dPadEvent = true;
    }

    if ((digitalRead(KYWY_D_PAD_UP) == LOW) != dPadUpPressed) {
      dPadUpPressed = !dPadUpPressed;
      publish(dPadUpPressed ? &dPadUpPressedMessage : &dPadUpReleasedMessage);
      inputEvent = true;
      dPadEvent = true;
    }

    if ((digitalRead(KYWY_D_PAD_DOWN) == LOW) != dPadDownPressed) {
      dPadDownPressed = !dPadDownPressed;
      publish(dPadDownPressed ? &dPadDownPressedMessage : &dPadDownReleasedMessage);
      inputEvent = true;
      dPadEvent = true;
    }

    if ((digitalRead(KYWY_D_PAD_CENTER) == LOW) != dPadCenterPressed) {
      dPadCenterPressed = !dPadCenterPressed;
      publish(dPadCenterPressed ? &dPadCenterPressedMessage : &dPadCenterReleasedMessage);
      inputEvent = true;
      dPadEvent = true;
    }

    if (inputEvent) {
      publish(&inputMessage);
    }

    if (dPadEvent) {
      publish(&dPadMessage);
    }

    break;
  }
  default: {
    break;
  }
  }
};

} // namespace Kywy
