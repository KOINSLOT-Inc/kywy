// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Display.hpp"

namespace Display {

void Display::drawCircleWithEvenDiameterFromTopLeftCorner(int16_t x, int16_t y,
                                                          uint16_t diameter,
                                                          uint16_t color,
                                                          bool fill) {
  int16_t radius = diameter / 2;
  int16_t xOffset = 0, yOffset = -radius + 1;

  int16_t xTopLeftCenter = x + radius - 1, yTopLeftCenter = y + radius - 1;
  int16_t xTopRightCenter = x + radius - 0, yTopRightCenter = y + radius - 1;
  int16_t xBottomLeftCenter = x + radius - 1,
          yBottomLeftCenter = y + radius - 0;
  int16_t xBottomRightCenter = x + radius - 0,
          yBottomRightCenter = y + radius - 0;

  int16_t discriminatorThreshold;
  if (radius <= 3) {
    discriminatorThreshold = 0;
  } else if (radius <= 6) {
    discriminatorThreshold = 3;
  } else {
    discriminatorThreshold = 5;
  }

  while (xOffset <= (-1 * yOffset)) {
    // leverage 8-way symmetry

    if (fill) {
      driver->setBufferBlock(xTopLeftCenter - xOffset, yTopLeftCenter + yOffset,
                             2 * (abs(xOffset) + 1), 1, color);
      driver->setBufferBlock(xTopLeftCenter + yOffset, yTopLeftCenter - xOffset,
                             2 * (abs(yOffset) + 1), 1, color);
      driver->setBufferBlock(xBottomLeftCenter - xOffset,
                             yBottomLeftCenter - yOffset,
                             2 * (abs(xOffset) + 1), 1, color);
      driver->setBufferBlock(xBottomLeftCenter + yOffset,
                             yBottomLeftCenter + xOffset,
                             2 * (abs(yOffset) + 1), 1, color);
    } else {
      // top left quadrant
      driver->setBufferPixel(xTopLeftCenter - xOffset, yTopLeftCenter + yOffset,
                             color);
      driver->setBufferPixel(xTopLeftCenter + yOffset, yTopLeftCenter - xOffset,
                             color);

      // top right quadrant
      driver->setBufferPixel(xTopRightCenter + xOffset,
                             yTopRightCenter + yOffset, color);
      driver->setBufferPixel(xTopRightCenter - yOffset,
                             yTopRightCenter - xOffset, color);

      // bottom left quadrant
      driver->setBufferPixel(xBottomLeftCenter - xOffset,
                             yBottomLeftCenter - yOffset, color);
      driver->setBufferPixel(xBottomLeftCenter + yOffset,
                             yBottomLeftCenter + xOffset, color);

      // bottom right quadrant
      driver->setBufferPixel(xBottomRightCenter + xOffset,
                             yBottomRightCenter - yOffset, color);
      driver->setBufferPixel(xBottomRightCenter - yOffset,
                             yBottomRightCenter + xOffset, color);
    }

    xOffset++;

    int16_t discriminator = (xOffset * xOffset) + (yOffset * yOffset) - ((radius - 1) * (radius - 1));
    if (discriminator > discriminatorThreshold) {
      yOffset++;
    }
  }
};

void Display::drawCircleWithOddDiameterFromCenter(int16_t x, int16_t y,
                                                  uint16_t diameter,
                                                  uint16_t color, bool fill) {
  int16_t radius = diameter / 2;
  int16_t xOffset = 0, yOffset = -radius;

  int16_t discriminatorThreshold;
  switch (radius) {
    case 1:
      discriminatorThreshold = 0;
      break;
    case 2:
      discriminatorThreshold = 1;
      break;
    case 3:
      discriminatorThreshold = 3;
      break;
    default:
      discriminatorThreshold = 5;
  }

  while (xOffset <= (-1 * yOffset)) {

    if (fill) {
      driver->setBufferBlock(x - xOffset, y + yOffset, 2 * abs(xOffset) + 1, 1,
                             color);
      driver->setBufferBlock(x + yOffset, y - xOffset, 2 * abs(yOffset) + 1, 1,
                             color);
      driver->setBufferBlock(x - xOffset, y - yOffset, 2 * abs(xOffset) + 1, 1,
                             color);
      driver->setBufferBlock(x + yOffset, y + xOffset, 2 * abs(yOffset) + 1, 1,
                             color);

    } else {
      // leverage 8-way symmetry
      driver->setBufferPixel(x + xOffset, y + yOffset, color);
      driver->setBufferPixel(x + xOffset, y - yOffset, color);
      driver->setBufferPixel(x - xOffset, y + yOffset, color);
      driver->setBufferPixel(x - xOffset, y - yOffset, color);
      driver->setBufferPixel(x + yOffset, y + xOffset, color);
      driver->setBufferPixel(x + yOffset, y - xOffset, color);
      driver->setBufferPixel(x - yOffset, y + xOffset, color);
      driver->setBufferPixel(x - yOffset, y - xOffset, color);
    }
    xOffset++;

    int16_t discriminator =
      (xOffset * xOffset) + (yOffset * yOffset) - (radius * radius);
    if (discriminator > discriminatorThreshold) {
      yOffset++;
    }
  }
};

void Display::drawOrFillCircle(Origin::Object2D origin, int16_t x, int16_t y,
                               uint16_t diameter, uint16_t color, bool fill) {
  if (diameter % 2 == 0) {  // even diameter
    switch (origin) {
      case Origin::Object2D::TOP_LEFT:
        drawCircleWithEvenDiameterFromTopLeftCorner(x, y, diameter, color, fill);
        break;
      case Origin::Object2D::TOP_RIGHT:
        drawCircleWithEvenDiameterFromTopLeftCorner(x - diameter, y, diameter,
                                                    color, fill);
        break;
      case Origin::Object2D::BOTTOM_LEFT:
        drawCircleWithEvenDiameterFromTopLeftCorner(x, y - diameter, diameter,
                                                    color, fill);
        break;
      case Origin::Object2D::BOTTOM_RIGHT:
        drawCircleWithEvenDiameterFromTopLeftCorner(x - diameter, y - diameter,
                                                    diameter, color, fill);
        break;
      case Origin::Object2D::CENTER:
        // Since there is no pixel center of an even diameter circle we bias to
        // the bottom right. E.g. below there are 4 possible "center" pixels for
        // the `diameter = 8` circle represented by the "X" and 3 "O"'s. Since the
        // origin of the screen is the top left corner we choose the pixel who's
        // top left corner is the center of the circle: the "X". This means that
        // the left bound of the circle will be `x - (diameter / 2)` and the upper
        // bound of the circle will be `y - (diameter / 2)`. However, because the
        // center is biased to the bottom right the right bound will be `x +
        // (diameter / 2) - 1` and the bottom bound will be `y + (diameter / 2) -
        // 1`.
        //
        // ############
        // ####    ####
        // ### #### ###
        // ## ###### ##
        // ## ##OO## ##
        // ## ##OX## ##
        // ## ###### ##
        // ### #### ###
        // ####    ####
        // ############
        drawCircleWithEvenDiameterFromTopLeftCorner(
          x - (diameter / 2), y - (diameter / 2), diameter, color, fill);
        break;
    }
  } else {  // odd diameter
    switch (origin) {
      case Origin::Object2D::TOP_LEFT:
        drawCircleWithOddDiameterFromCenter(
          x + (diameter / 2), y + (diameter / 2), diameter, color, fill);
        break;
      case Origin::Object2D::TOP_RIGHT:
        drawCircleWithOddDiameterFromCenter(
          x - (diameter / 2), y + (diameter / 2), diameter, color, fill);
        break;
      case Origin::Object2D::BOTTOM_LEFT:
        drawCircleWithOddDiameterFromCenter(
          x + (diameter / 2), y - (diameter / 2), diameter, color, fill);
        break;
      case Origin::Object2D::BOTTOM_RIGHT:
        drawCircleWithOddDiameterFromCenter(
          x - (diameter / 2), y - (diameter / 2), diameter, color, fill);
        break;
      case Origin::Object2D::CENTER:
        drawCircleWithOddDiameterFromCenter(x, y, diameter, color, fill);
        break;
    }
  }
};

void Display::drawCircle(int16_t x, int16_t y, uint16_t diameter,
                         Object2DOptions options) {
  drawOrFillCircle(options.getOrigin(), x, y, diameter, options.getColor(),
                   false);
}

void Display::fillCircle(int16_t x, int16_t y, uint16_t diameter,
                         Object2DOptions options) {
  drawOrFillCircle(options.getOrigin(), x, y, diameter, options.getColor(),
                   true);
}

}  // namespace Display
