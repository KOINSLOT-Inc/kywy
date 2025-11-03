// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SPIBUS_HPP
#define SPIBUS_HPP

#include <stdint.h>
#include <stddef.h>

#define KYWY_SCK 18
#define KYWY_MOSI 19
#define KYWY_MISO 16

#define KYWY_DISPLAY_CS 17
#define KYWY_SDCARD_CS 15
#define KYWY_EXP1_CS 15  // expansion port 1 CS (black pins on back) shared with sdcard, cant use pins and card at same time
#define KYWY_EXP2_CS 14  // expansion port 2 CS (black pins on back)

namespace SPIBus {

// Initialize the SPI bus with mbed::SPI
// Parameters:
//   mosiPin: MOSI pin number
//   misoPin: MISO pin number
//   sckPin: SCK pin number
void initialize();

// Check if the SPI bus is currently locked by a DMA transfer
bool isBusLocked();

// Start a DMA transfer of a buffer over SPI
// Returns true if transfer started, false if bus was busy
// Parameters:
//   buffer: pointer to data buffer to transmit
//   size: number of bytes to transmit
//   csPin: chip select pin number
//   csActiveHigh: true if CS is active high, false if active low
//   frequency: SPI clock frequency in Hz
//   completionCallback: optional callback invoked when transfer completes (from IRQ context)
bool startDMATransfer(uint8_t *buffer, size_t size, int csPin, bool csActiveHigh,
                      uint32_t frequency, void (*completionCallback)() = nullptr);

bool startDuplexDMATransfer(uint8_t *txBuffer, uint8_t *rxBuffer, size_t size, int csPin, bool csActiveHigh,
                            uint32_t frequency, void (*completionCallback)() = nullptr);

bool sendCommandDMATransfer(uint8_t *cmdBuffer, size_t cmdSize, int csPin, bool csActiveHigh,
                            uint32_t frequency, void (*completionCallback)());

}  // namespace SPIBus

#endif  // SPIBUS_HPP
