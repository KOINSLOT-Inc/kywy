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
static volatile int dmaChan1 = -1;  // For write only and duplex
static volatile int dmaChan2 = -1;  // For duplex transfers

// Callback to invoke when DMA transfer completes
static void (*currentCompletionCallback)() = nullptr;

// CS pin and polarity for current transfer
static int currentCSPin = -1;
static bool currentCSActiveHigh = false;

// DMA IRQ handler - called when DMA transfer completes
// This runs after the DMA transfer is complete to clean up and release the SPI bus.
// This is an IRQ handler, so must be fast and safe.
extern "C" void spi_bus_dma_irq_handler(void) {
  // Read pending interrupts for DMA channels
  uint32_t pending = dma_hw->ints0;

  bool handledAny = false;

  // If dmaChan1 is active and its interrupt is pending, handle it
  if (dmaChan1 >= 0 && (pending & (1u << dmaChan1))) {
    // Clear interrupt for this channel
    dma_hw->ints0 = 1u << dmaChan1;

    // Disable IRQ for this channel
    dma_channel_set_irq0_enabled(dmaChan1, false);

    // Mark that we handled something
    handledAny = true;
    // Unclaim channel
    dma_channel_unclaim(dmaChan1);
    dmaChan1 = -1;
  }

  // If dmaChan2 is active and its interrupt is pending, handle it
  if (dmaChan2 >= 0 && (pending & (1u << dmaChan2))) {
    // Clear interrupt for this channel
    dma_hw->ints0 = 1u << dmaChan2;

    // Disable IRQ for this channel
    dma_channel_set_irq0_enabled(dmaChan2, false);

    handledAny = true;
    // Unclaim channel
    dma_channel_unclaim(dmaChan2);
    dmaChan2 = -1;
  }

  // If nothing to handle, just return
  if (!handledAny) {
    return;
  }

  // Disable peripheral DMA request (TX) - safe even if duplex
  spi0_hw->dmacr &= ~0x1u;

  // Wait for SPI to finish transmitting (FIFO empty and not busy)
  // BSY bit (bit 4) is set while SPI is transmitting
  while ((spi0_hw->sr & (1 << 4)) != 0) {
    // Busy wait
  }

  // Deassert CS pin (only once transfers are complete)
  if (currentCSPin >= 0) {
    digitalWrite(currentCSPin, currentCSActiveHigh ? LOW : HIGH);
    currentCSPin = -1;
  }

  // Release the bus lock
  busLocked = false;

  // Invoke completion callback if set
  if (currentCompletionCallback) {
    // Capture and clear before calling to avoid reentrancy issues
    void (*cb)() = currentCompletionCallback;
    currentCompletionCallback = nullptr;
    cb();
  }
}

void resetSDCard() {
  if (!mbedSPI) {
    return;  // SPI not initialized yet
  }

  // Put SD card into proper SPI mode by sending CMD0 (GO_IDLE_STATE)

  // Very handy source: https://elm-chan.org/docs/mmc/mmc_e.html

  // Send 80+ dummy clocks with CS high (per SD spec requirement)
  for (int i = 0; i < 12; i++) {
    mbedSPI->write(0xFF);
  }
  
  // Send CMD0 to reset SD card into SPI mode
  digitalWrite(KYWY_EXP1_CS, LOW);
  delayMicroseconds(10);
  mbedSPI->write(0x40);  // CMD0
  mbedSPI->write(0x00);
  mbedSPI->write(0x00);
  mbedSPI->write(0x00);
  mbedSPI->write(0x00);
  mbedSPI->write(0x95);  // CRC for CMD0
  mbedSPI->write(0xFF);  // Throwaway bit
  
  // Wait for and read R1 response or timeout
  for (int i = 0; i < 20; i++) { // timeout at 20 tries (possibly no sdcard)
    uint8_t response = mbedSPI->write(0xFF);
    if (response != 0xFF) break;
  }
  
  digitalWrite(KYWY_EXP1_CS, HIGH);  // Disable SD card CS
  
  // Send additional clocks to complete initialization
  for (int i = 0; i < 10; i++) {
    mbedSPI->write(0xFF);
  }
}

void initialize() {
  // Create mbed::SPI object to initialize SPI hardware
  // Keep this object private to SPIBus - it should only be accessed internally
  mbedSPI = new mbed::SPI((PinName)KYWY_MOSI, (PinName)KYWY_MISO, (PinName)KYWY_SCK);
  mbedSPI->format(8, 0);        // 8-bit, mode 0 (will be reconfigured per transfer)
  mbedSPI->frequency(2000000);  // 2MHz default (will be reconfigured per transfer)

  // Configure MISO with pull-down to reduce crosstalk from SD card
  pinMode(KYWY_MISO, INPUT_PULLDOWN);

  // Assume things are plugged in and we need to deselect them to prevent bus conflicts
  // Assume default EXP devices are active high (eg SD card, common convention)
  pinMode(KYWY_DISPLAY_CS, OUTPUT);
  pinMode(KYWY_EXP1_CS, OUTPUT);
  pinMode(KYWY_EXP2_CS, OUTPUT);

  digitalWrite(KYWY_DISPLAY_CS, LOW);   // Active low
  digitalWrite(KYWY_EXP1_CS, HIGH);     // Active high
  digitalWrite(KYWY_EXP2_CS, HIGH);     // Active high

  // Initialize SD card to prevent display artifacts
  resetSDCard();
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

  // Configure SPI frequency for this transfer
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

// Start a full-duplex DMA transfer: write from txBuffer, read to rxBuffer
// Returns true if transfer started, false if bus was busy
// txBuffer: pointer to data to send
// rxBuffer: pointer to buffer to receive data
// size: number of bytes to transfer
// csPin: chip select pin number
// csActiveHigh: true if CS is active high, false if active low
// frequency: SPI clock frequency in Hz
// completionCallback: optional callback invoked when transfer completes (from IRQ context)
bool startDuplexDMATransfer(uint8_t *txBuffer, uint8_t *rxBuffer, size_t size, int csPin, bool csActiveHigh,
                            uint32_t frequency, void (*completionCallback)()) {
  if (!mbedSPI) {
    return false;  // Not initialized
  }
  uint32_t interrupts = save_and_disable_interrupts();
  if (busLocked || dmaChan1 >= 0 || dmaChan2 >= 0) {
    restore_interrupts(interrupts);
    return false;  // Bus is busy
  }
  busLocked = true;
  restore_interrupts(interrupts);

  mbedSPI->frequency(frequency);
  currentCSPin = csPin;
  currentCSActiveHigh = csActiveHigh;
  currentCompletionCallback = completionCallback;

  if (csPin >= 0) digitalWrite(csPin, csActiveHigh ? HIGH : LOW);

  // Claim two DMA channels: one for TX, one for RX
  int txChan = dma_claim_unused_channel(true);
  int rxChan = dma_claim_unused_channel(true);
  dmaChan1 = txChan;
  dmaChan2 = rxChan;

  // RX DMA config: SPI RX FIFO -> rxBuffer
  dma_channel_config rxConf = dma_channel_get_default_config(rxChan);
  channel_config_set_read_increment(&rxConf, false);
  channel_config_set_write_increment(&rxConf, true);
  channel_config_set_dreq(&rxConf, DREQ_SPI0_RX);
  channel_config_set_transfer_data_size(&rxConf, DMA_SIZE_8);
  dma_channel_configure(rxChan, &rxConf,
                        rxBuffer,      // dest
                        &spi0_hw->dr,  // src (SPI RX FIFO)
                        (uint)size,    // count
                        false);        // don't start yet

  // TX DMA config: txBuffer -> SPI TX FIFO
  dma_channel_config txConf = dma_channel_get_default_config(txChan);
  channel_config_set_read_increment(&txConf, true);
  channel_config_set_write_increment(&txConf, false);
  channel_config_set_dreq(&txConf, DREQ_SPI0_TX);
  channel_config_set_transfer_data_size(&txConf, DMA_SIZE_8);
  spi0_hw->dmacr |= 0x1;
  dma_channel_configure(txChan, &txConf,
                        &spi0_hw->dr,  // dest (SPI TX FIFO)
                        txBuffer,      // src
                        (uint)size,    // count
                        false);        // don't start yet

  // Enable IRQs for both channels
  dma_hw->ints0 = (1u << txChan) | (1u << rxChan);
  dma_channel_set_irq0_enabled(txChan, true);
  dma_channel_set_irq0_enabled(rxChan, true);
  irq_set_exclusive_handler(DMA_IRQ_0, spi_bus_dma_irq_handler);
  irq_set_enabled(DMA_IRQ_0, true);

  // Start RX first, then TX (order matters for SPI)
  dma_channel_start(rxChan);
  dma_channel_start(txChan);

  return true;
}

// Send a command buffer (TX only, RX ignored)
bool sendCommandDMATransfer(uint8_t *cmdBuffer, size_t cmdSize, int csPin, bool csActiveHigh,
                            uint32_t frequency, void (*completionCallback)()) {
  // Just use startDMATransfer (TX only)
  return startDMATransfer(cmdBuffer, cmdSize, csPin, csActiveHigh, frequency, completionCallback);
}

// Receive data (RX only, TX sends dummy 0xFF)
bool receiveDataDMATransfer(uint8_t *rxBuffer, size_t rxSize, int csPin, bool csActiveHigh,
                            uint32_t frequency, void (*completionCallback)()) {
  // Allocate a static dummy TX buffer (0xFF)
  static uint8_t dummyTx[512];
  if (rxSize > sizeof(dummyTx)) return false;
  memset(dummyTx, 0xFF, rxSize);
  return startDuplexDMATransfer(dummyTx, rxBuffer, rxSize, csPin, csActiveHigh, frequency, completionCallback);
}

}  // namespace SPIBus
