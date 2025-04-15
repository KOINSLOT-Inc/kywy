// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_STATE_MACHINE
#define KYWY_LIB_STATE_MACHINE 1

#include <stdint.h>

namespace StateMachine {
const uint8_t MAX_STATE_DEPTH = 16;  // used to prevent infinite recursive loops

typedef struct Event Event;
typedef struct Result Result;

class StateMachine;
typedef Result (::StateMachine::StateMachine::*State)(Event event);

typedef enum : uint16_t {
  EVENT_REQUEST_PARENT,  // used to fetch the parent of a nested substate
  EVENT_REQUEST_CHILD,   // used to decide which child to transition to if a
                         // transition target is a composite state
  EVENT_ENTER,
  EVENT_EXIT,

  EVENT_USER,  // library users can begin event enumeration with this event
} Signal;

typedef enum : uint8_t {
  RESULT_PARENT,
  RESULT_CHILD,
  RESULT_HANDLED,
  RESULT_UNHANDLED,
  RESULT_TRANSITION,
  RESULT_ERROR,
} ResultType;

struct Event {
  uint16_t signal;
};

struct Result {
  ResultType type;
  State target;
};

class StateMachine {
public:
  void initialize(State state);
  virtual void setup() = 0;

  Result handle(Event event);

private:
  State state;
  uint8_t depth = 0;

  // enters all parents of the target state, used only once for initialization
  void enterParentStates(State targetState);

  // derive the ancestory of `state` and store it in `ancestry`, returns the
  // depth of the ancestory, assumes `ancestry` is initialized as an array of
  // null pointers
  uint8_t getAncestry(State targetState, State ancestry[]);

  // transitions from the current state to a new target State
  // exits and enters all parent states along the way
  void transition(State targetState);

  // recursive component of `handle`, pushes an event through a state and
  // handles the result
  Result processEvent(State targetState, Event event);
};

}  // namespace StateMachine

#endif