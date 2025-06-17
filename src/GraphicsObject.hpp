// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_GRAPHICS_OBJECT
#define KYWY_LIB_GRAPHICS_OBJECT 1

#include "Display.hpp"

class GraphicsObject {
public:
  void render();
  void setDisplay(Display::Display *display);
  void setPosition(int16_t x, int16_t y);

  bool isVisible();
  void setVisible(bool visible);
  // onSetVisible is a hook to perform actions after the object is set visible
  virtual void onSetVisible() {}

  Display::Display *display;

  // onSetDisplay is a hook to perform calculations once the display interface
  // is available
  virtual void onSetDisplay() {}

  int16_t x = 0;
  int16_t y = 0;

  virtual void draw() = 0;
  virtual void erase(int16_t lastRenderedX, int16_t lastRenderedY) = 0;

private:
  bool visible = false;

  bool lastRenderedVisible = false;
  int16_t lastRenderedX = 0;
  int16_t lastRenderedY = 0;
};

#endif