// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "StateMachine.hpp"

namespace StateMachine {

void StateMachine::initialize(State targetState) {
  depth = 0;

  enterParentStates(targetState);

  state = targetState;

  Result result = { .type = RESULT_CHILD, .target = targetState };
  while (result.target) {
    result = (this->*(result.target))(Event{ .signal = EVENT_REQUEST_CHILD });
    if (result.target) {
      (this->*(result.target))(Event{ .signal = EVENT_ENTER });
      state = result.target;
    }
  }
};

void StateMachine::enterParentStates(State targetState) {
  Result result = (this->*(targetState))(Event{ .signal = EVENT_REQUEST_PARENT });

  State parent = result.target;
  if (parent) {
    depth++;
    if (depth == MAX_STATE_DEPTH) {
      return;  // TODO: error
    }

    enterParentStates(parent);
  }

  (this->*(targetState))(Event{ .signal = EVENT_ENTER });
};

uint8_t StateMachine::getAncestry(State targetState, State ancestry[]) {
  // call EVENT_REQUEST_PARENT until result.target is null
  // store state along the way
  // reverse list
  depth = 0;
  ancestry[depth] = targetState;

  State parent =
    (this->*(targetState))(Event{ .signal = EVENT_REQUEST_PARENT }).target;

  while (parent) {
    depth++;
    if (depth == MAX_STATE_DEPTH) {
      return 0;  // TODO: error
    }

    ancestry[depth] = parent;

    parent = (this->*(parent))(Event{ .signal = EVENT_REQUEST_PARENT }).target;
  }

  // reverse populated portion of list
  // [D, C, B, A, null, null] -> [A, B, C, D, null, null]
  for (uint8_t i = 0; i < (depth + 1) / 2; i++) {
    State temporaryStore = ancestry[i];
    ancestry[i] = ancestry[depth - i];
    ancestry[depth - i] = temporaryStore;
  }

  return depth;
};

void StateMachine::transition(State targetState) {
  // get current and target ancestry
  State currentAncestry[MAX_STATE_DEPTH] = {};
  uint8_t currentAncestryDepth = getAncestry(state, currentAncestry);

  State targetAncestry[MAX_STATE_DEPTH] = {};
  uint8_t targetAncestryDepth = getAncestry(targetState, targetAncestry);

  if (currentAncestry[0] != targetAncestry[0]) {
    return;  // TODO: error, need common ancestor
  }

  // find common ancestor
  uint8_t i = 0;
  while (currentAncestry[i] && targetAncestry[i] && currentAncestry[i] == targetAncestry[i]) {
    i++;
  }
  uint8_t commonAncestorIndex = i - 1;

  // exit to common ancestor
  for (uint8_t i = currentAncestryDepth; i > commonAncestorIndex; i--) {
    Result result = (this->*(currentAncestry[i]))(Event{ .signal = EVENT_EXIT });
    if (result.type == RESULT_ERROR)
      return;  // TODO: error
  }

  // enter to target state
  for (uint8_t i = commonAncestorIndex + 1; i <= targetAncestryDepth; i++) {
    Result result = (this->*(targetAncestry[i]))(Event{ .signal = EVENT_ENTER });
    if (result.type == RESULT_ERROR)
      return;  // TODO: error
  }

  state = targetState;

  // enter any child states
  Result result = { .type = RESULT_CHILD, .target = targetState };
  while (result.target) {
    result = (this->*(result.target))(Event{ .signal = EVENT_REQUEST_CHILD });
    if (result.target) {
      (this->*(result.target))(Event{ .signal = EVENT_ENTER });
      state = result.target;
    }
  }
}

Result StateMachine::handle(Event event) {
  return processEvent(state, event);
}

Result StateMachine::processEvent(State targetState, Event event) {
  Result result = (this->*targetState)(event);

  switch (result.type) {
    case RESULT_UNHANDLED:
      {  // pass to parent to handle
        State parent =
          (this->*targetState)(Event{ .signal = EVENT_REQUEST_PARENT }).target;
        if (!state)
          return Result{ .type = RESULT_ERROR, .target = state };

        return processEvent(parent, event);
      }
    case RESULT_TRANSITION:
      {
        transition(result.target);
        break;
      }
    case RESULT_HANDLED:
    case RESULT_ERROR:
      return result;
    case RESULT_PARENT:
    case RESULT_CHILD:
      return Result{ .type = RESULT_ERROR, .target = result.target };
  }

  return result;
}
}  // namespace StateMachine
