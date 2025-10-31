// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Display.hpp"
#include "SPIBus.hpp"
#include "mbed.h"

// Global event queue for display operations
static events::EventQueue displayEventQueue;
static rtos::Thread displayThread;

// Global reference to the driver for callback access
static Display::Driver::Driver *globalDisplayDriver = nullptr;

// Global counters for tracking display performance
volatile int displayTotalCallbackSends = 0;
volatile int displayTotalSuccessfulSends = 0;

// Function to process display updates in event queue context
void processDisplayUpdate() {
  if (globalDisplayDriver) {
    displayTotalCallbackSends++;
    bool success = globalDisplayDriver->sendBufferToDisplay();
    if (success) {
      displayTotalSuccessfulSends++;
    }
  }
}

// Callback invoked when display DMA transfer completes
void displayDMAComplete() {
  // Post display check to event queue to be processed outside interrupt context
  displayEventQueue.call(processDisplayUpdate);
}

namespace Display {

namespace Driver {

// Static buffer definitions for KYWY_DISPLAY_DRIVER
uint8_t KYWY_DISPLAY_DRIVER::KYWY_DISPLAY_ACTIVE_BUFFER[KYWY_DISPLAY_DRIVER::KYWY_DISPLAY_BUFFER_SIZE];
uint8_t KYWY_DISPLAY_DRIVER::KYWY_DISPLAY_TRANSFER_BUFFER[KYWY_DISPLAY_DRIVER::KYWY_DISPLAY_BUFFER_SIZE];
uint8_t KYWY_DISPLAY_DRIVER::KYWY_DISPLAY_DROPPED_FRAME_BUFFER[KYWY_DISPLAY_DRIVER::KYWY_DISPLAY_BUFFER_SIZE];

void KYWY_DISPLAY_DRIVER::initializeDisplay() {
  // SPI hardware is initialized by SPIBus::initialize() called from Kywy.cpp

  pinMode(KYWY_DISPLAY_CS, OUTPUT);
  pinMode(KYWY_DISPLAY_DISP, OUTPUT);

  digitalWrite(KYWY_DISPLAY_CS, LOW); // Keep CS low (disabled) for Sharp Memory Display
  digitalWrite(KYWY_DISPLAY_DISP, LOW); //ensure display is off during init

  addCommandsToBuffer(KYWY_DISPLAY_ACTIVE_BUFFER);
  addCommandsToBuffer(KYWY_DISPLAY_TRANSFER_BUFFER);
  addCommandsToBuffer(KYWY_DISPLAY_DROPPED_FRAME_BUFFER);

  clearBuffer();

  sendBufferToDisplay();

  digitalWrite(KYWY_DISPLAY_DISP, HIGH); // turn on display

  setRotation(Rotation::DEFAULT);
}

void KYWY_DISPLAY_DRIVER::setRotation(Rotation rotation) {
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

void KYWY_DISPLAY_DRIVER::clearBuffer() {
  // Initialize the buffer with the exact DMA-ready format
  addCommandsToBuffer(KYWY_DISPLAY_ACTIVE_BUFFER);

  // Clear the pixel data areas to white (0xff)
  const size_t LINES = KYWY_DISPLAY_HEIGHT;              // 168 lines
  const size_t LINE_BYTES = KYWY_DISPLAY_WIDTH / 8 + 2;  // 20 bytes per line

  for (size_t line = 0; line < LINES; ++line) {
    size_t base = 1 + line * LINE_BYTES;
    // Clear the 18 pixel data bytes (0xff = white pixels)
    memset(KYWY_DISPLAY_ACTIVE_BUFFER + base + 1, 0xff, 18);
  }
}

bool KYWY_DISPLAY_DRIVER::sendBufferToDisplay() {
  // Check that there is something to send (callback needs to do in case of dropped frames)
  if (!displayPending && !droppedFrame) {
    // No update pending and no dropped frame, nothing to send
    return true;  // Nothing to do, but not a failure
  }

  // Check if we have a fresh frame to display
  if (displayPending) {
    // Copy Active buffer to Transfer buffer to prepare for sending
    if (SPIBus::isBusLocked()) {
      // SPI bus busy, drop frame
      droppedFrame = true;  // Mark that we have a dropped frame to send
      memcpy(KYWY_DISPLAY_DROPPED_FRAME_BUFFER, KYWY_DISPLAY_ACTIVE_BUFFER, KYWY_DISPLAY_BUFFER_SIZE);
      return false;
    }
    memcpy(KYWY_DISPLAY_TRANSFER_BUFFER, KYWY_DISPLAY_ACTIVE_BUFFER, KYWY_DISPLAY_BUFFER_SIZE);
  } 
  
  // check that we have a dropped frame to send (ignore if we have a fresh frame)
  if (!displayPending && droppedFrame) {
    // Copy dropped frame buffer (command-structured) to transfer buffer
    if (SPIBus::isBusLocked()) {
      // SPI bus busy, cannot send dropped frame now, it will remain dropped
      return false;
    }
    memcpy(KYWY_DISPLAY_TRANSFER_BUFFER, KYWY_DISPLAY_DROPPED_FRAME_BUFFER, KYWY_DISPLAY_BUFFER_SIZE);
  }

  //  Toggle VCOM
  if (lastTimeVcomToggled + 950 < millis()) {
    // Ensure VCOM is toggled at 950ms
    lastTimeVcomToggled = millis();
  } else {
    vcom = vcom ? 0x00 : vcomCommand;  // toggle vcom
  }
  // SET VCOM AND WRITE COMMAND in transfer buffer
  KYWY_DISPLAY_TRANSFER_BUFFER[0] = vcom | writeCommand;
  addCommandsToBuffer(KYWY_DISPLAY_TRANSFER_BUFFER);  // ensure commands are correct

  // Start DMA transfer via SPIBus with Sharp Memory Display configuration
  // CS pin: KYWY_DISPLAY_CS, active HIGH (per Sharp Memory Display datasheet)
  // Frequency: 2MHz (2000000 Hz)
  if (!SPIBus::startDMATransfer(KYWY_DISPLAY_TRANSFER_BUFFER, KYWY_DISPLAY_BUFFER_SIZE, KYWY_DISPLAY_CS, true, KYWY_DISPLAY_FREQUENCY, displayDMAComplete)) {
    // Failed to start transfer, bus was busy!! This should be rare since we checked bus was free
    droppedFrame = true;  // Mark that we have a dropped frame to send
    memcpy(KYWY_DISPLAY_DROPPED_FRAME_BUFFER, KYWY_DISPLAY_ACTIVE_BUFFER, KYWY_DISPLAY_BUFFER_SIZE); // Copy current buffer to dropped frame buffer
    return false;
  }

  // Clear flags since transfer was started
  displayPending = false;
  droppedFrame = false;

  // Transfer started successfully and happens asynchronously via DMA
  // SPIBus will call displayDMAComplete() when done

  return true;
}

uint16_t KYWY_DISPLAY_DRIVER::mapDisplayToBufferByte(int16_t x, int16_t y) {
  // Map to the command-structured buffer:
  // Byte 0: VCOM+COMMAND
  // Each line (y): 1 address byte + 18 data bytes + 1 padding
  // So line y starts at: 1 + y * 20
  // Pixel data starts at: 1 + y * 20 + 1 = y * 20 + 2
  // Byte within line: x / 8
  int index = (20 * y) + (x / 8) + 2;
  return index;
}

uint8_t KYWY_DISPLAY_DRIVER::mapDisplayToBufferBit(int16_t x, int16_t y) {
  // Return the raw bit position - bit reversal will be handled in setBufferPixel
  int bit = x % 8;
  return bit;
}

void KYWY_DISPLAY_DRIVER::setBufferPixel(int16_t x, int16_t y, uint16_t color) {
  if (x < 0 || x >= 144 || y < 0 || y >= 168) {
    return;
  }

  int index = mapDisplayToBufferByte(x, y);
  int bit = mapDisplayToBufferBit(x, y);

  if (color) {
    KYWY_DISPLAY_ACTIVE_BUFFER[index] =
      KYWY_DISPLAY_ACTIVE_BUFFER[index] | (1 << (7 - bit));
  } else {
    KYWY_DISPLAY_ACTIVE_BUFFER[index] =
      KYWY_DISPLAY_ACTIVE_BUFFER[index] & (0xff ^ (1 << (7 - bit)));
  }
}

void KYWY_DISPLAY_DRIVER::addCommandsToBuffer(uint8_t *buffer) {
  // Build the exact DMA-ready buffer format
  const size_t LINES = KYWY_DISPLAY_HEIGHT;              // 168 lines
  const size_t LINE_BYTES = KYWY_DISPLAY_WIDTH / 8 + 2;  // 18 data + 2 (line addr + trailing 0)

  // Header (will be updated with VCOM in sendBufferToDisplay)
  buffer[0] = writeCommand;

  // Fill lines
  for (size_t line = 0; line < LINES; ++line) {
    size_t base = 1 + line * LINE_BYTES;
    buffer[base + 0] = reverse(line + 1);
    // Note: pixel data at buffer[base + 1] to buffer[base + 18] will be set by drawing functions
    buffer[base + 19] = 0x00;  // trailing byte for this line
  }

  // Final tail byte
  buffer[1 + (LINES * LINE_BYTES)] = 0x00;
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

void KYWY_DISPLAY_DRIVER::writeBitmapOrBlockToBuffer(
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
  uint8_t *buffer = KYWY_DISPLAY_ACTIVE_BUFFER + mapDisplayToBufferByte(x, y);

  // index bitmap by bits instead of bytes to handle all the byte splitting
  uint16_t bitmapBitIndex = bitmapWidth * bitmapY + bitmapX;

  // precomputed values
  uint8_t bufferBitsNotToWriteToInLeftByteColumn = mapDisplayToBufferBit(x, y);
  uint8_t bufferBitsToWriteToInLeftByteColumn = 8 - bufferBitsNotToWriteToInLeftByteColumn;

  // buffer wrap distance calculation
  int splitLeftBits =
    8 - bufferBitsNotToWriteToInLeftByteColumn;  // how many bits of the left most byte column need to be filled
  splitLeftBits =
    splitLeftBits == 8
      ? 0
      : splitLeftBits;  // if we have a whole column on the left just include
                        // it as part of the inner bytes
  int splitRightBits =
    (x + width) % 8;  // how many bits of the right most byte column need to be filled
  uint16_t innerBytes =
    (width - splitLeftBits - splitRightBits) / 8;  // how many bytes are between the right and left column

  // Buffer wrap distance for new command-structured buffer:
  // Each line has 20 bytes: 1 address + 18 data + 1 padding
  // We need to account for:
  // 1. Bytes from current position to end of current line's pixel data
  // 2. Skip over padding byte and next line's address byte
  // 3. Position at start of next line's pixel data
  uint16_t remainingBytesInLine = 18 - innerBytes - (splitLeftBits ? 1 : 0) - (splitRightBits ? 1 : 0);
  uint16_t bufferWrapDistance = remainingBytesInLine + 2;  // +2 for padding + address

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

void KYWY_DISPLAY_DRIVER::setBufferBlock(int16_t x, int16_t y, uint16_t width,
                                         uint16_t height, uint16_t color) {
  writeBitmapOrBlockToBuffer(x, y, width, height, nullptr,
                             BitmapOptions().opaque(true), true, color);
}

void KYWY_DISPLAY_DRIVER::writeBitmapToBuffer(int16_t x, int16_t y, uint16_t width,
                                              uint16_t height, uint8_t *bitmap,
                                              BitmapOptions options) {
  writeBitmapOrBlockToBuffer(x, y, width, height, bitmap, options, false, 0x00);
}

}  // namespace Driver

void Display::setup() {
  // Initialize global driver reference for callback access
  globalDisplayDriver = driver;

  // Start display event queue thread if not already started
  static bool displayThreadStarted = false;
  if (!displayThreadStarted) {
    displayThread.start(mbed::callback(&displayEventQueue, &events::EventQueue::dispatch_forever));
    displayThreadStarted = true;
  }

  driver->initializeDisplay();
}

void Display::clear() {
  displayPending = false; // ensure no accidental update after clear
  driver->clearBuffer();
}

bool Display::update() {
  displayPending = true; // flag display needs updating
  droppedFrame = false;  // No longer trying to send a dropped frame since we have a new update
  return driver->sendBufferToDisplay();  // Return true if frame was sent immediately, false if dropped
}

void Display::setRotation(Rotation rotation) {
  driver->setRotation(rotation);
}

// Performance monitoring functions
int Display::getTotalCallbackSends() {
  return displayTotalCallbackSends;
}

int Display::getTotalSuccessfulSends() {
  return displayTotalSuccessfulSends;
}

void Display::resetPerformanceCounters() {
  displayTotalCallbackSends = 0;
  displayTotalSuccessfulSends = 0;
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
