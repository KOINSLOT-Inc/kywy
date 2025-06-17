// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_DISPLAY
#define KYWY_LIB_DISPLAY 1

#include "Fonts.hpp"
#include <Arduino.h>
#include <SPIMaster.h>
#include <stdint.h>

// for v0.2 version of board
// #define KYWY_DISPLAY_SCK  18
// #define KYWY_DISPLAY_MOSI 20
// #define KYWY_DISPLAY_CS   17
// #define KYWY_DISPLAY_DISP 22

// for v1.3 version of board
#define KYWY_DISPLAY_SCK 18
#define KYWY_DISPLAY_MOSI 19
#define KYWY_DISPLAY_MISO 16
#define KYWY_DISPLAY_CS 17
#define KYWY_DISPLAY_DISP 22

#define KYWY_DISPLAY_WIDTH 144
#define KYWY_DISPLAY_HEIGHT 168
#define BLACK 0x00
#define WHITE 0xff

namespace Display {

enum class Rotation {
  DEFAULT,
  CLOCKWISE_90,
  CLOCKWISE_180,
  CLOCKWISE_270,
};

namespace Origin {
enum class Object1D {
  ENDPOINT,
  MIDPOINT,
};

enum class Object2D {
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_RIGHT,
  CENTER,
};

enum class Text {
  // duplicate of Object2D
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_RIGHT,
  CENTER,

  // text specific origins
  BASELINE_LEFT,
  BASELINE_CENTER,
  BASELINE_RIGHT,
};
}  // namespace Origin

struct Object1DOptions {
  uint16_t _color = 0x00;
  Origin::Object1D _origin = Origin::Object1D::ENDPOINT;

  Object1DOptions color(uint16_t setColor) {
    _color = setColor;
    return *this;
  };
  uint16_t getColor() {
    return _color;
  };

  Object1DOptions origin(Origin::Object1D setOrigin) {
    _origin = setOrigin;
    return *this;
  };
  Origin::Object1D getOrigin() {
    return _origin;
  };
};

struct Object2DOptions {
  uint16_t _color = 0x00;
  Origin::Object2D _origin = Origin::Object2D::TOP_LEFT;

  Object2DOptions color(uint16_t setColor) {
    _color = setColor;
    return *this;
  };
  uint16_t getColor() {
    return _color;
  };

  Object2DOptions origin(Origin::Object2D setOrigin) {
    _origin = setOrigin;
    return *this;
  };
  Origin::Object2D getOrigin() {
    return _origin;
  };
};

struct TextOptions {
  uint16_t _color = 0x00;
  Origin::Text _origin = Origin::Text::TOP_LEFT;
  uint8_t *_font = nullptr;
  bool _opaque = false;

  TextOptions color(uint16_t setColor) {
    _color = setColor;
    return *this;
  };
  uint16_t getColor() {
    return _color;
  };

  TextOptions origin(Origin::Text setOrigin) {
    _origin = setOrigin;
    return *this;
  };
  Origin::Text getOrigin() {
    return _origin;
  };

  TextOptions font(uint8_t *setFont) {
    _font = setFont;
    return *this;
  };
  uint8_t *getFont() {
    return _font;
  };

  TextOptions opaque(bool setOpaque) {
    _opaque = setOpaque;
    return *this;
  };
  bool getOpaque() {
    return _opaque;
  };
};

struct BitmapOptions {
  uint16_t _color = 0x00;
  bool _negative = false;
  Origin::Object2D _origin = Origin::Object2D::TOP_LEFT;
  bool _opaque = false;

  BitmapOptions color(uint16_t setColor) {
    _color = setColor;
    return *this;
  };
  uint16_t getColor() {
    return _color;
  };

  BitmapOptions negative(bool setNegative) {
    _negative = setNegative;
    return *this;
  };
  uint16_t getNegative() {
    return _negative;
  };

  BitmapOptions origin(Origin::Object2D setOrigin) {
    _origin = setOrigin;
    return *this;
  };
  Origin::Object2D getOrigin() {
    return _origin;
  };

  BitmapOptions opaque(bool setOpaque) {
    _opaque = setOpaque;
    return *this;
  };
  bool getOpaque() {
    return _opaque;
  };
};

namespace Driver {

struct PinMap {
  uint8_t cs;    // Pin: Chip Select
  uint8_t rst;   // Pin: Reset
  uint8_t dc;    // Pin: Data/Command
  uint8_t sclk;  // Pin: Serial Clock
  uint8_t mosi;  // Pin: Master Out/Slave In
};

class Driver {
public:
  Driver() {};
  Driver(PinMap pins)
    : pins(pins) {};
  virtual ~Driver() {};

  PinMap pins;
  virtual uint16_t getWidth() = 0;
  virtual uint16_t getHeight() = 0;

  virtual void initializeDisplay() = 0;
  virtual void clearBuffer() = 0;
  virtual void sendBufferToDisplay() = 0;

  virtual void setRotation(Rotation rotation) = 0;

  // set a single pixel
  virtual void setBufferPixel(int16_t x, int16_t y, uint16_t color) = 0;

  // set a rectangle to a single color
  virtual void setBufferBlock(int16_t x, int16_t y, uint16_t width,
                              uint16_t height, uint16_t color) = 0;

  // writes a bitmap to the buffer
  virtual void writeBitmapToBuffer(int16_t x, int16_t y, uint16_t width,
                                   uint16_t height, uint8_t *bitmap,
                                   BitmapOptions options = BitmapOptions()) = 0;

protected:
  // crops a block within the screen bounds, returns false if the block doesn't
  // overlap with the screen
  bool cropBlock(int16_t &x, int16_t &y, uint16_t &width, uint16_t &height);
};

class MBED_SPI_DRIVER : public Driver {
public:
  uint16_t getWidth() {
    return 144;
  };
  uint16_t getHeight() {
    return 168;
  };

  MBED_SPI_DRIVER() {};
  ~MBED_SPI_DRIVER() {
    delete mbedSPI;
  };

  void initializeDisplay();
  void clearBuffer();
  void sendBufferToDisplay();

  void setRotation(Rotation rotation);

  void setBufferPixel(int16_t x, int16_t y, uint16_t color);

  void writeBitmapOrBlockToBuffer(int16_t x, int16_t y, uint16_t width,
                                  uint16_t height, uint8_t *bitmap,
                                  BitmapOptions options = BitmapOptions(),
                                  bool block = false,
                                  uint16_t blockColor = 0x00);

  // inherits from the utility function above
  void setBufferBlock(int16_t x, int16_t y, uint16_t width, uint16_t height,
                      uint16_t color);
  void writeBitmapToBuffer(int16_t x, int16_t y, uint16_t width,
                           uint16_t height, uint8_t *bitmap,
                           BitmapOptions options = BitmapOptions());

private:
  mbed::SPI *mbedSPI;
  uint8_t clearCommand = 0x20;
  uint8_t writeCommand = 0x80;

  uint8_t vcomCommand = 0x40;
  uint8_t vcom = 0x40;  // this value will be toggled between 0x40 and 0x00

  uint8_t MBED_SPI_DRIVER_BUFFER[(144 * 168) / 8] = { 0 };
  uint8_t MBED_SPI_DRIVER_LINE_BUFFER[20] = { 0 };
  uint8_t MBED_SPI_DRIVER_RX_BUFFER[20] = { 0 };

  const unsigned char nibbleFlipper[16] = { 0x0, 0x8, 0x4, 0xc, 0x2, 0xa,
                                            0x6, 0xe, 0x1, 0x9, 0x5, 0xd,
                                            0x3, 0xb, 0x7, 0xf };
  uint8_t reverse(uint8_t n) {
    return (nibbleFlipper[n & 0b1111] << 4) | nibbleFlipper[n >> 4];
  }
};

}  // namespace Driver

class Display {
public:
  Display() {};
  Display(Driver::Driver *driver)
    : driver(driver) {};

  void setup();
  void clear();
  void update();

  void setRotation(Rotation rotation);

  void drawPixel(int16_t x, int16_t y, uint16_t color = 0x00);

  void drawLine(int16_t xStart, int16_t yStart, int16_t xEnd, int16_t yEnd,
                Object1DOptions options = Object1DOptions());
  void drawLine(int16_t x, int16_t y, double length, double angle,
                Object1DOptions options = Object1DOptions());

  void drawCircle(int16_t x, int16_t y, uint16_t diameter,
                  Object2DOptions options = Object2DOptions());
  void fillCircle(int16_t x, int16_t y, uint16_t diameter,
                  Object2DOptions options = Object2DOptions());

  void drawRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height,
                     Object2DOptions options = Object2DOptions());
  void fillRectangle(int16_t x, int16_t y, uint16_t width, uint16_t height,
                     Object2DOptions options = Object2DOptions());

  void drawBitmap(int16_t x, int16_t y, uint16_t width, uint16_t height,
                  uint8_t *bitmap, BitmapOptions options = BitmapOptions());

  void drawText(int16_t x, int16_t y, const char *text,
                TextOptions options = TextOptions());
  void getTextSize(const char *text, uint16_t &width, uint16_t &height,
                   TextOptions options = TextOptions());
  void setFont(uint8_t *font);

  Driver::Driver *driver;

private:
  uint8_t *defaultFont = Font::intel_one_mono_8_pt;

  void drawCircleWithEvenDiameterFromTopLeftCorner(int16_t x, int16_t y,
                                                   uint16_t diameter,
                                                   uint16_t color, bool fill);
  void drawCircleWithOddDiameterFromCenter(int16_t x, int16_t y,
                                           uint16_t diameter, uint16_t color,
                                           bool fill);
  void drawOrFillCircle(Origin::Object2D origin, int16_t x, int16_t y,
                        uint16_t diameter, uint16_t color, bool fill);

  void shiftOrigin2DToTopLeft(Origin::Object2D origin, int16_t &x, int16_t &y,
                              uint16_t width, uint16_t height);

  // read the first UTF-8 character from a string and advance the string pointer
  // however many bytes the character spans
  uint16_t readUTF8Char(const char *&string);

  // also sets the font origin X and Y offset from the top left corner of the
  // string bounding box
  void getTextSize(uint8_t *fontData, const char *text, uint16_t &width,
                   uint16_t &height, int16_t &originXOffset,
                   int16_t &originYOffset, uint16_t &baselineLength);
};

}  // namespace Display

#endif
