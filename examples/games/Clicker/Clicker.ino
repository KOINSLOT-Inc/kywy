// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// Cookie Clicker Game - Simple Version
//
// This example shows:
//   - How to use variables to count things
//   - How to check if buttons are pressed
//   - How to draw pictures and text on screen
//   - How to make simple animations
//
// Press the RIGHT button to click the cookie and get points!

#include "Kywy.hpp"
#include "cookie.hpp"

Kywy::Engine engine;  // This starts the kywy firmware and engine

// Game variables - these store our game information
// Here we are creating some global values to track important information for our game
int cookieCount = 0;            // How many cookies we've clicked
bool showAnimation = false;     // Should we show the click animation?
int animationTime = 0;          // How long has the animation been running?
bool wasButtonPressed = false;  // Was the button pressed last time we checked?

// Where to draw the cookie on screen
// Here we use some variable that are defined in the Kywy library from when we included Kywy.hpp
int cookieX = KYWY_DISPLAY_WIDTH / 2;   // Middle of screen horizontally
int cookieY = KYWY_DISPLAY_HEIGHT / 2;  // Middle of screen vertically


void setup() {
  engine.start();  // This will start up the kywy engine along with things like the display, usb, buttons, etc.
}

void loop() {
  // Check if RIGHT button was just pressed (not held down)
  // Only count if button is pressed now AND was not pressed before
  if (engine.input.buttonRightPressed && !wasButtonPressed) {  // Button was just pressed and was not pressed during the last loop
    cookieCount++;                                             // Add one more cookie
    showAnimation = true;                                      // Start the click animation
    animationTime = 0;                                         // Reset animation timer
  }

  // Remember if button is pressed for next time
  // This will save the current state of the button for the next loop
  wasButtonPressed = engine.input.buttonRightPressed;

  // Update the animation
  if (showAnimation) {
    animationTime++;
    if (animationTime > 30) {  // Stop animation after 30 loops
      showAnimation = false;
    }
  }

  // Clear the screen
  engine.display.clear();

  // Draw the score at the top
  String scoreText = "Cookies: " + String(cookieCount);                                   // Create text to show score
  engine.display.drawText(cookieX, 20, scoreText,                                         // Draw the score text to the screen buffer - no .c_str() needed!
                          Display::TextOptions().origin(Display::Origin::Text::CENTER));  // Center the text

  // Draw the cookie picture in the center
  // Here we are using some of the variable that were defined when we included cookie.hpp
  engine.display.drawBitmap(
    cookieX - COOKIE_WIDTH / 2,   // Left edge of cookie
    cookieY - COOKIE_HEIGHT / 2,  // Top edge of cookie
    COOKIE_WIDTH, COOKIE_HEIGHT,
    cookie_data);

  // Draw click animation (circles around cookie when clicked)
  if (showAnimation) {
    int circleSize = 70 + (animationTime * 2);  // Circle grows bigger over time
    engine.display.drawCircle(cookieX, cookieY, circleSize,
                              Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
  }

  // Show everything on screen
  engine.display.update();
}