// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MenuSystem.hpp"
#include "Actor.hpp"
#include "Events.hpp"
#include "Kywy.hpp"

namespace Kywy {

MenuSystem::MenuSystem(Display::Display &display, const std::vector<MenuItem> &items, const MenuOptions &options)
    : display(display), items(items), options(options), selectedIndex(0), flattenedSelectedIndex(0) {}

void MenuSystem::displayMenu() {
    if (paused) return;

    display.clear();
    int startY = options.y + 5;
    int indentWidth = 12; // Width in pixels for each indent level (increased for better visibility)

    // Build the flattened menu structure for display and navigation
    buildFlattenedMenu();

    // Calculate how many items we can display from the flattened menu
    int displayCount = std::min(scrollOptions.visibleItems, (int)flattenedMenu.size() - scrollOptions.startIndex);

    // Draw the visible portion of the flattened menu
    for (int i = 0; i < displayCount; ++i) {
        int itemIndex = scrollOptions.startIndex + i;
        if (itemIndex >= flattenedMenu.size()) break;

        const FlatMenuItem& flatItem = flattenedMenu[itemIndex];
        const MenuItem* item = flatItem.item;
        int indentLevel = flatItem.indentLevel;
        bool isSubmenuItem = flatItem.isSubmenuItem;

        if (!item) continue;

        // Check if this is the selected item in the flattened list
        bool isSelected = (itemIndex == flattenedSelectedIndex);

        // Prepare text with appropriate indentation and selection indicator
        std::string itemText;

        // Add selection indicator or indentation space
        itemText = isSelected ? std::string(1, options.pointer) : " ";

        // Add submenu prefix if this is a submenu item
        if (isSubmenuItem) {
            itemText += " ";
        }

        // Add the actual label
        itemText += item->label;

        int yPosition = startY + i * options.itemHeight;
        int xPosition = options.x + (indentLevel * indentWidth); // Apply indentation

        Display::TextOptions textOptions;
        textOptions._color = 0x00;
        textOptions._origin = Display::Origin::Text::BASELINE_LEFT;
        textOptions._font = options.font;

        // Handle menu item types for drawing
        switch (item->type) {
            case MenuItemType::TOGGLE: {
                bool toggleState = *(item->toggleable);
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
                if (item->optionValueProvider) {
                    itemText += item->optionValueProvider();
                } else {
                    itemText += item->optionValue;
                }
                break;
            case MenuItemType::SUBMENU:
                // Show different indicator based on expanded state
                itemText += item->expanded ? " V" : " >";
                break;
        }

        display.drawText(xPosition, yPosition, itemText.c_str(), textOptions);
    }

    display.update();
}

void MenuSystem::nextOption() {
    if (items.empty()) return;

    // Make sure the flattened menu is built and up to date
    buildFlattenedMenu();

    if (flattenedMenu.empty()) return;

    // Get the current selected item type to check if we should skip any entries
    bool wasOnLabel = false;
    if (flattenedSelectedIndex < flattenedMenu.size()) {
        const MenuItem* item = flattenedMenu[flattenedSelectedIndex].item;
        wasOnLabel = (item && item->type == MenuItemType::LABEL);
    }

    // Move to the next item in the flattened menu
    flattenedSelectedIndex = (flattenedSelectedIndex + 1) % flattenedMenu.size();

    // Skip label items since they can't be selected
    while (flattenedSelectedIndex < flattenedMenu.size() &&
           flattenedMenu[flattenedSelectedIndex].item &&
           flattenedMenu[flattenedSelectedIndex].item->type == MenuItemType::LABEL) {
        flattenedSelectedIndex = (flattenedSelectedIndex + 1) % flattenedMenu.size();
    }

    // Update the main menu selected index if we're on a main menu item
    syncSelectedIndices();

    // Handle scrolling
    if (flattenedSelectedIndex < scrollOptions.startIndex) {
        // Wrapped around to the beginning
        scrollOptions.startIndex = 0;
    } else if (flattenedSelectedIndex >= scrollOptions.startIndex + scrollOptions.visibleItems) {
        // Need to scroll down
        scrollOptions.startIndex = flattenedSelectedIndex - scrollOptions.visibleItems + 1;
    }
}

void MenuSystem::previousOption() {
    if (items.empty()) return;

    // Make sure the flattened menu is built and up to date
    buildFlattenedMenu();

    if (flattenedMenu.empty()) return;

    // Get the current selected item type to check if we should skip any entries
    bool wasOnLabel = false;
    if (flattenedSelectedIndex < flattenedMenu.size()) {
        const MenuItem* item = flattenedMenu[flattenedSelectedIndex].item;
        wasOnLabel = (item && item->type == MenuItemType::LABEL);
    }

    if (flattenedSelectedIndex == 0) {
        // Wrap to the end
        flattenedSelectedIndex = flattenedMenu.size() - 1;
    } else {
        flattenedSelectedIndex--;
    }

    // Skip label items since they can't be selected
    while (flattenedSelectedIndex < flattenedMenu.size() &&
           flattenedMenu[flattenedSelectedIndex].item &&
           flattenedMenu[flattenedSelectedIndex].item->type == MenuItemType::LABEL) {
        if (flattenedSelectedIndex == 0) {
            flattenedSelectedIndex = flattenedMenu.size() - 1;
        } else {
            flattenedSelectedIndex--;
        }
    }

    // Handle scrolling
    if (flattenedSelectedIndex < scrollOptions.startIndex) {
        // Need to scroll up
        scrollOptions.startIndex = flattenedSelectedIndex;
    } else if (flattenedSelectedIndex >= scrollOptions.startIndex + scrollOptions.visibleItems) {
        // Wrapped to the end, set scroll to show the last items
        if (flattenedMenu.size() > scrollOptions.visibleItems) {
            scrollOptions.startIndex = std::min(
                flattenedSelectedIndex - scrollOptions.visibleItems + 1,
                flattenedMenu.size() - scrollOptions.visibleItems
            );
        } else {
            scrollOptions.startIndex = 0;
        }
    }

    // Update the main menu selected index if we're on a main menu item
    syncSelectedIndices();
}

// Build the flattened menu structure that includes both main menu items and expanded submenu items
void MenuSystem::buildFlattenedMenu() {
    // Store the currently selected item's unique identification for later matching
    const MenuItem* previousSelectedItem = nullptr;
    bool wasInSubmenu = false;
    int submenuParentIndex = -1;

    if (!flattenedMenu.empty() && flattenedSelectedIndex < flattenedMenu.size()) {
        // Save the current selection information to restore it after rebuilding
        previousSelectedItem = flattenedMenu[flattenedSelectedIndex].item;
        wasInSubmenu = flattenedMenu[flattenedSelectedIndex].isSubmenuItem;
        submenuParentIndex = flattenedMenu[flattenedSelectedIndex].parentIndex;
    }

    // Clear and rebuild the flattened menu
    flattenedMenu.clear();
    int flatIndex = 0;
    bool foundPreviousSelection = false;

    // First pass: add all items to the flattened menu
    for (size_t i = 0; i < items.size(); ++i) {
        MenuItem& item = items[i];

        // Add the main menu item
        flattenedMenu.push_back(FlatMenuItem(&item, 0, -1, false, nullptr));

        // Check if this was our previously selected item
        if (!wasInSubmenu && previousSelectedItem == &item) {
            flattenedSelectedIndex = flatIndex;
            foundPreviousSelection = true;
        } else if (i == selectedIndex && !foundPreviousSelection) {
            // If we don't find our previously selected item, default to the main menu selection
            flattenedSelectedIndex = flatIndex;
        }

        int parentIndex = flatIndex; // Remember this index for submenu items
        flatIndex++;

        // If this is an expanded submenu, add its items
        if (item.type == MenuItemType::SUBMENU && item.expanded && item.submenu) {
            for (size_t j = 0; j < item.submenu->items.size(); ++j) {
                MenuItem& subItem = item.submenu->items[j];
                flattenedMenu.push_back(FlatMenuItem(&subItem, 1, parentIndex, true, &item));

                // Check if this was our previously selected submenu item
                if (wasInSubmenu && previousSelectedItem == &subItem && submenuParentIndex == parentIndex) {
                    flattenedSelectedIndex = flatIndex;
                    foundPreviousSelection = true;
                }

                flatIndex++;
            }
        }
    }

    // Safety check for bounds
    if (flattenedMenu.empty()) {
        flattenedSelectedIndex = 0;
    } else if (flattenedSelectedIndex >= flattenedMenu.size()) {
        flattenedSelectedIndex = flattenedMenu.size() - 1;
    }
}

// Sync the main menu selectedIndex with the flattened menu flattenedSelectedIndex
void MenuSystem::syncSelectedIndices() {
    if (flattenedSelectedIndex >= flattenedMenu.size()) {
        flattenedSelectedIndex = flattenedMenu.size() - 1;
    }

    const FlatMenuItem& selected = flattenedMenu[flattenedSelectedIndex];

    // If selected item is a main menu item, update selectedIndex
    if (!selected.isSubmenuItem) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (&items[i] == selected.item) {
                selectedIndex = i;
                break;
            }
        }
    }
}

// Handle menu item types on selection
void MenuSystem::selectOption() {
    // Make sure the flattened menu is built
    buildFlattenedMenu();

    if (flattenedMenu.empty() || flattenedSelectedIndex >= flattenedMenu.size()) {
        return;
    }

    // Get the selected item from the flattened menu
    const FlatMenuItem& flatItem = flattenedMenu[flattenedSelectedIndex];
    const MenuItem* itemPtr = flatItem.item;

    if (!itemPtr) return;

    // If this is a submenu item, we need to handle it differently
    if (flatItem.isSubmenuItem) {
        // Handle submenu item selection based on its type
        switch (itemPtr->type) {
            case MenuItemType::TOGGLE:
                if (itemPtr->toggleable) {
                    *(itemPtr->toggleable) = !*(itemPtr->toggleable);
                }
                break;
            case MenuItemType::LABEL:
                return;
            case MenuItemType::OPTION:
                // For OPTION items, execute the action
                if (itemPtr->action) {
                    pause();
                    itemPtr->action();
                    unpause();
                    buildFlattenedMenu(); // Rebuild in case menu structure changed
                    displayMenu();
                }
                return;
            case MenuItemType::SUBMENU:
                // Nested submenus not supported in this version
                break;
            case MenuItemType::ACTION:
            default:
                if (itemPtr->action) {
                    pause();
                    itemPtr->action();
                    unpause();
                }
                break;
        }
        return;
    }

    // Handle main menu item selection
    MenuItem& item = items[selectedIndex]; // Use the synchronized selectedIndex

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
                buildFlattenedMenu(); // Rebuild in case menu structure changed
                displayMenu();
            }
            return;
        case MenuItemType::SUBMENU:
            // Toggle submenu expansion
            if (item.submenu) {
                item.expanded = !item.expanded;
                // No need to call action, just redraw with expanded/collapsed submenu
                buildFlattenedMenu(); // Rebuild with new expanded state
                displayMenu();
                return;
            }
            break;
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

void MenuSystem::handleBackAction() {
    // Make sure flattened menu is built
    buildFlattenedMenu();

    if (flattenedMenu.empty()) return;

    // Get the currently selected item
    if (flattenedSelectedIndex >= flattenedMenu.size()) return;

    const FlatMenuItem& flatItem = flattenedMenu[flattenedSelectedIndex];

    // If we're in a submenu, get the parent item and collapse it
    if (flatItem.isSubmenuItem && flatItem.parentItem) {
        // We're in a submenu - collapse it
        flatItem.parentItem->expanded = false;

        // Find the parent menu item in the flattened menu
        for (size_t i = 0; i < items.size(); i++) {
            if (&items[i] == flatItem.parentItem) {
                // Find this item in the flattened menu
                for (size_t j = 0; j < flattenedMenu.size(); j++) {
                    if (flattenedMenu[j].item == &items[i] && !flattenedMenu[j].isSubmenuItem) {
                        flattenedSelectedIndex = j;
                        syncSelectedIndices();
                        break;
                    }
                }
                break;
            }
        }
    } else {
        // We're in the main menu - check if there's any expanded submenu to collapse
        bool anyCollapsed = false;
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].type == MenuItemType::SUBMENU && items[i].expanded) {
                items[i].expanded = false;
                anyCollapsed = true;
            }
        }

        // If we didn't collapse anything, we could add more behavior here
        // like closing the menu or going back to a previous screen
    }

    // Rebuild the flattened menu structure
    buildFlattenedMenu();
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
            case Kywy::Events::BUTTON_LEFT_PRESSED:
                // Handle back action - collapse submenus or go back
                menu.handleBackAction();
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

    // Build the flattened menu before displaying
    buildFlattenedMenu();
    displayMenu();
}

} // namespace Kywy
