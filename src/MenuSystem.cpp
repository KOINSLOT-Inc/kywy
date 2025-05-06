// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MenuSystem.hpp"
#include "Actor.hpp"
#include "Events.hpp"
#include "Kywy.hpp"

namespace Kywy {

MenuSystem::MenuSystem(Display::Display &display, const std::vector<MenuItem> &items, const MenuOptions &options)
    : display(display), items(items), options(options), selectedIndex(0) {}

void MenuSystem::displayMenu() {
    if (paused) return;

    display.clear();
    int startY = options.y + 5;

    for (int i = 0; i < scrollOptions.visibleItems; ++i) {
        int itemIndex = scrollOptions.startIndex + i;
        if (itemIndex >= items.size()) break;

        const MenuItem& item = items[itemIndex];
        bool isSelected = (itemIndex == selectedIndex);

        std::string itemText = (isSelected ? std::string(1, options.pointer) : " ") + item.label;
        int yPosition = startY + i * options.itemHeight;

        Display::TextOptions textOptions;
        textOptions._color = 0x00;
        textOptions._origin = Display::Origin::Text::BASELINE_LEFT;
        textOptions._font = options.font;

        // Handle menu item types for drawing
        switch (item.type) {
            case MenuItemType::TOGGLE: {
                bool toggleState = *item.toggleable;
                itemText += toggleState ? " [X]" : " [ ]";
                break;
            }
            case MenuItemType::ACTION:
            default:
                break;
            case MenuItemType::LABEL:
                itemText += " ";
                textOptions._font = options.labelFont;
                break;
            case MenuItemType::OPTION:
                itemText += " : ";
                // Use optionValueProvider if available, otherwise use the static optionValue
                if (item.optionValueProvider) {
                    itemText += item.optionValueProvider();
                } else {
                    itemText += item.optionValue;
                }
                break;
            case MenuItemType::SUBMENU:
                itemText += " >";
                break;
        }

        display.drawText(options.x, yPosition, itemText.c_str(), textOptions);
    }

    display.update();
}

void MenuSystem::nextOption() {
    if (items.empty()) return;

    selectedIndex = (selectedIndex + 1) % items.size();

    if (selectedIndex == 0) {
        scrollOptions.startIndex = 0;
    } else if (selectedIndex >= scrollOptions.startIndex + scrollOptions.visibleItems) {
        if (scrollOptions.startIndex + scrollOptions.visibleItems < items.size()) {
            scrollOptions.startIndex++;
        }
    }
}

void MenuSystem::previousOption() {
    if (items.empty()) return;

    if (selectedIndex == 0) {
        selectedIndex = items.size() - 1;
        scrollOptions.startIndex = std::max(0, static_cast<int>(items.size()) - scrollOptions.visibleItems);
    } else {
        selectedIndex--;
        if (selectedIndex < scrollOptions.startIndex) {
            if (scrollOptions.startIndex > 0) {
                scrollOptions.startIndex--;
            }
        }
    }
}

// Handle menu item types on selection
void MenuSystem::selectOption() {
    MenuItem& item = items[selectedIndex];

    switch (item.type) {
        case MenuItemType::TOGGLE:
            if (item.toggleable) {
                *item.toggleable = !(*item.toggleable);
            }
            break;
        case MenuItemType::LABEL:
            return;
        case MenuItemType::OPTION:
            // For OPTION items, execute the action first
            if (item.action) {
                pause();
                item.action();
                unpause();
                // Redraw the menu to show the updated option value
                displayMenu();
            }
            return;
        case MenuItemType::SUBMENU:
        case MenuItemType::ACTION:
        default:
            break;
    }

    if (item.action) {
        pause();
        item.action();
        unpause();
    }
}

void MenuSystem::pauseMenu() {
    pause();
}

void MenuSystem::unpauseMenu() {
    unpause();
}

bool MenuSystem::isMenuPaused() const {
    return paused;
}



class MenuInputHandler : public Actor::Actor {
public:
    MenuInputHandler(MenuSystem &menu, Kywy::Engine &engine)
        : menu(menu), engine(engine), rightButtonPressed(false) {}

    void handle(::Actor::Message *message) {
        if (menu.isMenuPaused()) {
            return;
        }

        switch (message->signal) {
            case Kywy::Events::D_PAD_UP_PRESSED:
                menu.previousOption();
                menu.displayMenu();
                break;
            case Kywy::Events::D_PAD_DOWN_PRESSED:
                menu.nextOption();
                menu.displayMenu();
                break;
            case Kywy::Events::BUTTON_RIGHT_PRESSED:
                menu.selectOption();
                menu.displayMenu();
                break;
        }
    }



private:
    MenuSystem &menu;
    Kywy::Engine &engine;
    bool rightButtonPressed;
};

void MenuSystem::start(Kywy::Engine &engine) {
    auto inputHandler = new MenuInputHandler(*this, engine);
    inputHandler->subscribe(&engine.input);
    inputHandler->start();

    displayMenu();
}

} // namespace Kywy
