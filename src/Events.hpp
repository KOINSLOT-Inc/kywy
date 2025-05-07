// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_EVENTS
#define KYWY_LIB_EVENTS 1

namespace Kywy::Events {

typedef enum : int {
  // Clock Events
  TICK,
  SET_TICK_DURATION,

  // Input Events
  INPUT,          // used for "any button" interactions
  INPUT_PRESSED,  // used for "any button pressed" interactions (not released)
  D_PAD,          // used for "any d-pad" interactions
  D_PAD_PRESSED,  // used for "any d-pad pressed" interactions (not released)
  BUTTON_LEFT_PRESSED,
  BUTTON_LEFT_RELEASED,
  BUTTON_RIGHT_PRESSED,
  BUTTON_RIGHT_RELEASED,
  D_PAD_LEFT_PRESSED,
  D_PAD_LEFT_RELEASED,
  D_PAD_RIGHT_PRESSED,
  D_PAD_RIGHT_RELEASED,
  D_PAD_UP_PRESSED,
  D_PAD_UP_RELEASED,
  D_PAD_DOWN_PRESSED,
  D_PAD_DOWN_RELEASED,
  D_PAD_CENTER_PRESSED,
  D_PAD_CENTER_RELEASED,

  // User Event Boundary
  USER_EVENTS,

  MENU_MESSAGE,  // Specific message for menu-related actions

} KywyEvents;

}  // namespace Kywy::Events

#endif
