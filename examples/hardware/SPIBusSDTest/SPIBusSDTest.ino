// SPDX-License-Identifier: GPL-3.0-or-later
// Simple test for SPIBus duplex DMA with SD card on GPIO 15 (CS)
// Assumes MOSI/MISO/SCK are already set via configureBusPins
#include "SPIBus.hpp"
#include <Arduino.h>


#include "Display.hpp"

#define SD_CS_PIN 15
#define SPI_FREQ 1000000  // 1 MHz for SD card init
#define DISP_FREQ 2000000 // 2 MHz for display

uint8_t txBuf[8] = {0x40, 0x00, 0x00, 0x00, 0x00, 0x95, 0xFF, 0xFF}; // CMD0 (GO_IDLE_STATE)
uint8_t rxBuf[8] = {0};

volatile bool done = false;

void dmaDone() {
  done = true;
}


void setup() {
  Serial.begin(115200);
  // Use the display's SPI pins for both devices
  SPIBus::configureBusPins(KYWY_DISPLAY_MOSI, KYWY_DISPLAY_MISO, KYWY_DISPLAY_SCK);
  pinMode(SD_CS_PIN, OUTPUT);
  digitalWrite(SD_CS_PIN, HIGH);
  delay(10);

  // --- Display test ---
  Serial.println("Updating display via DMA...");
  pinMode(KYWY_DISPLAY_CS, OUTPUT);
  pinMode(KYWY_DISPLAY_DISP, OUTPUT);
  digitalWrite(KYWY_DISPLAY_CS, LOW);
  digitalWrite(KYWY_DISPLAY_DISP, HIGH);

  // Draw something simple
  Display::DISPLAY_DRIVER display;
  display.clearBuffer();
  for (int y = 0; y < 20; ++y) {
    display.setBufferPixel(y, y, 0); // Diagonal line
  }
  display.sendBufferToDisplay();
  delay(100); // Wait for DMA to finish (in real code, use callback)

  // --- SD card test ---
  Serial.println("Starting SD card SPI DMA test...");
  done = false;
  bool ok = SPIBus::startDuplexDMATransfer(
    txBuf, rxBuf, sizeof(txBuf), sizeof(rxBuf),
    SD_CS_PIN, false, SPI_FREQ, dmaDone,
    nullptr, 0
  );
  if (!ok) {
    Serial.println("DMA start failed");
    return;
  }
  while (!done) {}
  Serial.print("SD card response: ");
  for (int i = 0; i < sizeof(rxBuf); ++i) {
    Serial.print(rxBuf[i], HEX); Serial.print(" ");
  }
  Serial.println();
}

void loop() {
  // Nothing
}
