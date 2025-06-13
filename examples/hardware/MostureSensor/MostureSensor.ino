// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * I2C Capacitive Soil Moisture Sensor Example for Kywy
 * 
 * LEARNING OBJECTIVES:
 * - Understand I2C communication with register-based sensors
 * - Learn to read multi-byte data (big-endian format)
 * - Practice sensor calibration and data conversion
 * - Implement real-time sensor monitoring with error handling
 * 
 * WHAT THIS EXAMPLE DOES:
 * - Reads soil moisture using capacitive sensing technology
 * - Reads temperature from an onboard sensor
 * - Displays live sensor data with visual indicators
 * - Shows proper I2C communication error handling
 * 
 * HARDWARE SETUP:
 * - Uses STEMMA Soil Sensor from Adafruit (ATSAMD10-based)
 * - Connect: VCC→3.3V, GND→GND, SDA→GP4, SCL→GP5
 * - Default I2C address: 0x36 (configurable to 0x39)
 * - Uses "seesaw" protocol for sensor communication
 * 
 * I2C CONCEPTS DEMONSTRATED:
 * - Multi-register sensor communication
 * - Big-endian data conversion
 * - Device detection and error recovery
 * - Register-based protocol implementation
 */

#include <Wire.h>
#include "Kywy.hpp"

// === SENSOR CONFIGURATION ===
const uint8_t SENSOR_I2C_ADDRESS = 0x36;  // Can be 0x36 or 0x39

// === SEESAW PROTOCOL CONSTANTS ===
// The sensor uses a protocol called "seesaw" with different modules
const uint8_t MODULE_STATUS = 0x00;    // Status and temperature module
const uint8_t MODULE_TOUCH = 0x0F;     // Capacitive touch sensing module

// Register addresses within each module
const uint8_t REG_TEMPERATURE = 0x04;         // Temperature register
const uint8_t REG_TOUCH_CHANNEL_BASE = 0x10;  // Base address for touch channels

// Hardware channel assignments
const uint8_t MOISTURE_CHANNEL = 0;  // Touch channel 0 is connected to moisture sensor

// === GLOBAL VARIABLES ===
Kywy::Engine engine;

// Sensor state tracking
bool sensorConnected = false;
uint16_t moistureReading = 0;    // Raw capacitive touch value
float temperatureC = 0.0;        // Temperature in Celsius
unsigned long lastUpdateTime = 0;

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
    KYWY_DISPLAY_WIDTH / 2, 8,
    "Soil Sensor Monitor",
    Display::TextOptions().origin(Display::Origin::Text::CENTER));
    
  // Show sensor type
  engine.display.drawText(
    KYWY_DISPLAY_WIDTH / 2, 20,
    "Capacitive I2C Sensor",
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
      // Sensor is connected - read current values
      readAllSensorData();
    } else {
      // Sensor not connected - check if it's been reconnected
      checkSensorConnection();
    }
    
    // Update the display with latest data
    updateDisplay();
    
    // Remember when we last updated
    lastUpdateTime = millis();
  }
  
  // Small delay to prevent overwhelming the processor
  delay(100);
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
  Wire.write(moduleAddress);     // Select the module (status, touch, etc.)
  Wire.write(registerAddress);   // Select the register within that module
  
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
                      moistureBytes[1];                     // Low byte second
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
    "Soil Sensor",
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
    // === SENSOR DATA DISPLAY ===
    
    // Connection status with I2C address
    char statusText[30];
    snprintf(statusText, sizeof(statusText), "Connected (0x%02X)", SENSOR_I2C_ADDRESS);
    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 22,
      statusText,
      Display::TextOptions().origin(Display::Origin::Text::CENTER));
    
    // Temperature reading
    char tempText[20];
    snprintf(tempText, sizeof(tempText), "Temp: %.1f°C", temperatureC);
    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 40,
      tempText,
      Display::TextOptions().origin(Display::Origin::Text::CENTER));
    
    // Raw capacitive reading (useful for calibration)
    char capacitiveText[25];
    snprintf(capacitiveText, sizeof(capacitiveText), "Capacitive: %d", moistureReading);
    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 55,
      capacitiveText,
      Display::TextOptions().origin(Display::Origin::Text::CENTER));
    
    // Moisture percentage
    uint8_t moisturePercent = convertToMoisturePercentage();
    char moistureText[20];
    snprintf(moistureText, sizeof(moistureText), "Moisture: %d%%", moisturePercent);
    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 70,
      moistureText,
      Display::TextOptions().origin(Display::Origin::Text::CENTER));
    
    // Moisture level description
    const char* moistureDescription = getMoistureDescription(moisturePercent);
    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 85,
      moistureDescription,
      Display::TextOptions().origin(Display::Origin::Text::CENTER));
    
    // Visual moisture level bar
    drawMoistureBar(moisturePercent);
    
    // Sensor information
    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 120,
      "I2C Capacitive Sensor",
      Display::TextOptions().origin(Display::Origin::Text::CENTER));
      
    // Runtime counter
    char runtimeText[15];
    snprintf(runtimeText, sizeof(runtimeText), "Runtime: %lus", millis() / 1000);
    engine.display.drawText(
      KYWY_DISPLAY_WIDTH / 2, 135,
      runtimeText,
      Display::TextOptions().origin(Display::Origin::Text::CENTER));
  }
  
  // Update the physical display
  engine.display.update();
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
  // === VISUAL MOISTURE LEVEL INDICATOR ===
  
  // Bar graph dimensions
  const int BAR_WIDTH = 100;
  const int BAR_HEIGHT = 8;
  const int BAR_X = (KYWY_DISPLAY_WIDTH - BAR_WIDTH) / 2;  // Center horizontally
  const int BAR_Y = 100;
  
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
}
