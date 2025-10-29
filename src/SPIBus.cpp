// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SPIBus.hpp"
#include <Arduino.h>
#include <mbed.h>

extern "C" {
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "hardware/regs/dreq.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
}

namespace SPIBus {

// mbed::SPI object for SPI initialization
static mbed::SPI *mbedSPI = nullptr;

// Our own SPI mutex - simple flag that can be safely accessed from IRQ
// mbed based mutexes are not safe to use from IRQ context.
static volatile bool busLocked = false;

// DMA channel used for transfers. Default to no channel claimed (-1)
static volatile int dmaChan1 = -1; // For write only and duplex
static volatile int dmaChan2 = -1; // For duplex transfers

// Callback to invoke when DMA transfer completes
static void (*currentCompletionCallback)() = nullptr;

// CS pin and polarity for current transfer
static int currentCSPin = -1;
static bool currentCSActiveHigh = false;

// DMA IRQ handler - called when DMA transfer completes
// This runs after the DMA transfer is complete to clean up and release the SPI bus.
// This is an IRQ handler, so must be fast and safe.
extern "C" void spi_bus_dma_irq_handler(void) {
  // Clear interrupt for this channel
  dma_hw->ints0 = 1u << dmaChan1;

  // Disable IRQ for this channel
  dma_channel_set_irq0_enabled(dmaChan1, false);

  // Disable peripheral DMA request
  spi0_hw->dmacr &= ~0x1u;

  // Wait for SPI to finish transmitting (FIFO empty and not busy)
  // BSY bit (bit 4) is set while SPI is transmitting
  while ((spi0_hw->sr & (1 << 4)) != 0) {
    // Busy wait
  }

  // Deassert CS pin
  if (currentCSPin >= 0) {
    digitalWrite(currentCSPin, currentCSActiveHigh ? LOW : HIGH);
    currentCSPin = -1;
  }

  // Unclaim channel
  dma_channel_unclaim(dmaChan1);

  // Mark channel as free
  dmaChan1 = -1;

  // Release the bus lock
  busLocked = false;

  // Invoke completion callback if set
  if (currentCompletionCallback) {
    currentCompletionCallback();
    currentCompletionCallback = nullptr;
  }
}

void initialize() {
  // Create mbed::SPI object to initialize SPI hardware
  // Keep this object private to SPIBus - it should only be accessed internally
  mbedSPI = new mbed::SPI((PinName)KYWY_MOSI, (PinName)KYWY_MISO, (PinName)KYWY_SCK);
  mbedSPI->format(8, 0);        // 8-bit, mode 0 (will be reconfigured per transfer)
  mbedSPI->frequency(2000000);  // 2MHz default (will be reconfigured per transfer)

  // Asume things are plugged in and we need to deselect them to prevent bus conflicts
  // Assume default EXP devices are active high (eg SD card, common convention)
  pinMode(KYWY_DISPLAY_CS, OUTPUT);
  pinMode(KYWY_SDCARD_CS, OUTPUT);
  pinMode(KYWY_EXP1_CS, OUTPUT);
  pinMode(KYWY_EXP2_CS, OUTPUT);
  
  digitalWrite(KYWY_DISPLAY_CS, LOW);
  digitalWrite(KYWY_SDCARD_CS, HIGH);
  digitalWrite(KYWY_EXP1_CS, HIGH);
  digitalWrite(KYWY_EXP2_CS, HIGH);
}

bool isBusLocked() {
  return busLocked;
}

bool startDMATransfer(uint8_t *buffer, size_t size, int csPin, bool csActiveHigh,
                      uint32_t frequency, void (*completionCallback)()) {
  // Check if SPI is initialized
  if (!mbedSPI) {
    return false;  // Not initialized
  }

  // Atomically acquire the bus lock using critical section
  // This is truly atomic and safe in IRQ context
  uint32_t interrupts = save_and_disable_interrupts();

  // Check if bus is already locked
  if (busLocked || dmaChan1 >= 0) {
    restore_interrupts(interrupts);
    return false;  // Bus is busy
  }

  // Lock the bus
  busLocked = true;

  // Restore interrupts - we now own the lock
  restore_interrupts(interrupts);

  // Configure SPI frequency for this transfer using mbed::SPI
  // This must be done AFTER acquiring the lock to prevent race conditions
  mbedSPI->frequency(frequency);

  // Store CS pin info and callback
  currentCSPin = csPin;
  currentCSActiveHigh = csActiveHigh;
  currentCompletionCallback = completionCallback;

  // Assert CS pin before starting transfer
  if (csPin >= 0) {
    digitalWrite(csPin, csActiveHigh ? HIGH : LOW);
  }

  // Claim a DMA channel and configure it to transfer from buffer -> SPI TX FIFO
  int dma_chan = dma_claim_unused_channel(true);
  dma_channel_config c = dma_channel_get_default_config(dma_chan);
  channel_config_set_read_increment(&c, true);    // Read from incrementing memory
  channel_config_set_write_increment(&c, false);  // Write to fixed peripheral FIFO
  channel_config_set_dreq(&c, DREQ_SPI0_TX);
  channel_config_set_transfer_data_size(&c, DMA_SIZE_8);

  // Enable SPI TX DMA request in the peripheral (TXDMAE)
  spi0_hw->dmacr |= 0x1;  // TXDMAE = bit 0

  // Configure and start the DMA: destination is SPI0->dr (data register)
  dma_channel_configure(dma_chan, &c,
                        &spi0_hw->dr,  // Destination (peripheral FIFO)
                        buffer,        // Source (our buffer)
                        (uint)size,    // Transfer count in bytes
                        true);         // Start immediately

  // Install IRQ handler to finish the transfer
  // Use DMA IRQ0 and enable IRQ for this channel
  dmaChan1 = dma_chan;

  // Acknowledge/clear any existing interrupt and enable
  dma_hw->ints0 = 1u << dma_chan;
  dma_channel_set_irq0_enabled(dma_chan, true);
  irq_set_exclusive_handler(DMA_IRQ_0, spi_bus_dma_irq_handler);
  irq_set_enabled(DMA_IRQ_0, true);

  return true;
}

}  // namespace SPIBus
