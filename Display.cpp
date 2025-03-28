// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Display.hpp"

namespace Display {

namespace Driver {

void MBED_SPI_DRIVER::initializeDisplay() {
  mbedSPI =
      new mbed::SPI((PinName)KYWY_DISPLAY_MOSI, (PinName)KYWY_DISPLAY_MISO,
                    (PinName)KYWY_DISPLAY_SCK);
  mbedSPI->format(8, 0);
  mbedSPI->frequency(8000000);

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

void MBED_SPI_DRIVER::sendBufferToDisplay() {
  mbedSPI->lock();
  digitalWrite(KYWY_DISPLAY_CS, HIGH);

  mbedSPI->write(vcom | writeCommand);
  vcom = vcom ? 0x00 : vcomCommand; // toggle vcom at least 1 time per second to
                                    // prevent DC bias

  for (int line = 0; line < 168; line++) {
    MBED_SPI_DRIVER_LINE_BUFFER[0] = reverse(line + 1);
    memcpy((void *)(MBED_SPI_DRIVER_LINE_BUFFER + 1),
           (const void *)(MBED_SPI_DRIVER_BUFFER + 18 * line), 18);
    MBED_SPI_DRIVER_LINE_BUFFER[19] = 0x00;
    mbedSPI->write((const char *)MBED_SPI_DRIVER_LINE_BUFFER, 20,
                   (char *)MBED_SPI_DRIVER_RX_BUFFER, 20);
  }
  mbedSPI->write(0x00);

  digitalWrite(KYWY_DISPLAY_CS, LOW);
  mbedSPI->unlock();
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
  if (x > (getWidth() - 1) || y > (getHeight() - 1) || x + width - 1 < 0 ||
      y + height - 1 < 0)
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

void MBED_SPI_DRIVER::setBufferBlock(int16_t x, int16_t y, uint16_t width,
                                     uint16_t height, uint16_t color) {
  if (!cropBlock(x, y, width, height))
    return; // no overlap between block and screen

  // top left corner of the cropped block
  uint8_t *buffer = MBED_SPI_DRIVER_BUFFER + (18 * y) + (x / 8);
  int bit = x % 8;

  //                                   bitmap width
  //                   |-------------------------------------------|
  //    buffer ->  ________ ________ ________ ________ ________ ________
  //
  //                   |--| |---------------------------------| |--|
  //                    ^                                         ^
  //  splitLeft = true _|             innerBytes = 4              |_ splitRight
  //  = true

  int splitLeftBits =
      8 - x % 8; // how many bits of the left most byte column need to be filled
  splitLeftBits =
      splitLeftBits == 8
          ? 0
          : splitLeftBits; // if we have a whole column on the left just include
                           // it as part of the inner bytes
  int splitRightBits =
      (x + width) %
      8; // how many bits of the right most byte column need to be filled
  uint16_t innerBytes =
      (width - splitLeftBits - splitRightBits) /
      8; // how many bytes are between the right and left column

  // special case where the block only takes up a single column, e.g.
  // 0b00000000
  // 0b00011100
  // 0b00011100
  // 0b00011100
  // 0b00000000
  bool singleColumn = width <= (8 - bit);

  // precompute colors
  uint8_t leftColumnColor, innerColor, rightColumnColor;
  if (color) {
    leftColumnColor =
        0xff >> (8 - splitLeftBits); // OR mask, e.g. if splitLeftBits == 2 then
                                     // leftColumnColor == 0b00000011
    innerColor = 0xff;
    rightColumnColor =
        0xff << (8 - splitRightBits); // OR mask, e.g. if splitRightBits == 2
                                      // then rightColumnColor == 0b11000000

    if (singleColumn) {
      innerColor = (splitLeftBits == 0 ? 0xff : leftColumnColor) &
                   (splitRightBits == 0 ? 0xff : rightColumnColor);
    }
  } else {
    leftColumnColor =
        0xff << splitLeftBits; // AND mask, e.g. if splitLeftBits == 2 then
                               // leftColumnColor == 0b11111100
    innerColor = 0x00;
    rightColumnColor =
        0xff >> splitRightBits; // AND mask, e.g. if splitRightBits == 2 then
                                // rightColumnColor == 0b00111111

    if (singleColumn) {
      innerColor = (splitLeftBits == 0 ? 0x00 : leftColumnColor) |
                   (splitRightBits == 0 ? 0x00 : rightColumnColor);
    }
  }

  // distance to increment buffer to wrap from end of block to beginning of
  // block on next line
  uint16_t wrapDistance =
      18 - innerBytes - (splitLeftBits ? 1 : 0) - (splitRightBits ? 1 : 0);

  if (color) {
    for (int16_t j = 0; j < height; j++) {
      if (singleColumn) {
        *buffer = *buffer | innerColor;
        buffer += wrapDistance + 1;
        continue;
      }

      if (splitLeftBits) { // fill left edge
        *buffer = *buffer | leftColumnColor;
        buffer++;
      }

      if (innerBytes) {
        memset(buffer, 0xff, innerBytes);
        buffer += innerBytes;
      }

      if (splitRightBits) { // fill right edge
        *buffer = *buffer | rightColumnColor;
        buffer++;
      }

      buffer += wrapDistance;
    }
  } else {
    for (int16_t j = 0; j < height; j++) {
      if (singleColumn) {
        *buffer = *buffer & innerColor;
        buffer += wrapDistance + 1;
        continue;
      }

      if (splitLeftBits) { // fill left edge
        *buffer = *buffer & leftColumnColor;
        buffer++;
      }

      if (innerBytes) {
        memset(buffer, 0x00, innerBytes);
        buffer += innerBytes;
      }

      if (splitRightBits) { // fill right edge
        *buffer = *buffer & rightColumnColor;
        buffer++;
      }

      buffer += wrapDistance;
    }
  }
}

void MBED_SPI_DRIVER::writeBitmapToBuffer(int16_t x, int16_t y, uint16_t width,
                                          uint16_t height, uint8_t *bitmap,
                                          BitmapOptions options) {

  // we can write from an arbitrary chunk of the bitmap to an arbitrary chunk of
  // the screen buffer
  uint16_t bitmapX = 0, bitmapY = 0, bitmapWidth = width;

  if (x < 0)
    bitmapX += -1 * x; // left edge of bitmap is off screen

  if (y < 0)
    bitmapY += -1 * y; // top edge of bitmap is off screen

  if (!cropBlock(x, y, width, height))
    return; // no overlap between bitmap and screen

  // get top left corner of block to write on screen
  uint8_t *buffer = MBED_SPI_DRIVER_BUFFER + (18 * y) + (x / 8);

  // index bitmap by bits instead of bytes to handle all the byte splitting
  uint16_t bitmapBitIndex = bitmapWidth * bitmapY + bitmapX;

  // precomputed values
  uint8_t bufferBitsNotToWriteToInLeftByteColumn = x % 8;
  uint8_t bufferBitsToWriteToInLeftByteColumn = 8 - x % 8;

  uint8_t bitmapBitsNotToWriteInLeftByteColumn = bitmapX % 8;
  uint8_t bitmapBitsToWriteInLeftByteColumn = 8 - bitmapX % 8;

  // buffer wrap distance calculation
  int splitLeftBits =
      8 - x % 8; // how many bits of the left most byte column need to be filled
  splitLeftBits =
      splitLeftBits == 8
          ? 0
          : splitLeftBits; // if we have a whole column on the left just include
                           // it as part of the inner bytes
  int splitRightBits =
      (x + width) %
      8; // how many bits of the right most byte column need to be filled
  uint16_t innerBytes =
      (width - splitLeftBits - splitRightBits) /
      8; // how many bytes are between the right and left column
  uint16_t bufferWrapDistance =
      18 - innerBytes - (splitLeftBits ? 1 : 0) - (splitRightBits ? 1 : 0);

  // iterate over each line
  for (int16_t j = 0; j < height; j++) {
    uint16_t bitsLeftToWrite = width;

    while (bitsLeftToWrite) {
      uint8_t byteToWrite;
      uint8_t bitsWritten;
      uint8_t mask; // identifies the part of the byte column we want to write

      byteToWrite =
          ((*(bitmap + (bitmapBitIndex / 8)) << (bitmapBitIndex % 8)) |
           (*(bitmap + (bitmapBitIndex / 8) + 1) >> (8 - bitmapBitIndex % 8)));

      if (options.getNegative()) {
        byteToWrite = ~byteToWrite;
      }

      // we're only writing a single partial column and need to mask both sides
      // of the byteToWrite
      if (bufferBitsToWriteToInLeftByteColumn > width) {
        byteToWrite =
            byteToWrite >>
            bufferBitsNotToWriteToInLeftByteColumn; // shift starting bitmap bit
                                                    // to match starting bit of
                                                    // buffer byte column

        mask = 0xff >> bufferBitsNotToWriteToInLeftByteColumn;  // mask of left side since this is a partial column
        mask &= 0xff
            << (8 -
                (bufferBitsNotToWriteToInLeftByteColumn +
                 width)); // mask off right side since this is a partial column 

        bitsWritten = width;

        // we're on the leftmost column
      } else if (bitsLeftToWrite == width) {
        byteToWrite =
            byteToWrite >>
            bufferBitsNotToWriteToInLeftByteColumn; // shift starting bitmap bit
                                                    // to match starting bit of
                                                    // buffer byte column

        mask = 0xff >> bufferBitsNotToWriteToInLeftByteColumn; // mask off left side

        bitsWritten = bufferBitsToWriteToInLeftByteColumn;

        // we're writing an inner column
      } else if (bitsLeftToWrite >= 8) {
        mask = 0xff; // don't mask off anything

        bitsWritten = 8;

        // we're writing the rightmost column
      } else if (bitsLeftToWrite > 0 & bitsLeftToWrite < 8) {
        mask = 0xff
          << (8 - bitsLeftToWrite); // mask off right side

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
          *buffer = ~byteToWrite;
        } else {
          *buffer = byteToWrite;
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

} // namespace Driver

void Display::setup() { driver->initializeDisplay(); }
void Display::clear() { driver->clearBuffer(); }
void Display::update() { driver->sendBufferToDisplay(); }
void Display::setRotation(Rotation rotation) { driver->setRotation(rotation); }
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
  if (yStart == yEnd) {  // horizontal line
    if (xEnd < xStart) { // setBufferBlock draws left-to-right so make sure xEnd
                         // is >= xStart
      std::swap(xEnd, xStart);
      std::swap(yEnd, yStart);
    }
    driver->setBufferBlock(xStart, yStart, xEnd - xStart + 1, 1,
                           options.getColor());
    return;
  }

  if (xStart == xEnd) {  // vertical line
    if (yEnd < yStart) { // setBufferBlock draws top-to-bottom so make sure yEnd
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
  driver->setBufferBlock(x, y, width, 1, options.getColor()); // top line
  driver->setBufferBlock(x, y + height - 1, width, 1,
                         options.getColor());                  // bottom line
  driver->setBufferBlock(x, y, 1, height, options.getColor()); // left line
  driver->setBufferBlock(x + width - 1, y, 1, height,
                         options.getColor()); // right line
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

} // namespace Display
