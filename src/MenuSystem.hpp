// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_MENU_SYSTEM_HPP
#define KYWY_MENU_SYSTEM_HPP

#include "Display.hpp"
#include "Font.hpp"
#include <vector>
#include <string>

namespace Kywy {

// Forward declaration
class Engine;

class MenuSystem {
public:
  // Define the types of menu items
  enum class MenuItemType {
    ACTION,   // Simple action item
    TOGGLE,   // Boolean toggle option
    SUBMENU,  // Submenu item
    OPTION,   // Option item
    LABEL,    // Header item
  };

  struct MenuItem {
    std::string label;
    MenuItemType type = MenuItemType::ACTION;
    bool *toggleable = nullptr;                                  // Pointer to a boolean for toggleable items
    std::function<void()> action = nullptr;                      // Function to call when selected
    std::string optionValue = "";                                // Value for OPTION type menu items
    std::function<std::string()> optionValueProvider = nullptr;  // Function to get current option value
    MenuSystem *submenu = nullptr;                               // Pointer to submenu for SUBMENU type
    bool expanded = false;                                       // Flag to track if a submenu is expanded

    // Constructor for action items (type defaults to ACTION)
    MenuItem(const std::string &label, bool *toggleable, const std::function<void()> &action)
      : label(label), toggleable(toggleable), action(action) {
      // Set the type based on the parameters provided
      if (toggleable != nullptr) {
        type = MenuItemType::TOGGLE;
      }
    }

    // Full constructor with all parameters
    MenuItem(const std::string &label, MenuItemType type, bool *toggleable,
             const std::function<void()> &action, const std::string &optionValue = "",
             const std::function<std::string()> &optionValueProvider = nullptr,
             MenuSystem *submenu = nullptr)
      : label(label), type(type), toggleable(toggleable), action(action),
        optionValue(optionValue), optionValueProvider(optionValueProvider),
        submenu(submenu), expanded(false) {
    }
  };

  // Helper functions for creating different types of menu items
  static MenuItem createAction(const std::string &label, const std::function<void()> &action) {
    return MenuItem(label, MenuItemType::ACTION, nullptr, action);
  }

  static MenuItem createToggle(const std::string &label, bool *toggleable, const std::function<void()> &action = nullptr) {
    return MenuItem(label, MenuItemType::TOGGLE, toggleable, action);
  }

  static MenuItem createLabel(const std::string &label) {
    return MenuItem(label, MenuItemType::LABEL, nullptr, nullptr);
  }

  static MenuItem createOption(const std::string &label, const std::string &optionValue,
                               const std::function<void()> &action = nullptr,
                               const std::function<std::string()> &optionValueProvider = nullptr) {
    return MenuItem(label, MenuItemType::OPTION, nullptr, action, optionValue, optionValueProvider);
  }

  static MenuItem createSubmenu(const std::string &label, MenuSystem *submenu) {
    // No action needed - the submenu will be toggled in selectOption
    return MenuItem(label, MenuItemType::SUBMENU, nullptr, nullptr, "", nullptr, submenu);
  }

  struct MenuOptions {
    MenuOptions()
      : x(10), y(10), itemHeight(10), pointer('>') {}
    int x = 0;
    int y = 8;
    int itemHeight = 12;
    char pointer = '>';  // Character to indicate the current selection
    uint8_t *font = Display::Font::intel_one_mono_8_pt;
    uint8_t *labelFont = Display::Font::intel_one_mono_8_pt;
  };

  struct ScrollOptions {
    int startIndex = 0;     // Index where the menu starts
    int visibleItems = 10;  // Number of items visible at a time

    ScrollOptions &setStartIndex(int index) {
      startIndex = index;
      return *this;
    }

    ScrollOptions &setVisibleItems(int count) {
      visibleItems = count;
      return *this;
    }
  };

  ScrollOptions scrollOptions;

  MenuSystem(Display::Display &display, const std::vector<MenuItem> &items, const MenuOptions &options = MenuOptions());

  void displayMenu();
  void nextOption();
  void previousOption();
  void selectOption();

  bool paused = false;

  void pause() {
    paused = true;
  }

  void unpause() {
    paused = false;
  }

  bool isPaused() const {
    return paused;
  }

  void pauseMenu();
  void unpauseMenu();
  bool isMenuPaused() const;

  // Collapse expanded submenus when back button is pressed
  void handleBackAction();

  void start(Kywy::Engine &engine);  // Start the menu system with an engine reference

  // Method to get the state of a toggleable menu item
  bool getToggleableState(size_t index) const {
    if (index < items.size() && items[index].toggleable) {
      return *items[index].toggleable;
    }
    return false;  // Return false if index is out of bounds or not toggleable
  }



private:
  struct FlatMenuItem {
    const MenuItem *item;  // Pointer to the actual menu item
    int indentLevel;       // Indentation level (0 for main menu, 1+ for submenu)
    int parentIndex;       // Index in the flattened list of the parent item (-1 for main items)
    bool isSubmenuItem;    // Whether this is a submenu item
    MenuItem *parentItem;  // Pointer to the parent menu item (nullptr for main items)

    FlatMenuItem(const MenuItem *item, int indentLevel, int parentIndex, bool isSubmenuItem, MenuItem *parentItem)
      : item(item), indentLevel(indentLevel), parentIndex(parentIndex),
        isSubmenuItem(isSubmenuItem), parentItem(parentItem) {}
  };

  Display::Display &display;
  MenuOptions options;
  size_t selectedIndex;                     // Index in main menu
  size_t flattenedSelectedIndex = 0;        // Index in flattened menu
  std::vector<FlatMenuItem> flattenedMenu;  // The flattened menu for display

  // Helper methods for submenu handling
  void buildFlattenedMenu();   // Build the flattened menu structure
  void syncSelectedIndices();  // Sync main menu index with flattened index

public:
  std::vector<MenuItem> items;
};

}  // namespace Kywy

#endif  // KYWY_MENU_SYSTEM_HPP
