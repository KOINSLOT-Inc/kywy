// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// Choose your own adventure game
//
// This example shows:
//   -How to use if/else statements to make decisions
//   -How to use buttons to navigate between screens
//   -How to draw pictures and text on screen
//

#include "Kywy.hpp"
#include "screens.hpp"

Kywy::Engine engine;  // This starts the kywy firmware and engine

// Game variables - these store our game information

// Here we use some variables that are defined in the Kywy library from when we included Kywy.hpp
// It will be useful to know where the center of the screen is
int centerX = KYWY_DISPLAY_WIDTH / 2;   // Middle of screen horizontally
int centerY = KYWY_DISPLAY_HEIGHT / 2;  // Middle of screen vertically

//Variables to track which screen we're on
bool onRightScreen = false;
bool onLeftScreen = false;

// Button state tracking for proper button press detection
bool wasRightButtonPressed = false;  // Was the right button pressed last time we checked?
bool wasLeftButtonPressed = false;   // Was the left button pressed last time we checked?

void setup() {
  engine.start();                                                                         // This will start up the kywy engine along with things like the display, usb, buttons, etc.
  engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen);  // Draw the starting screen
  engine.display.drawText(centerX, centerY - 20, "Go Right");                             // Draw text
  engine.display.drawText(centerX - 60, centerY - 20, "Go Left");                         // Draw text
}

void loop() {

  //Here we will check if a button is pressed and decide which screen to show

  // Check if RIGHT button was just pressed (not held down)
  if (engine.input.buttonRightPressed && !wasRightButtonPressed) {
    if (!onRightScreen && !onLeftScreen) {                                                    // We're on the start screen
      engine.display.clear();                                                                 // Clear the screen and draw the right screen
      engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, rightScreen);  // Change to right screen
      onRightScreen = true;                                                                   //Track that we're on the right screen
      onLeftScreen = false;                                                                   //Track that we're not on the left screen
    } else if (onLeftScreen) {                                                                // We're on the left screen - go back to start
      engine.display.clear();                                                                 // Clear the screen and draw the start screen
      engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen);  // Change to start screen
      onLeftScreen = false;                                                                   //Track that we're not on the left screen
      onRightScreen = false;                                                                  //Track that we're not on the right screen
      engine.display.drawText(centerX, centerY - 20, "Go Right");                             // Draw text
      engine.display.drawText(centerX - 60, centerY - 20, "Go Left");                         // Draw text
    }
  }

  // Check if LEFT button was just pressed (not held down)
  if (engine.input.buttonLeftPressed && !wasLeftButtonPressed) {
    if (!onRightScreen && !onLeftScreen) {                                                    // We're on the start screen
      engine.display.clear();                                                                 // Clear the screen and draw the left screen
      engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, leftScreen);   // Change to left screen
      onLeftScreen = true;                                                                    //Track that we're on the left screen
      onRightScreen = false;                                                                  //Track that we're not on the right screen
    } else if (onRightScreen) {                                                               // We're on the right screen - go back to start
      engine.display.clear();                                                                 // Clear the screen and draw the start screen
      engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen);  // Change to start screen
      onRightScreen = false;                                                                  //Track that we're not on the right screen
      onLeftScreen = false;                                                                   //Track that we're not on the left screen
      engine.display.drawText(centerX, centerY - 20, "Go Right");                             // Draw text
      engine.display.drawText(centerX - 60, centerY - 20, "Go Left");                         // Draw text
    }
  }

  // Remember button states for next time
  wasRightButtonPressed = engine.input.buttonRightPressed;
  wasLeftButtonPressed = engine.input.buttonLeftPressed;

  //Could you extend this game to have more screens and more choices?
  //How about a story with multiple choices at each step?

  // Show everything on screen
  engine.display.update();
}