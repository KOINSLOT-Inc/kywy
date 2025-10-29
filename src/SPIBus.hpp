// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SPIBUS_HPP
#define SPIBUS_HPP

#include <stdint.h>
#include <stddef.h>

namespace SPIBus {

// Initialize the SPI bus with mbed::SPI
// Parameters:
//   mosiPin: MOSI pin number
//   misoPin: MISO pin number
//   sckPin: SCK pin number
void initialize(int mosiPin, int misoPin, int sckPin);

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

}  // namespace SPIBus

#endif  // SPIBUS_HPP
