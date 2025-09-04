// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * I2C Capacitive Soil Moisture Sensor Example for Kywy
 * With Animated Plant Monster Sprite
 * 
 * LEARNING OBJECTIVES:
 * - Understand I2C communication with register-based sensors
 * - Learn to read multi-byte data (big-endian format)
 * - Practice sensor calibration and data conversion
 * - Implement real-time sensor monitoring with error handling
 * - Display sprite animations based on sensor data
 * 
 * WHAT THIS EXAMPLE DOES:
 * - Reads soil moisture using capacitive sensing technology
 * - Reads temperature from an onboard sensor
 * - Displays live sensor data with visual indicators
 * - Shows animated plant monster sprite that reacts to moisture levels
 * - Demonstrates sprite sheet animation techniques
 * 
 * SPRITE ANIMATIONS:
 * - Normal state: Row 1 animation (happy plant monster)
 * - Low moisture: Row 3 animation (thirsty plant monster)
 * - Watering detected: Row 2 animation (excited plant monster, 2 loops)
 * 
 * HARDWARE SETUP:
 * - Uses STEMMA Soil Sensor from Adafruit (ATSAMD10-based)
 * - Connect: VCC→3.3V, GND→GND, SDA→GP4, SCL→GP5
 * - Default I2C address: 0x36 (configurable to 0x39)
 * - Uses "seesaw" protocol for sensor communication
 */

#include <Wire.h>
#include "Kywy.hpp"
#include "plant_monster_sprites.h"

// === SENSOR CONFIGURATION ===
const uint8_t SENSOR_I2C_ADDRESS = 0x36;  // Can be 0x36 or 0x39

// === SEESAW PROTOCOL CONSTANTS ===
// The sensor uses a protocol called "seesaw" with different modules
const uint8_t MODULE_STATUS = 0x00;  // Status and temperature module
const uint8_t MODULE_TOUCH = 0x0F;   // Capacitive touch sensing module

// Register addresses within each module
const uint8_t REG_TEMPERATURE = 0x04;         // Temperature register
const uint8_t REG_TOUCH_CHANNEL_BASE = 0x10;  // Base address for touch channels

// Hardware channel assignments
const uint8_t MOISTURE_CHANNEL = 0;  // Touch channel 0 is connected to moisture sensor

// === SPRITE ANIMATION CONFIGURATION ===
// Plant monster sprite sheet organization (3x4 grid):
// Row 1 (frames 0-2): Normal/happy state
// Row 2 (frames 3-5): Watering detected (excited)
// Row 3 (frames 6-8): Low moisture (thirsty)
// Row 4 (frames 9-11): Not used

enum PlantState {
  PLANT_NORMAL,   // Happy, content plant
  PLANT_THIRSTY,  // Low moisture, needs water
  PLANT_WATERING  // Watering detected, excited
};

// Animation timing
const unsigned long FRAME_DURATION = 500;       // 500ms per frame
const unsigned long ANIMATION_INTERVAL = 2000;  // Check for state changes every 2s

// === GLOBAL VARIABLES ===
Kywy::Engine engine;

// Sensor state tracking
bool sensorConnected = false;
uint16_t moistureReading = 0;   // Raw capacitive touch value
uint16_t previousMoisture = 0;  // Previous reading for change detection
float temperatureC = 0.0;       // Temperature in Celsius
unsigned long lastUpdateTime = 0;

// Sprite animation state
PlantState currentPlantState = PLANT_NORMAL;
uint8_t currentFrame = 0;
unsigned long lastFrameTime = 0;
unsigned long lastStateChangeTime = 0;
uint8_t wateringLoopCount = 0;  // Count loops during watering animation

// Sprite frame arrays for each animation state
const uint8_t* normalFrames[] = { PLANT_frame_0, PLANT_frame_1, PLANT_frame_2 };
const uint8_t* thirstyFrames[] = { PLANT_frame_6, PLANT_frame_7, PLANT_frame_8 };
const uint8_t* wateringFrames[] = { PLANT_frame_3, PLANT_frame_4, PLANT_frame_5 };

// Debug display mode
bool showDebugInfo = false;

void setup() {
  // === HARDWARE INITIALIZATION ===

  // Initialize I2C communication bus
  Wire.begin();

  // Start the Kywy graphics engine
  engine.start();
  engine.display.clear();

  // === DISPLAY STARTUP SCREEN ===

  // Show application title
  engine.display.drawText(
    KYWY_DISPLAY_WIDTH / 2, 64,
    "Soil Sensor Monitor",
    Display::TextOptions().origin(Display::Origin::Text::CENTER));


  engine.display.update();
  delay(2000);  // Show startup screen for 2 seconds

  // === INITIAL SENSOR SETUP ===

  // Check if sensor is present on I2C bus
  checkSensorConnection();

  // Take first reading if sensor is connected
  if (sensorConnected) {
    readAllSensorData();
  }

  // Display initial data
  updateDisplay();
}

void loop() {
  // === MAIN SENSOR READING LOOP ===

  // Update sensor readings every 1000ms (1 second)
  if (millis() - lastUpdateTime >= 1000) {

    if (sensorConnected) {
      // Store previous moisture for change detection
      previousMoisture = moistureReading;

      // Sensor is connected - read current values
      readAllSensorData();

      // Update plant state based on sensor data
      updatePlantState();
    } else {
      // Sensor not connected - check if it's been reconnected
      checkSensorConnection();
    }

    // Update the display with latest data
    updateDisplay();

    // Remember when we last updated
    lastUpdateTime = millis();
  }

  // Update sprite animation frame
  updateSpriteAnimation();

  // Check for button input to toggle debug mode
  handleButtonInput();

  // Small delay to prevent overwhelming the processor
  delay(50);
}

void handleButtonInput() {
  // === BUTTON INPUT HANDLING ===

  // Check if right button is pressed to toggle debug mode
  if (engine.input.buttonRightPressed) {
    showDebugInfo = !showDebugInfo;  // Toggle debug display

    // Update display immediately to show change
    updateDisplay();

    // Wait for button release to prevent rapid toggling
    while (engine.input.buttonRightPressed) {
      delay(10);
    }
  }
}

void updatePlantState() {
  // === DETERMINE PLANT STATE BASED ON SENSOR DATA ===

  uint8_t moisturePercent = convertToMoisturePercentage();

  // Detect sudden moisture increase (watering event)
  int16_t moistureChange = (int16_t)moistureReading - (int16_t)previousMoisture;

  if (moistureChange > 100 && moisturePercent > 30) {
    // Significant moisture increase detected - watering event!
    if (currentPlantState != PLANT_WATERING) {
      currentPlantState = PLANT_WATERING;
      currentFrame = 0;
      wateringLoopCount = 0;
      lastStateChangeTime = millis();
      lastFrameTime = millis();
    }
  } else if (currentPlantState == PLANT_WATERING) {
    // Check if we've completed 2 loops of watering animation
    if (wateringLoopCount >= 2) {
      // Switch to appropriate state based on moisture level
      currentPlantState = (moisturePercent < 30) ? PLANT_THIRSTY : PLANT_NORMAL;
      currentFrame = 0;
      lastFrameTime = millis();
    }
  } else {
    // Normal state determination based on moisture level
    PlantState newState = (moisturePercent < 30) ? PLANT_THIRSTY : PLANT_NORMAL;

    if (newState != currentPlantState) {
      currentPlantState = newState;
      currentFrame = 0;
      lastFrameTime = millis();
    }
  }
}

void updateSpriteAnimation() {
  // === UPDATE SPRITE ANIMATION FRAME ===

  // Check if it's time to advance to the next frame
  if (millis() - lastFrameTime >= FRAME_DURATION) {
    currentFrame++;

    // Handle frame wrapping based on current state
    if (currentFrame >= 3) {  // All animations have 3 frames
      currentFrame = 0;

      // If we're in watering state, count completed loops
      if (currentPlantState == PLANT_WATERING) {
        wateringLoopCount++;
      }
    }

    lastFrameTime = millis();
  }
}

void drawPlantSprite() {
  // === DRAW CURRENT PLANT SPRITE FRAME ===

  const uint8_t* frameData = nullptr;

  // Select appropriate frame based on current state
  switch (currentPlantState) {
    case PLANT_NORMAL:
      frameData = normalFrames[currentFrame];
      break;
    case PLANT_THIRSTY:
      frameData = thirstyFrames[currentFrame];
      break;
    case PLANT_WATERING:
      frameData = wateringFrames[currentFrame];
      break;
  }

  // Draw sprite centered horizontally, positioned below moisture bar
  if (frameData != nullptr) {
    int spriteX = (KYWY_DISPLAY_WIDTH - PLANT_FRAME_WIDTH) / 2;
    int spriteY = 50;  // Below moisture bar, above temperature/time

    engine.display.drawBitmap(spriteX, spriteY,
                              PLANT_FRAME_WIDTH,
                              PLANT_FRAME_HEIGHT,
                              (uint8_t*)frameData);
  }
}

void checkSensorConnection() {
  // === I2C DEVICE DETECTION ===

  // Try to start communication with the sensor
  Wire.beginTransmission(SENSOR_I2C_ADDRESS);

  // Check if device acknowledges (returns 0 = success)
  sensorConnected = (Wire.endTransmission() == 0);
}

bool readSensorRegister(uint8_t moduleAddress, uint8_t registerAddress, uint8_t* dataBuffer, uint8_t bytesToRead) {
  // === SEESAW PROTOCOL REGISTER READING ===

  // Step 1: Tell sensor which register we want to read
  Wire.beginTransmission(SENSOR_I2C_ADDRESS);
  Wire.write(moduleAddress);    // Select the module (status, touch, etc.)
  Wire.write(registerAddress);  // Select the register within that module

  // Check if sensor acknowledged our request
  if (Wire.endTransmission() != 0) {
    return false;  // Communication failed
  }

  // Step 2: Give sensor time to prepare the data
  delay(10);  // Small delay for sensor processing

  // Step 3: Request the data from the sensor
  Wire.requestFrom(SENSOR_I2C_ADDRESS, bytesToRead);

  // Step 4: Read the response into our buffer
  uint8_t bytesReceived = 0;
  while (Wire.available() && bytesReceived < bytesToRead) {
    dataBuffer[bytesReceived] = Wire.read();
    bytesReceived++;
  }

  // Return true if we got all the bytes we expected
  return (bytesReceived == bytesToRead);
}

void readAllSensorData() {
  // === READ TEMPERATURE FROM STATUS MODULE ===

  // Temperature is stored as a 32-bit value in big-endian format
  uint8_t temperatureBytes[4];
  if (readSensorRegister(MODULE_STATUS, REG_TEMPERATURE, temperatureBytes, 4)) {

    // Convert 4 bytes from big-endian to 32-bit integer
    // Big-endian means most significant byte comes first
    uint32_t temperatureRaw = ((uint32_t)temperatureBytes[0] << 24) |  // Byte 0: bits 31-24
                              ((uint32_t)temperatureBytes[1] << 16) |  // Byte 1: bits 23-16
                              ((uint32_t)temperatureBytes[2] << 8) |   // Byte 2: bits 15-8
                              temperatureBytes[3];                     // Byte 3: bits 7-0

    // Convert to Celsius (sensor returns temperature × 65536)
    temperatureC = (float)temperatureRaw / 65536.0;
  }

  // === READ MOISTURE FROM TOUCH MODULE ===

  // Capacitive moisture reading is a 16-bit value in big-endian format
  uint8_t moistureBytes[2];
  if (readSensorRegister(MODULE_TOUCH, REG_TOUCH_CHANNEL_BASE + MOISTURE_CHANNEL, moistureBytes, 2)) {

    // Convert 2 bytes from big-endian to 16-bit integer
    moistureReading = ((uint16_t)moistureBytes[0] << 8) |  // High byte first
                      moistureBytes[1];                    // Low byte second
  }
}

uint8_t convertToMoisturePercentage() {
  // === SENSOR CALIBRATION AND CONVERSION ===

  // Convert raw capacitive reading to meaningful percentage
  // These values need to be calibrated for your specific soil type:

  // Typical capacitive touch sensor readings:
  // - In air (completely dry): ~200-300
  // - Dry soil: ~300-600
  // - Moist soil: ~600-900
  // - In water (saturated): ~900-1000+

  const uint16_t DRY_THRESHOLD = 200;   // Calibrate: measure sensor in dry soil
  const uint16_t WET_THRESHOLD = 1000;  // Calibrate: measure sensor in wet soil

  // Handle edge cases
  if (moistureReading <= DRY_THRESHOLD) return 0;    // 0% moisture
  if (moistureReading >= WET_THRESHOLD) return 100;  // 100% moisture

  // Linear interpolation between dry and wet thresholds
  return map(moistureReading, DRY_THRESHOLD, WET_THRESHOLD, 0, 100);
}

void updateDisplay() {
  // === DISPLAY UPDATE FUNCTION ===

  // Clear previous display content
  engine.display.clear();

  // Application title
  engine.display.drawText(
    KYWY_DISPLAY_WIDTH / 2, 8,
    "Plant Monitor",
    Display::TextOptions().origin(Display::Origin::Text::CENTER));

  if (!sensorConnected) {
    // === SENSOR DISCONNECTED SCREEN ===

    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 30,
      "Sensor Not Found",
      Display::TextOptions().origin(Display::Origin::Text::CENTER));

    char addressText[25];
    snprintf(addressText, sizeof(addressText), "Looking for: 0x%02X", SENSOR_I2C_ADDRESS);
    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 45,
      addressText,
      Display::TextOptions().origin(Display::Origin::Text::CENTER));

    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 60,
      "Check connections:",
      Display::TextOptions().origin(Display::Origin::Text::CENTER));

    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 75,
      "VCC, GND, SDA, SCL",
      Display::TextOptions().origin(Display::Origin::Text::CENTER));

  } else {
    // === SENSOR DATA DISPLAY WITH CENTERED SPRITE ===

    uint8_t moisturePercent = convertToMoisturePercentage();

    // Draw moisture bar at top (like a health bar)
    drawMoistureBar(moisturePercent);

    // Draw animated plant sprite centered below the bar
    drawPlantSprite();

    // Plant state description below sprite
    const char* stateDescription = getPlantStateDescription();
    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 125,
      stateDescription,
      Display::TextOptions().origin(Display::Origin::Text::CENTER));

    // Debug information (shown when right button is pressed)
    // Position between status message and bottom corner text
    if (showDebugInfo) {
      char rawText[20];
      snprintf(rawText, sizeof(rawText), "Raw: %d", moistureReading);
      engine.display.drawText(
        KYWY_DISPLAY_WIDTH / 2, 140,
        rawText,
        Display::TextOptions().origin(Display::Origin::Text::CENTER));

      char addressText[20];
      snprintf(addressText, sizeof(addressText), "I2C: 0x%02X", SENSOR_I2C_ADDRESS);
      engine.display.drawText(
        KYWY_DISPLAY_WIDTH / 2, 153,
        addressText,
        Display::TextOptions().origin(Display::Origin::Text::CENTER));
    }

    // Temperature and time in bottom corners - FIXED POSITION, never moves
    int bottomTextY = KYWY_DISPLAY_HEIGHT - 4;  // Always 4px from bottom edge

    char tempText[12];
    snprintf(tempText, sizeof(tempText), "%.1f°C", temperatureC);
    engine.display.drawText(4, bottomTextY, tempText,
                            Display::TextOptions().origin(Display::Origin::Text::BASELINE_LEFT));  // Bottom left corner, 4px from edges

    char timeText[12];
    snprintf(timeText, sizeof(timeText), "%lus", millis() / 1000);
    engine.display.drawText(KYWY_DISPLAY_WIDTH - 4, bottomTextY, timeText,
                            Display::TextOptions().origin(Display::Origin::Text::BASELINE_RIGHT));  // Bottom right corner, 4px from edges
  }

  // Update the physical display
  engine.display.update();
}

const char* getPlantStateDescription() {
  // === PLANT STATE INTERPRETATION ===

  switch (currentPlantState) {
    case PLANT_NORMAL:
      return "Happy";
    case PLANT_THIRSTY:
      return "Thirsty";
    case PLANT_WATERING:
      return "Drinking!";
    default:
      return "Unknown";
  }
}

const char* getMoistureDescription(uint8_t percentage) {
  // === MOISTURE LEVEL INTERPRETATION ===

  // Convert percentage to human-readable description
  if (percentage < 25) {
    return "Very Dry";
  } else if (percentage < 50) {
    return "Dry";
  } else if (percentage < 75) {
    return "Moist";
  } else {
    return "Very Wet";
  }
}

void drawMoistureBar(uint8_t percentage) {
  // === VISUAL MOISTURE LEVEL INDICATOR (HEALTH BAR STYLE) ===

  // Bar graph dimensions - positioned at top like a health bar
  const int BAR_WIDTH = 80;
  const int BAR_HEIGHT = 8;
  const int BAR_X = 20;  // Left side of screen
  const int BAR_Y = 25;  // Below title

  // Draw border around the bar
  engine.display.drawRectangle(BAR_X - 1, BAR_Y - 1, BAR_WIDTH + 2, BAR_HEIGHT + 2,
                               Display::Object2DOptions().color(0x00));  // Black border

  // Fill background with white
  engine.display.fillRectangle(BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT,
                               Display::Object2DOptions().color(0xFF));  // White background

  // Calculate how much of the bar to fill based on percentage
  int fillWidth = (BAR_WIDTH * percentage) / 100;

  // Fill the bar proportionally to the moisture level
  if (fillWidth > 0) {
    engine.display.fillRectangle(BAR_X, BAR_Y, fillWidth, BAR_HEIGHT,
                                 Display::Object2DOptions().color(0x00));  // Black fill
  }

  // Add percentage label to the right of the bar (aligned with bar center)
  char percentText[8];
  snprintf(percentText, sizeof(percentText), "%d%%", percentage);
  engine.display.drawText(BAR_X + BAR_WIDTH + 8, BAR_Y, percentText);  // Moved up 4px to align with bar
}
