// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "GraphicsObject.hpp"

void GraphicsObject::setPosition(int16_t x, int16_t y) {
  this->x = x;
  this->y = y;

  if (!this->isVisible()) {
    lastRenderedX = x;
    lastRenderedY = y;
  }
};

void GraphicsObject::setDisplay(Display::Display *display) {
  this->display = display;
  onSetDisplay();
};

void GraphicsObject::render() {
  if (lastRenderedVisible) {  // erase current position
    this->erase(lastRenderedX, lastRenderedY);
    lastRenderedVisible = false;
  }

  if (!isVisible())
    return;

  this->draw();

  lastRenderedVisible = true;
  lastRenderedX = x;
  lastRenderedY = y;
};

bool GraphicsObject::isVisible() {
  return this->visible;
}
void GraphicsObject::setVisible(bool visible) {
  this->visible = visible;
  onSetVisible();
}