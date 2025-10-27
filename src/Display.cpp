// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later


#include "Display.hpp"

extern "C" {
#include "hardware/dma.h"
#include "hardware/spi.h"
#include "hardware/regs/dreq.h"
}

// DMA channel used for display transfers. Accessed from IRQ handler.
static volatile int display_dma_chan = -1;

// Our own SPI mutex - simple flag that can be safely accessed from IRQ
// mbed based mutexes are not safe to use from IRQ context.
static volatile bool spi_bus_locked = false;

// Flag to indicate display update is pending
static volatile bool displayPending = false;

extern "C" void display_dma_irq(void) {
  int chan = display_dma_chan;
  if (chan < 0) return;
  // clear interrupt for this channel
  dma_hw->ints0 = 1u << chan;
  dma_channel_set_irq0_enabled(chan, false);
  // disable peripheral DMA request
  spi0_hw->dmacr &= ~0x1u;
  // deassert CS
  digitalWrite(KYWY_DISPLAY_CS, LOW);
  // unclaim channel
  dma_channel_unclaim(chan);
  display_dma_chan = -1; // Mark channel as free
  
  // Release our SPI bus lock (safe from IRQ)
  spi_bus_locked = false;
}

namespace Display {

namespace Driver {

void MBED_SPI_DRIVER::initializeDisplay() {
  mbedSPI =
    new mbed::SPI((PinName)KYWY_DISPLAY_MOSI, (PinName)KYWY_DISPLAY_MISO,
                  (PinName)KYWY_DISPLAY_SCK);
  mbedSPI->format(8, 0);
  mbedSPI->frequency(2000000); //max 2MHz for display,  max per datasheet

  pinMode(KYWY_DISPLAY_CS, OUTPUT);
  pinMode(KYWY_DISPLAY_DISP, OUTPUT);

  digitalWrite(KYWY_DISPLAY_CS, LOW);

  clearBuffer();
  
  sendBufferToDisplay();

  digitalWrite(KYWY_DISPLAY_DISP, HIGH);

  setRotation(Rotation::DEFAULT);
}

void MBED_SPI_DRIVER::setRotation(Rotation rotation) {
  switch (rotation) {
    case Rotation::DEFAULT:
      break;
    case Rotation::CLOCKWISE_90:
      break;
    case Rotation::CLOCKWISE_180:
      break;
    case Rotation::CLOCKWISE_270:
      break;
  }
}

void MBED_SPI_DRIVER::clearBuffer() {
  memset(MBED_SPI_DRIVER_BUFFER, 0xff, sizeof(MBED_SPI_DRIVER_BUFFER));
}

void MBED_SPI_DRIVER::dmaTransferBuffer(uint8_t *buffer, size_t size) {
  // This function uses DMA to transfer a buffer to the display over SPI
  // without blocking the CPU.
  // Since default mbed SPI does not support DMA, we directly access the RP2040
  // hardware registers and DMA controller directly.
  // At highlevel, the transfer process is:
  // 1. Configure and start a DMA channel to transfer the buffer to SPI
  // 2. Enable SPI TX DMA requests
  // 3. Wait for DMA IRQ to signal completion and perform cleanup or same on timeout.

  if (!mbedSPI || display_dma_chan >= 0) {
    return;
  }

  // Claim a DMA channel and configure it to transfer from buffer -> SPI TX FIFO
  int dma_chan = dma_claim_unused_channel(true);
  dma_channel_config c = dma_channel_get_default_config(dma_chan);
  channel_config_set_read_increment(&c, true);   // read from incrementing memory
  channel_config_set_write_increment(&c, false); // write to fixed peripheral FIFO
  channel_config_set_dreq(&c, DREQ_SPI0_TX);
  channel_config_set_transfer_data_size(&c, DMA_SIZE_8);

  // Enable SPI TX DMA request in the peripheral (TXDMAE)
  spi0_hw->dmacr |= 0x1; // TXDMAE = bit 0

  // Configure and start the DMA: destination is SPI0->dr (data register)
  dma_channel_configure(dma_chan, &c,
                        &spi0_hw->dr, // destination (peripheral FIFO)
                        buffer,         // source (our buffer)
                        (uint)size, // transfer count in bytes
                        true);         // start immediately

  // Install IRQ handler to finish the transfer so we don't block the CPU here.
  // Use DMA IRQ0 and enable IRQ for this channel.
  display_dma_chan = dma_chan;
  // acknowledge/clear any existing interrupt and enable
  dma_hw->ints0 = 1u << dma_chan;
  dma_channel_set_irq0_enabled(dma_chan, true);
  irq_set_exclusive_handler(DMA_IRQ_0, display_dma_irq);
  irq_set_enabled(DMA_IRQ_0, true);
}

void MBED_SPI_DRIVER::sendBufferToDisplay() {
  // Check if SPI bus is already locked by a DMA transfer
  if(!mbedSPI || spi_bus_locked) {
    // SPI bus busy, drop frame
    return;
  }

  // This function uses DMA to transfer the display buffer to the display
  // over SPI without blocking the CPU. 
  // Since default mbed SPI does not support DMA, we directly access the RP2040
  // hardware registers and DMA controller directly.
  // At highlevel, the transfer process is:
  // 1. Build a contiguous TX buffer with header, line data, and tail
  // 2. Configure and start a DMA channel to transfer the TX buffer to SPI
  // 3. Enable SPI TX DMA requests
  // 4. Wait for DMA IRQ to signal completion and perform cleanup or same on timeout.

  // Build contiguous TX buffer: 1 byte header + 168 lines * 20 bytes + 1 byte tail
  const size_t LINES = KYWY_DISPLAY_HEIGHT + 1;
  const size_t LINE_BYTES = KYWY_DISPLAY_WIDTH / 8 + 2; // 18 data + 2 (line addr + trailing 0)
  const size_t TX_SIZE = 1 + (LINES * LINE_BYTES) + 1; // Total size

  // Ensure the TX buffer is 32-bit aligned for DMA peripheral efficiency and to
  // avoid misaligned reads which can corrupt the final bytes on some DMA
  // controllers.
  static uint8_t txbuf[TX_SIZE] __attribute__((aligned(4)));

  // Header
  txbuf[0] = vcom | writeCommand;
  vcom = vcom ? 0x00 : vcomCommand;  // toggle vcom

  // Fill lines
  for (size_t line = 0; line < LINES; ++line) {
    size_t base = 1 + line * LINE_BYTES;
    txbuf[base + 0] = reverse(line + 1);
    memcpy((void *)(txbuf + base + 1), (const void *)(MBED_SPI_DRIVER_BUFFER + 18 * line), 18);
    txbuf[base + 19] = 0x00;
  }

    // Tail
  txbuf[TX_SIZE - 1] = 0x00;

  // Lock our SPI bus (simple flag, safe for IRQ to unlock)
  spi_bus_locked = true;
  
  // Assert CS and start DMA transfer
  digitalWrite(KYWY_DISPLAY_CS, HIGH);
  dmaTransferBuffer(txbuf, TX_SIZE);

  // Transfer happens asynchronously via DMA
  // IRQ will unlock spi_bus_locked when complete
  // No need to wait - that's the whole point of non-blocking!
  
  return;
}

void MBED_SPI_DRIVER::setBufferPixel(int16_t x, int16_t y, uint16_t color) {
  if (x < 0 || x >= 144 || y < 0 || y >= 168) {
    return;
  }

  int index = (18 * y) + (x / 8);
  int bit = x % 8;

  if (color) {
    MBED_SPI_DRIVER_BUFFER[index] =
      MBED_SPI_DRIVER_BUFFER[index] | (1 << (7 - bit));
  } else {
    MBED_SPI_DRIVER_BUFFER[index] =
      MBED_SPI_DRIVER_BUFFER[index] & (0xff ^ (1 << (7 - bit)));
  }
}

bool Driver::cropBlock(int16_t &x, int16_t &y, uint16_t &width,
                       uint16_t &height) {
  if (x > (getWidth() - 1) || y > (getHeight() - 1) || x + width - 1 < 0 || y + height - 1 < 0)
    return false;

  if (x < 0) {
    width += x;
    x = 0;
  }

  if ((x + width - 1) > (getWidth() - 1)) {
    width = getWidth() - x;
  }

  if (y < 0) {
    height += y;
    y = 0;
  }

  if ((y + height - 1) > (getHeight() - 1)) {
    height = getHeight() - y;
  }

  return true;
}

void MBED_SPI_DRIVER::writeBitmapOrBlockToBuffer(
  int16_t x, int16_t y, uint16_t width, uint16_t height, uint8_t *bitmap,
  BitmapOptions options, bool block, uint16_t blockColor) {

  // we can write from an arbitrary chunk of the bitmap to an arbitrary chunk of
  // the screen buffer
  uint16_t bitmapX = 0, bitmapY = 0, bitmapWidth = width;

  if (x < 0)
    bitmapX += -1 * x;  // left edge of bitmap is off screen

  if (y < 0)
    bitmapY += -1 * y;  // top edge of bitmap is off screen

  if (!cropBlock(x, y, width, height))
    return;  // no overlap between bitmap and screen

  // get top left corner of block to write on screen
  uint8_t *buffer = MBED_SPI_DRIVER_BUFFER + (18 * y) + (x / 8);

  // index bitmap by bits instead of bytes to handle all the byte splitting
  uint16_t bitmapBitIndex = bitmapWidth * bitmapY + bitmapX;

  // precomputed values
  uint8_t bufferBitsNotToWriteToInLeftByteColumn = x % 8;
  uint8_t bufferBitsToWriteToInLeftByteColumn = 8 - x % 8;

  // buffer wrap distance calculation
  int splitLeftBits =
    8 - x % 8;  // how many bits of the left most byte column need to be filled
  splitLeftBits =
    splitLeftBits == 8
      ? 0
      : splitLeftBits;  // if we have a whole column on the left just include
                        // it as part of the inner bytes
  int splitRightBits =
    (x + width) % 8;  // how many bits of the right most byte column need to be filled
  uint16_t innerBytes =
    (width - splitLeftBits - splitRightBits) / 8;  // how many bytes are between the right and left column
  uint16_t bufferWrapDistance =
    18 - innerBytes - (splitLeftBits ? 1 : 0) - (splitRightBits ? 1 : 0);

  // iterate over each line
  for (int16_t j = 0; j < height; j++) {
    uint16_t bitsLeftToWrite = width;

    while (bitsLeftToWrite) {
      uint8_t byteToWrite;
      uint8_t bitsWritten = 0;
      uint8_t mask =
        0x00;  // identifies the part of the byte column we want to write

      if (block) {
        byteToWrite = blockColor;
      } else {
        byteToWrite =
          ((*(bitmap + (bitmapBitIndex / 8)) << (bitmapBitIndex % 8)) | (*(bitmap + (bitmapBitIndex / 8) + 1) >> (8 - bitmapBitIndex % 8)));
      }

      if (options.getNegative()) {
        byteToWrite = ~byteToWrite;
      }

      // we're only writing a single partial column and need to mask both sides
      // of the byteToWrite
      if (bufferBitsToWriteToInLeftByteColumn > width) {
        byteToWrite =
          byteToWrite >> bufferBitsNotToWriteToInLeftByteColumn;  // shift starting bitmap bit
                                                                  // to match starting bit of
                                                                  // buffer byte column

        mask =
          0xff >> bufferBitsNotToWriteToInLeftByteColumn;  // mask of left side since
                                                           // this is a partial column
        mask &=
          0xff
          << (8 - (bufferBitsNotToWriteToInLeftByteColumn + width));  // mask off right side since this is a partial column

        bitsWritten = width;

        // we're on the leftmost column
      } else if (bitsLeftToWrite == width) {
        byteToWrite =
          byteToWrite >> bufferBitsNotToWriteToInLeftByteColumn;  // shift starting bitmap bit
                                                                  // to match starting bit of
                                                                  // buffer byte column

        mask = 0xff >> bufferBitsNotToWriteToInLeftByteColumn;  // mask off left side

        bitsWritten = bufferBitsToWriteToInLeftByteColumn;

        // we're writing an inner column
      } else if (bitsLeftToWrite >= 8) {
        mask = 0xff;  // don't mask off anything

        bitsWritten = 8;

        // we're writing the rightmost column
      } else if ((bitsLeftToWrite > 0) & (bitsLeftToWrite < 8)) {
        mask = 0xff << (8 - bitsLeftToWrite);  // mask off right side

        bitsWritten = bitsLeftToWrite;
      }

      // actually do the writing
      if (!options.getOpaque()) {
        if (options.getColor()) {
          *buffer |= (~byteToWrite) & mask;
        } else {
          *buffer &= byteToWrite | (~mask);
        }
      } else {
        if (options.getColor()) {
          *buffer = (*buffer & ~mask) | (~byteToWrite & mask);
        } else {
          *buffer = (*buffer & ~mask) | (byteToWrite & mask);
        }
      }

      // advance our tracking variables
      buffer += 1;
      bitmapBitIndex += bitsWritten;
      bitsLeftToWrite -= bitsWritten;
    }

    // advance to the next line on the bitmap and buffer
    bitmapBitIndex += bitmapWidth - width;
    buffer += bufferWrapDistance;
  }
}

void MBED_SPI_DRIVER::setBufferBlock(int16_t x, int16_t y, uint16_t width,
                                     uint16_t height, uint16_t color) {
  writeBitmapOrBlockToBuffer(x, y, width, height, nullptr,
                             BitmapOptions().opaque(true), true, color);
}

void MBED_SPI_DRIVER::writeBitmapToBuffer(int16_t x, int16_t y, uint16_t width,
                                          uint16_t height, uint8_t *bitmap,
                                          BitmapOptions options) {
  writeBitmapOrBlockToBuffer(x, y, width, height, bitmap, options, false, 0x00);
}

}  // namespace Driver

void Display::setup() {
  driver->initializeDisplay();
}
void Display::clear() {
  driver->clearBuffer();
}

void Display::update() {
  displayPending = true;
}

void Display::checkPendingUpdate() {
  if (displayPending && !spi_bus_locked) {
    displayPending = false;
    driver->sendBufferToDisplay();
  }
}

void Display::setRotation(Rotation rotation) {
  driver->setRotation(rotation);
}
void Display::drawPixel(int16_t x, int16_t y, uint16_t color) {
  driver->setBufferPixel(x, y, color);
}

void Display::shiftOrigin2DToTopLeft(Origin::Object2D origin, int16_t &x,
                                     int16_t &y, uint16_t width,
                                     uint16_t height) {
  switch (origin) {
    case Origin::Object2D::TOP_LEFT:
      break;
    case Origin::Object2D::TOP_RIGHT:
      x -= width - 1;
      break;
    case Origin::Object2D::BOTTOM_LEFT:
      y -= height - 1;
      break;
    case Origin::Object2D::BOTTOM_RIGHT:
      x -= width - 1;
      y -= height - 1;
      break;
    case Origin::Object2D::CENTER:
      // similarly to circles, where there is no pixel center we bias to the
      // bottom right so that the left bound is at `x
      // - (width / 2)` and the upper bound is at `y - (height / 2)`.
      x -= width / 2;
      y -= height / 2;
      break;
  }
}

void Display::drawLine(int16_t xStart, int16_t yStart, int16_t xEnd,
                       int16_t yEnd, Object1DOptions options) {
  if (yStart == yEnd) {   // horizontal line
    if (xEnd < xStart) {  // setBufferBlock draws left-to-right so make sure xEnd
                          // is >= xStart
      std::swap(xEnd, xStart);
      std::swap(yEnd, yStart);
    }
    driver->setBufferBlock(xStart, yStart, xEnd - xStart + 1, 1,
                           options.getColor());
    return;
  }

  if (xStart == xEnd) {   // vertical line
    if (yEnd < yStart) {  // setBufferBlock draws top-to-bottom so make sure yEnd
                          // is >= yStart
      std::swap(xEnd, xStart);
      std::swap(yEnd, yStart);
    }
    driver->setBufferBlock(xStart, yStart, 1, yEnd - yStart + 1,
                           options.getColor());
    return;
  }

  // for sloped lines use Bresenham's Algorithm with integer arithmetic
  // <https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm>
  int16_t dX = abs(xEnd - xStart), xStep = xStart < xEnd ? 1 : -1;
  int16_t dY = -1 * abs(yEnd - yStart), yStep = yStart < yEnd ? 1 : -1;
  int16_t xHead = xStart, yHead = yStart;
  int16_t error = dX + dY;

  while (true) {
    driver->setBufferPixel(xHead, yHead, options.getColor());

    if (xHead == xEnd && yHead == yEnd)
      break;

    if (2 * error >= dY) {
      if (xHead == xEnd)
        break;

      error += dY;
      xHead += xStep;
    }

    if (2 * error <= dX) {
      if (yHead == yEnd)
        break;

      error += dX;
      yHead += yStep;
    }
  }
}

void Display::drawLine(int16_t x, int16_t y, double length, double angle,
                       Object1DOptions options) {
  int16_t xStart = 0, yStart = 0, xEnd = 0, yEnd = 0;

  // multiply y deltas by -1 since our y-axis is inverted compared to standard
  // cartesian coordinates
  switch (options.getOrigin()) {
    case Origin::Object1D::ENDPOINT:
      xStart = x;
      yStart = y;
      xEnd = round(xStart + length * cos(angle));
      yEnd = round(yStart + -1 * length * sin(angle));
      break;
    case Origin::Object1D::MIDPOINT:
      xStart = round(x - 0.5 * length * cos(angle));
      yStart = round(y - -1 * 0.5 * length * sin(angle));
      xEnd = round(x + 0.5 * length * cos(angle));
      yEnd = round(y + -1 * 0.5 * length * sin(angle));
      break;
  }

  drawLine(xStart, yStart, xEnd, yEnd, options);
};

void Display::drawRectangle(int16_t x, int16_t y, uint16_t width,
                            uint16_t height, Object2DOptions options) {
  shiftOrigin2DToTopLeft(options.getOrigin(), x, y, width, height);
  driver->setBufferBlock(x, y, width, 1, options.getColor());  // top line
  driver->setBufferBlock(x, y + height - 1, width, 1,
                         options.getColor());                   // bottom line
  driver->setBufferBlock(x, y, 1, height, options.getColor());  // left line
  driver->setBufferBlock(x + width - 1, y, 1, height,
                         options.getColor());  // right line
};

void Display::fillRectangle(int16_t x, int16_t y, uint16_t width,
                            uint16_t height, Object2DOptions options) {
  shiftOrigin2DToTopLeft(options.getOrigin(), x, y, width, height);
  driver->setBufferBlock(x, y, width, height, options.getColor());
};

void Display::drawBitmap(int16_t x, int16_t y, uint16_t width, uint16_t height,
                         uint8_t *bitmap, BitmapOptions options) {
  shiftOrigin2DToTopLeft(options.getOrigin(), x, y, width, height);
  driver->writeBitmapToBuffer(x, y, width, height, bitmap, options);
};

}  // namespace Display
