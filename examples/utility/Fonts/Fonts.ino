// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

void setup() {
  engine.start();

  int y = 5;

  // intel one mono
  engine.display.drawText(5, y, "abcd ABCD", Display::TextOptions().font(Display::Font::intel_one_mono_8_pt));
  y += 8 + 3;
  engine.display.drawText(5, y, "abcd ABCD", Display::TextOptions().font(Display::Font::intel_one_mono_12_pt));
  y += 12 + 3;
  engine.display.drawText(5, y, "abcd ABCD", Display::TextOptions().font(Display::Font::intel_one_mono_16_pt));
  y += 16 + 3;

  y += 5;

  // bailleul
  engine.display.drawText(5, y, "abcd ABCD", Display::TextOptions().font(Display::Font::bailleul_8_pt));
  y += 8 + 3;
  engine.display.drawText(5, y, "abcd ABCD", Display::TextOptions().font(Display::Font::bailleul_12_pt));
  y += 12 + 3;
  engine.display.drawText(5, y, "abcd ABCD", Display::TextOptions().font(Display::Font::bailleul_16_pt));
  y += 16 + 3;

  y += 5;

  // bailleul bold
  engine.display.drawText(5, y, "abcd ABCD", Display::TextOptions().font(Display::Font::bailleul_bold_8_pt));
  y += 8 + 3;
  engine.display.drawText(5, y, "abcd ABCD", Display::TextOptions().font(Display::Font::bailleul_bold_12_pt));
  y += 12 + 3;
  engine.display.drawText(5, y, "abcd ABCD", Display::TextOptions().font(Display::Font::bailleul_bold_16_pt));
  y += 16 + 3;

  engine.display.update();
}

void loop() {
  delay(1000);
}
