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
  inputPressedMessage.signal = Events::KywyEvents::INPUT_PRESSED;
  dPadMessage.signal = Events::KywyEvents::D_PAD;
  dPadPressedMessage.signal = Events::KywyEvents::D_PAD_PRESSED;

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
    case Events::TICK:
      {
        bool inputEvent = false, dPadEvent = false;
        bool inputPressedEvent = false, dPadPressedEvent = false;

        if ((digitalRead(KYWY_LEFT_BUTTON) == LOW) != _buttonLeftPressed) {
          _buttonLeftPressed = !_buttonLeftPressed;
          buttonLeftPressed = _buttonLeftPressed;
          publish(_buttonLeftPressed ? &buttonLeftPressedMessage : &buttonLeftReleasedMessage);
          inputEvent = true;
          if (_buttonLeftPressed) {
            inputPressedEvent = true;
          }
        }

        if ((digitalRead(KYWY_RIGHT_BUTTON) == LOW) != _buttonRightPressed) {
          _buttonRightPressed = !_buttonRightPressed;
          buttonRightPressed = _buttonRightPressed;
          publish(_buttonRightPressed ? &buttonRightPressedMessage : &buttonRightReleasedMessage);
          inputEvent = true;
          if (_buttonRightPressed) {
            inputPressedEvent = true;
          }
        }

        if ((digitalRead(KYWY_D_PAD_LEFT) == LOW) != _dPadLeftPressed) {
          _dPadLeftPressed = !_dPadLeftPressed;
          dPadLeftPressed = _dPadLeftPressed;
          publish(_dPadLeftPressed ? &dPadLeftPressedMessage : &dPadLeftReleasedMessage);
          inputEvent = true;
          dPadEvent = true;
          if (_dPadLeftPressed) {
            inputPressedEvent = true;
            dPadPressedEvent = true;
          }
        }

        if ((digitalRead(KYWY_D_PAD_RIGHT) == LOW) != _dPadRightPressed) {
          _dPadRightPressed = !_dPadRightPressed;
          dPadRightPressed = _dPadRightPressed;
          publish(_dPadRightPressed ? &dPadRightPressedMessage : &dPadRightReleasedMessage);
          inputEvent = true;
          dPadEvent = true;
          if (_dPadRightPressed) {
            inputPressedEvent = true;
            dPadPressedEvent = true;
          }
        }

        if ((digitalRead(KYWY_D_PAD_UP) == LOW) != _dPadUpPressed) {
          _dPadUpPressed = !_dPadUpPressed;
          dPadUpPressed = _dPadUpPressed;
          publish(_dPadUpPressed ? &dPadUpPressedMessage : &dPadUpReleasedMessage);
          inputEvent = true;
          dPadEvent = true;
          if (_dPadUpPressed) {
            inputPressedEvent = true;
            dPadPressedEvent = true;
          }
        }

        if ((digitalRead(KYWY_D_PAD_DOWN) == LOW) != _dPadDownPressed) {
          _dPadDownPressed = !_dPadDownPressed;
          dPadDownPressed = _dPadDownPressed;
          publish(_dPadDownPressed ? &dPadDownPressedMessage : &dPadDownReleasedMessage);
          inputEvent = true;
          dPadEvent = true;
          if (_dPadDownPressed) {
            inputPressedEvent = true;
            dPadPressedEvent = true;
          }
        }

        if ((digitalRead(KYWY_D_PAD_CENTER) == LOW) != _dPadCenterPressed) {
          _dPadCenterPressed = !_dPadCenterPressed;
          dPadCenterPressed = _dPadCenterPressed;
          publish(_dPadCenterPressed ? &dPadCenterPressedMessage : &dPadCenterReleasedMessage);
          inputEvent = true;
          dPadEvent = true;
          if (_dPadCenterPressed) {
            inputPressedEvent = true;
            dPadPressedEvent = true;
          }
        }

        if (inputEvent) {
          publish(&inputMessage);
        }

        if (inputPressedEvent) {
          publish(&inputPressedMessage);
        }

        if (dPadEvent) {
          publish(&dPadMessage);
        }

        if (dPadPressedEvent) {
          publish(&dPadPressedMessage);
        }

        break;
      }
    default:
      {
        break;
      }
  }
};

}  // namespace Kywy
