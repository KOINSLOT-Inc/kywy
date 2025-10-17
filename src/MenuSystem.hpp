// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_MENU_SYSTEM_HPP
#define KYWY_MENU_SYSTEM_HPP

#include "Display.hpp"
#include "Font.hpp"
#include "Scene.hpp"
#include <vector>
#include <string>

namespace Kywy {

// Forward declarations
class Engine;
class MenuInputHandler;

class MenuSystem {
public:
  // Define the types of menu items
  enum class MenuItemType {
    ACTION,   // Simple action item
    TOGGLE,   // Boolean toggle option
    SUBMENU,  // Submenu item
    OPTION,   // Option item
    LABEL,    // Header item
    SCENE,    // Scene launch item
  };

  struct MenuItem {
    std::string label;
    MenuItemType type = MenuItemType::ACTION;
    bool *toggleable = nullptr;                                  // Pointer to a boolean for toggleable items
    std::function<void()> action = nullptr;                      // Function to call when selected
    std::string optionValue = "";                                // Value for OPTION type menu items
    std::function<std::string()> optionValueProvider = nullptr;  // Function to get current option value
    MenuSystem *submenu = nullptr;                               // Pointer to submenu for SUBMENU type
    Scene *scene = nullptr;                                      // Pointer to scene for SCENE type
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
             MenuSystem *submenu = nullptr, Scene *scene = nullptr)
      : label(label), type(type), toggleable(toggleable), action(action),
        optionValue(optionValue), optionValueProvider(optionValueProvider),
        submenu(submenu), scene(scene), expanded(false) {
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

  // Simple scene helper - just provide the scene and the menu handles everything
  static MenuItem createSceneItem(const std::string &label, Scene *scene) {
    MenuItem item(label, MenuItemType::SCENE, nullptr, nullptr, "", nullptr, nullptr, scene);
    return item;
  }

  // Enhanced scene helper - creates and adds scene item to menu automatically
  void addSceneItem(const std::string &label, Scene *scene) {
    items.push_back(createSceneItem(label, scene));
    menuDirty = true;  // Mark menu for rebuild
  }

  // Template helper - creates scene instance and adds to menu in one call
  template<typename SceneType>
  void createAndAddScene(const std::string &label) {
    SceneType* scene = new SceneType();
    items.push_back(createSceneItem(label, scene));
    menuDirty = true;  // Mark menu for rebuild
  }

  // Enhanced menu item adding methods
  void addItem(const MenuItem &item) {
    items.push_back(item);
    menuDirty = true;  // Mark menu for rebuild
  }

  void addActionItem(const std::string &label, const std::function<void()> &action) {
    items.push_back(createAction(label, action));
    menuDirty = true;  // Mark menu for rebuild
  }

  void addToggleItem(const std::string &label, bool *toggleable, const std::function<void()> &action = nullptr) {
    items.push_back(createToggle(label, toggleable, action));
    menuDirty = true;  // Mark menu for rebuild
  }

  void addLabelItem(const std::string &label) {
    items.push_back(createLabel(label));
    menuDirty = true;  // Mark menu for rebuild
  }

  struct MenuOptions {
    MenuOptions()
      : x(0), y(10), itemHeight(12), pointer('>') {}  // Changed x from 10 to 0 for far left
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

  // Default constructor with empty menu items
  MenuSystem(Display::Display &display, const MenuOptions &options = MenuOptions())
    : MenuSystem(display, {}, options) {}

  MenuSystem(Display::Display &display, const std::vector<MenuItem> &items, const MenuOptions &options = MenuOptions());

  void displayMenu();
  void nextOption();
  void previousOption();
  void selectOption();

  // Scene state management
  void enterScene(Scene* scene);
  void exitScene();
  void onSceneExit();
  Scene* getCurrentScene() const { return currentScene; }
  bool isInScene() const { return currentScene != nullptr; }

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
  
  // Scene management
  Scene* currentScene = nullptr;            // Currently active scene
  Kywy::Engine* engine = nullptr;           // Engine reference for scene management
  
  // Performance optimization
  MenuInputHandler* inputHandler = nullptr; // Single input handler for performance
  bool menuDirty = true;                    // Flag to track if menu needs rebuilding

  // Helper methods for submenu handling
  void buildFlattenedMenu();   // Build the flattened menu structure
  void syncSelectedIndices();  // Sync main menu index with flattened index
  
  std::vector<MenuItem> items;  // The menu items
};

}  // namespace Kywy

#endif  // KYWY_MENU_SYSTEM_HPP
