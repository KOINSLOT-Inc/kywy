// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

typedef enum : uint16_t {
  RUN = ::StateMachine::EVENT_USER,
  CROUCH,
} Events;

class CharacterState : public StateMachine::StateMachine {
public:
  void setup() {
    initialize((::StateMachine::State)&CharacterState::Root);
  };

  ::StateMachine::Result Root(::StateMachine::Event event) {
    switch (event.signal) {
      case ::StateMachine::EVENT_REQUEST_PARENT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_PARENT,
          .target = nullptr,  // the root state has no parent
        };
        break;
      case ::StateMachine::EVENT_REQUEST_CHILD:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_CHILD,
          .target = (::StateMachine::State)&CharacterState::Still,  // by default we fall into the still state
        };
        break;
      case ::StateMachine::EVENT_ENTER:
        Serial.println("Entering the root state: here we initialize the character state");
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_EXIT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      case RUN:  // if we get a run event, transition to the running state
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_TRANSITION,
          .target = (::StateMachine::State)&CharacterState::Running,
        };
      case CROUCH:  // if we get a crouch event, transition to the crouching state
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_TRANSITION,
          .target = (::StateMachine::State)&CharacterState::Crouching,
        };
      default:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
    }
  };

  ::StateMachine::Result Still(::StateMachine::Event event) {
    switch (event.signal) {
      case ::StateMachine::EVENT_REQUEST_PARENT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_PARENT,
          .target = (::StateMachine::State)&CharacterState::Root,
        };
        break;
      case ::StateMachine::EVENT_REQUEST_CHILD:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_CHILD,
          .target = (::StateMachine::State)&CharacterState::Standing,  // by default we fall into the standing state
        };
        break;
      case ::StateMachine::EVENT_ENTER:
        Serial.println("Entering the still state: zoom camera out while standing still");
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_EXIT:
        Serial.println("Exiting the still state: zoom camera in on character");
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      default:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_UNHANDLED,
          .target = nullptr,
        };
        break;
    }
  };

  ::StateMachine::Result Standing(::StateMachine::Event event) {
    switch (event.signal) {
      case ::StateMachine::EVENT_REQUEST_PARENT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_PARENT,
          .target = (::StateMachine::State)&CharacterState::Still,
        };
        break;
      case ::StateMachine::EVENT_REQUEST_CHILD:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_CHILD,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_ENTER:
        Serial.println("Entering the standing state: set sprite to standing");
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_EXIT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      default:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_UNHANDLED,
          .target = nullptr,
        };
        break;
    }
  };

  ::StateMachine::Result Crouching(::StateMachine::Event event) {
    switch (event.signal) {
      case ::StateMachine::EVENT_REQUEST_PARENT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_PARENT,
          .target = (::StateMachine::State)&CharacterState::Still,
        };
        break;
      case ::StateMachine::EVENT_REQUEST_CHILD:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_CHILD,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_ENTER:
        Serial.println("Entering the crouching state: set sprite to crouching");
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_EXIT:
        Serial.println("Exiting the crouching state: play getting up animation");
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      default:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_UNHANDLED,
          .target = nullptr,
        };
        break;
    }
  };

  ::StateMachine::Result Moving(::StateMachine::Event event) {
    switch (event.signal) {
      case ::StateMachine::EVENT_REQUEST_PARENT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_PARENT,
          .target = (::StateMachine::State)&CharacterState::Root,
        };
        break;
      case ::StateMachine::EVENT_REQUEST_CHILD:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_CHILD,
          .target = (::StateMachine::State)&CharacterState::Walking,
        };
        break;
      case ::StateMachine::EVENT_ENTER:
        Serial.println("Entering the moving state: set slight motion blur and shake on screen");
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_EXIT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      default:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_UNHANDLED,
          .target = nullptr,
        };
        break;
    }
  };

  ::StateMachine::Result Walking(::StateMachine::Event event) {
    switch (event.signal) {
      case ::StateMachine::EVENT_REQUEST_PARENT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_PARENT,
          .target = (::StateMachine::State)&CharacterState::Moving,
        };
        break;
      case ::StateMachine::EVENT_REQUEST_CHILD:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_CHILD,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_ENTER:
        Serial.println("Entering the walking state: set sprite to walking");
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_EXIT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      default:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_UNHANDLED,
          .target = nullptr,
        };
        break;
    }
  };

  ::StateMachine::Result Running(::StateMachine::Event event) {
    switch (event.signal) {
      case ::StateMachine::EVENT_REQUEST_PARENT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_PARENT,
          .target = (::StateMachine::State)&CharacterState::Moving,
        };
        break;
      case ::StateMachine::EVENT_REQUEST_CHILD:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_CHILD,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_ENTER:
        Serial.println("Entering the running state: zoom in on character more, set sprite to running");
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      case ::StateMachine::EVENT_EXIT:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_HANDLED,
          .target = nullptr,
        };
        break;
      default:
        return ::StateMachine::Result{
          .type = ::StateMachine::RESULT_UNHANDLED,
          .target = nullptr,
        };
        break;
    }
  };
} CharacterState;

void setup() {
  delay(1000);  // delay so serial monitor can start up
  CharacterState.setup();
  CharacterState.handle(::StateMachine::Event{ .signal = CROUCH });
  CharacterState.handle(::StateMachine::Event{ .signal = RUN });
  // Serial output should be
  //   Entering the root state: here we initialize the character state
  //   Entering the still state: zoom camera out while standing still
  //   Entering the standing state: set sprite to standing
  //   Entering the crouching state: set sprite to crouching
  //   Exiting the crouching state: play getting up animation
  //   Exiting the still state: zoom camera in on character
  //   Entering the moving state: set slight motion blur and shake on screen
  //   Entering the running state: zoom in on character more, set sprite to running
}

void loop() {
  delay(1000);
}
