// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"
#include "MenuSystem.hpp"

Kywy::Engine engine;
bool toggleOption = false;

void exampleAction() {
    engine.display.clear();
    engine.display.drawText(10, 10, "Action executed!", Display::TextOptions().color(0xFF));
    engine.display.update();
    delay(2000);
}

void testDisplay() {
    engine.display.clear();
    engine.display.drawText(10, 10, "Test Display", Display::TextOptions().color(0xFF));
    engine.display.update();
    delay(2000);
}

// Ensure the menu is displayed at the start
void setup() {
    engine.start();

    std::vector<Kywy::MenuSystem::MenuItem> menuItems = {
        {"Start Game", nullptr, []() { engine.display.clear(); engine.display.drawText(10, 10, "Starting game...", Display::TextOptions().color(0x00)); engine.display.update(); delay(1000); }},
        {"Toggle Option", &toggleOption, nullptr},
        {"Execute Action", nullptr, exampleAction}
    };

    Kywy::MenuSystem::MenuOptions options;
    options.x = 5;
    options.y = 5;
    options.itemHeight = 15;
    options.pointer = '>';

    Kywy::MenuSystem menu(engine.display, menuItems, options);
    menu.start(engine); // Pass the engine instance to start the menu system
}

void loop() {
    // Empty loop as the menu runs in setup
}