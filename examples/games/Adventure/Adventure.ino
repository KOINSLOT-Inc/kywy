// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// Choose your own adventure game
//
// This example shows:
//   -How to use if/else statements to make decisions
//   -How to use d-pad to navigate between screens
//   -How to draw pictures and text on screen
//   -How to add story text to game screens
//   -How to create and use helper functions for code reuse

#include "Kywy.hpp"    // Main kywy library for controlling the KYWY
#include "screens.hpp" // This file includes all of our images for the game

Kywy::Engine engine;  // This starts the kywy firmware and engine

// Here we use some variables that are defined in the Kywy library from when we included Kywy.hpp
// It will be useful to know where the center of the screen is
int centerX = KYWY_DISPLAY_WIDTH / 2;   // Middle of screen horizontally
int centerY = KYWY_DISPLAY_HEIGHT / 2;  // Middle of screen vertically
uint16_t textWidth, textHeight;         //Need to store information about the text size

// Enum for button choices, this gives us easy to read names for the buttons
// Using the existing library enums for d-pad input instead of buttons
using ButtonEvent = Kywy::Events::KywyEvents;

// Helper function to wait for input and return which d-pad direction was pressed
// Dont worry too much about what this function is doing, just know it waits for a button press and returns which button was pressed
ButtonEvent waitForInput() {
  ButtonEvent pressed = Kywy::Events::KywyEvents::INPUT;  // Use INPUT as NONE equivalent
  
  // Check initial state of buttons
  bool initialLeftPressed = engine.input.dPadLeftPressed;
  bool initialRightPressed = engine.input.dPadRightPressed;
  
  // Track previous state to make sure a button was not already held down from the last time we checked
  bool wasLeftPressed = initialLeftPressed;
  bool wasRightPressed = initialRightPressed;
  
  while (pressed == Kywy::Events::KywyEvents::INPUT) {
    bool leftPressed = engine.input.dPadLeftPressed;
    bool rightPressed = engine.input.dPadRightPressed;
    
    // Detect button press: current is pressed AND previous was not pressed
    if (leftPressed && !wasLeftPressed) {
      pressed = Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED;
    } else if (rightPressed && !wasRightPressed) {
      pressed = Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED;
    }
    
    wasLeftPressed = leftPressed;
    wasRightPressed = rightPressed;
    engine.display.update();
    delay(10); // Small delay before we check the buttons again
  }
  return pressed;
}

// Make a function to draw text boxes for us
void drawInstructionText() {
  //Right text
  engine.display.getTextSize("Go Right", textWidth, textHeight);                                                                     //Get size of text
  engine.display.fillRectangle(centerX + 5, centerY - 25, textWidth + 10, textHeight + 10, Display::Object2DOptions().color(0xFF));  //Draw a white rectangle background for text
  engine.display.drawRectangle(centerX + 5, centerY - 25, textWidth + 10, textHeight + 10, Display::Object2DOptions().color(0x00));  //Draw a rectangle outline for text
  engine.display.drawText(centerX + 10, centerY - 20, "Go Right");                                                                   // Draw text
  //Left text
  engine.display.getTextSize("Go Left", textWidth, textHeight);                                              //Get size of text
  engine.display.fillRectangle(centerX - 65, centerY - 25, 60, 20, Display::Object2DOptions().color(0xFF));  //Draw a white rectangle background for text
  engine.display.drawRectangle(centerX - 65, centerY - 25, 60, 20, Display::Object2DOptions().color(0x00));  //Draw a rectangle outline for text
  engine.display.drawText(centerX - 60, centerY - 20, "Go Left");                                            // Draw text                             // Draw text
}

// Helper function to draw text at the bottom of the screen
// We will use this to add story elements to the bottom of our game screens
void drawBottomText(const char* text) {
  engine.display.getTextSize(text, textWidth, textHeight);
  engine.display.fillRectangle(0, KYWY_DISPLAY_HEIGHT - textHeight - 10, KYWY_DISPLAY_WIDTH, textHeight + 10, Display::Object2DOptions().color(0xFF));
  engine.display.drawRectangle(0, KYWY_DISPLAY_HEIGHT - textHeight - 10, KYWY_DISPLAY_WIDTH, textHeight + 10, Display::Object2DOptions().color(0x00));
  engine.display.drawText(5, KYWY_DISPLAY_HEIGHT - textHeight - 5, text);
}

void setup() {
  engine.start();  // This will start up the kywy engine along with things like the display, usb, buttons, etc.
}

// Game variable initialization
int score = 0; // Player score
int health = 100; // Player health

// The main game loop
// Edit this!!
void loop() {
  // Show start screen
  engine.display.clear();
  engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen); // Draws the start screen bitmap
  drawInstructionText(); // Adds the standard text options for going left or right

  // Wait for input
  ButtonEvent choice = waitForInput(); // Wait for a button to be pressed and store it into choice

  if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
    // Show left screen
    engine.display.clear(); // Clear the old screen to prepare for new one
    engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, leftScreen);
    score += 10; // Increase score for going left
    // Add text at the bottom of the screen
    drawBottomText("You found a reward!");
  } 
  else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
    // Show right screen
    engine.display.clear(); // Clear the old screen to prepare for new one
    engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, rightScreen);
    health -= 10; // Decrease health for going right
    // Add text at the bottom of the screen
    drawBottomText("You were attacked!");
  }

  choice = waitForInput(); // Wait for a button to be pressed and store it into choice
  // Here we get an input again, but we dont do anything with it
  // You can use this to add more screens and choices to your game
  // But for now, we'll just loop back to the start
  // Maybe we can do something with the score and health variables later?

  // Loop back to start
}