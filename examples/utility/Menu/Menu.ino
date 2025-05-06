// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"
#include "MenuSystem.hpp"
#include <sys/types.h>

Kywy::Engine engine;
bool toggleOption = false;
Kywy::MenuSystem* menuSystem = nullptr; // Pass null for un-needed options

void exampleFunction() {
    engine.display.clear();
    engine.display.drawText(10, 10, "Function executed!", Display::TextOptions().color(0x00));
    engine.display.drawText(10, 20, "Toggle Option is:", Display::TextOptions().color(0x00));
    engine.display.drawText(10, 30, toggleOption ? "ON" : "OFF", Display::TextOptions().color(0x00));
    engine.display.update();
    delay(2000);
}

void setup() {
    Serial.begin(9600);
    engine.start();

    std::vector<Kywy::MenuSystem::MenuItem> menuItems = {
        {"Display Message", nullptr, []() { engine.display.clear(); engine.display.drawText(10, 10, "Displaying message...", Display::TextOptions().color(0x00)); engine.display.update(); delay(2000); }}, // Text, bool for toggle, fuction pointer
        {"Toggle Option", &toggleOption, nullptr}, // This makes a bool for toggleOption
        {"Execute Function", nullptr, exampleFunction}
    };

    Kywy::MenuSystem::MenuOptions menuOptions;
    menuOptions.x = 0;
    menuOptions.y = 5;
    menuOptions.itemHeight = 15;
    menuOptions.pointer = '>';
    menuOptions.font = Display::Font::intel_one_mono_8_pt;

    // Create menu system and store it in the global pointer
    menuSystem = new Kywy::MenuSystem(engine.display, menuItems, menuOptions);

    // Pass the engine instance to start the menu system
    menuSystem->start(engine);
}

void loop() {
    delay(1000);
}
