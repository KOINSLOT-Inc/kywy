// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * I2C Device Scanner for Kywy
 * 
 * This example scans the I2C bus every second looking for connected devices.
 * When devices are found, it displays their addresses on screen.
 * The display updates automatically when devices are connected or disconnected.
 * 
 * I2C pins on Raspberry Pi Pico: GP4 = SDA, GP5 = SCL
 */

#include <Wire.h>
#include "Kywy.hpp"

Kywy::Engine engine;

// Store the last scan results to detect changes
uint8_t previousDevices[20];
int previousCount = 0;

void setup() {
  // Initialize I2C communication
  Wire.begin();

  // Start the Kywy engine and clear display
  engine.start();
  engine.display.clear();

  // Draw the title and status
  engine.display.drawText(
    KYWY_DISPLAY_WIDTH / 2, 10,
    "I2C Device Scanner",
    Display::TextOptions().origin(Display::Origin::Text::CENTER));

  engine.display.drawText(
    KYWY_DISPLAY_WIDTH / 2, 22,
    "Scanning...",
    Display::TextOptions().origin(Display::Origin::Text::CENTER));

  engine.display.update();

  // Perform initial scan
  scanAndUpdate();
}

void loop() {
  // Wait 1 second between scans
  delay(1000);

  // Scan for devices and update display if needed
  scanAndUpdate();
}

void scanAndUpdate() {
  // Array to store currently found devices
  uint8_t currentDevices[20];
  int currentCount = 0;

  // Scan all valid I2C addresses (0x08 to 0x77)
  for (uint8_t address = 8; address < 120; address++) {
    // Try to communicate with this address
    Wire.beginTransmission(address);

    // If device responds (returns 0), add it to our list
    if (Wire.endTransmission() == 0) {
      if (currentCount < 20) {  // Don't overflow our array
        currentDevices[currentCount] = address;
        currentCount++;
      }
    }
  }

  // Check if the device list has changed
  if (hasDeviceListChanged(currentDevices, currentCount)) {
    updateDisplay(currentDevices, currentCount);
    saveCurrentScan(currentDevices, currentCount);
  }
}

bool hasDeviceListChanged(uint8_t* devices, int count) {
  // Different number of devices means change
  if (count != previousCount) {
    return true;
  }

  // Same count, but check if any addresses are different
  for (int i = 0; i < count; i++) {
    if (devices[i] != previousDevices[i]) {
      return true;
    }
  }

  return false;  // No changes detected
}

void updateDisplay(uint8_t* devices, int count) {
  // Clear the results area (keep title and status)
  engine.display.fillRectangle(0, 34, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT - 34,
                               Display::Object2DOptions().color(0xFF));

  if (count == 0) {
    // No devices found
    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 46,
      "No devices found",
      Display::TextOptions().origin(Display::Origin::Text::CENTER));
  } else {
    // Show how many devices were found
    char message[30];
    snprintf(message, sizeof(message), "Found %d device%s:",
             count, count == 1 ? "" : "s");

    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 46,
      message,
      Display::TextOptions().origin(Display::Origin::Text::CENTER));

    // List each device address
    int yPosition = 58;
    for (int i = 0; i < count; i++) {
      char addressText[10];
      snprintf(addressText, sizeof(addressText), "0x%02X", devices[i]);

      engine.display.drawText(
        KYWY_DISPLAY_WIDTH / 2, yPosition,
        addressText,
        Display::TextOptions().origin(Display::Origin::Text::CENTER));

      yPosition += 12;  // Move down for next device

      // Stop if we run out of screen space
      if (yPosition > 150) break;
    }
  }

  // Update the display
  engine.display.update();
}

void saveCurrentScan(uint8_t* devices, int count) {
  // Remember this scan for next time
  previousCount = count;
  for (int i = 0; i < count; i++) {
    previousDevices[i] = devices[i];
  }
}
