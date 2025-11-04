// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later


// =================================================================== //
//
//                      Name:  Your Name
//                      Date:  Today
//                   Project:  Your Project Name
//               Description:  Your project description
//
// =================================================================== //

// Get started and more at https://www.kywy.io
// For help, see the docs: https://docs.kywy.io/
// For tools like the drawing editor and more see: https://tools.kywy.io/

#include "Kywy.hpp"

Kywy::Engine engine;

void setup() {  //                                         🚀 (SETUP)
  // Run once at startup. Initialize here
  engine.clock.setTickDuration(30);  // Set time between ticks
  engine.start();
}  // end setup

void loop() {  //                                          🔄 (LOOP)
  // Run continuously (no delay)
}  // end loop

void onTick(Actor::Message *message) {  //                 ⏱️ (ONTICK)
  // Run every clock tick i.e. draw to display
  // See display functions here: https://docs.kywy.io/api_reference/
}  // end onTick

void onInput(Actor::Message *message) {  //                🎮 (ONINPUT)
  // Do stuff when buttons are pressed/released
  switch (message->signal) {
    case Kywy::Events::BUTTON_LEFT_PRESSED:  //   🟢 (LEFT PRESSED)
      //  Handle Left button press here
      break;

    case Kywy::Events::BUTTON_RIGHT_PRESSED:  //  🟢 (RIGHT PRESSED)
      //  Handle Right button press here
      break;

    case Kywy::Events::D_PAD_LEFT_PRESSED:  //    ◀️ (D-PAD LEFT PRESSED)
      //  Handle D-Pad Left press here
      break;

    case Kywy::Events::D_PAD_RIGHT_PRESSED:  //   ▶️ (D-PAD RIGHT PRESSED)
      //  Handle D-Pad Right press here
      break;

    case Kywy::Events::D_PAD_UP_PRESSED:  //      🔼 (D-PAD UP PRESSED)
      //  Handle D-Pad Up press here
      break;

    case Kywy::Events::D_PAD_DOWN_PRESSED:  //    🔽 (D-PAD DOWN PRESSED)
      //  Handle D-Pad Down press here
      break;

    case Kywy::Events::BUTTON_LEFT_RELEASED:  //  🔴 (LEFT RELEASED)
      //  Handle Left button release here
      break;

    case Kywy::Events::BUTTON_RIGHT_RELEASED:  // 🔴 (RIGHT RELEASED)
      //  Handle Right button release here
      break;

    case Kywy::Events::D_PAD_LEFT_RELEASED:  //   ⬅️ (D-PAD LEFT RELEASED)
      //  Handle D-Pad Left release here
      break;

    case Kywy::Events::D_PAD_RIGHT_RELEASED:  //  ➡️ (D-PAD RIGHT RELEASED)
      //  Handle D-Pad Right release here
      break;

    case Kywy::Events::D_PAD_UP_RELEASED:  //     ⬆️ (D-PAD UP RELEASED)
      //  Handle D-Pad Up release here
      break;

    case Kywy::Events::D_PAD_DOWN_RELEASED:  //   ⬇️ (D-PAD DOWN RELEASED)
      //  Handle D-Pad Down release here
      break;

  }  // end switch message->signal
}  // end onInput
