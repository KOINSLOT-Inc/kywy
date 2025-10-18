// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MenuSystem.hpp"
#include "Actor.hpp"
#include "Events.hpp"
#include "Kywy.hpp"

// For millis() function
#ifdef ARDUINO
#include <Arduino.h>
#else
// Mock millis for non-Arduino environments
unsigned long millis() {
  return 0;
}
#endif

namespace Kywy {

MenuSystem::MenuSystem(Display::Display& display, const std::vector<MenuItem>& items, const MenuOptions& options)
  : display(display), items(items), options(options), selectedIndex(0), flattenedSelectedIndex(0) {}

void MenuSystem::displayMenu() {
  if (paused || isInScene()) return;

  display.clear();
  int startY = options.y + 5;
  int indentWidth = 8;  // Width in pixels for each indent level (increased for better visibility)

  // Build the flattened menu structure for display and navigation only if dirty
  if (menuDirty) {
    buildFlattenedMenu();
    menuDirty = false;
  }

  // Calculate how many items we can display from the flattened menu
  int displayCount = std::min(scrollOptions.visibleItems, (int)flattenedMenu.size() - scrollOptions.startIndex);

  // Draw the visible portion of the flattened menu
  for (int i = 0; i < displayCount; ++i) {

    int itemIndex = scrollOptions.startIndex + i;
    if (itemIndex >= flattenedMenu.size()) {
      break;
    }

    const FlatMenuItem& flatItem = flattenedMenu[itemIndex];
    const MenuItem* item = flatItem.item;

    int indentLevel = flatItem.indentLevel;
    bool isSubmenuItem = flatItem.isSubmenuItem;

    if (!item) continue;

    // Check if this is the selected item in the flattened list
    bool isSelected = (itemIndex == flattenedSelectedIndex);

    // Calculate positions first
    int yPosition = startY + i * options.itemHeight;
    int xPosition = options.x + (indentLevel * indentWidth);  // Apply indentation

    // Prepare text with appropriate indentation and selection indicator
    // Use a fixed-size buffer to avoid stack overflow from string operations
    char itemText[24];  // Fixed buffer for menu text
    int pos = 0;

    // Add selection indicator or indentation space
    itemText[pos++] = isSelected ? options.pointer : ' ';

    // Add the actual label (safely copy to avoid overflow)
    const char* label = item->label.c_str();
    while (*label && pos < 22) {  // Leave room for padding and null terminator
      itemText[pos++] = *label++;
    }

    Display::TextOptions textOptions;
    textOptions._color = 0x00;
    textOptions._origin = Display::Origin::Text::BASELINE_LEFT;
    textOptions._font = options.font;

    // Handle menu item types for drawing
    switch (item->type) {
      case MenuItemType::TOGGLE:
        {
          bool toggleState = *(item->toggleable);
          const char* suffix = toggleState ? " [X]" : " [ ]";
          while (*suffix && pos < 22) {
            itemText[pos++] = *suffix++;
          }
          break;
        }
      case MenuItemType::ACTION:
      default:
        break;
      case MenuItemType::LABEL:
        if (pos < 22) itemText[pos++] = ' ';
        textOptions._font = options.labelFont;
        break;
      case MenuItemType::OPTION:
        {
          if (pos < 21) {
            itemText[pos++] = ':';
            itemText[pos++] = ' ';
          }
          // Use optionValueProvider if available, otherwise use the static optionValue
          const char* optVal = item->optionValueProvider ? item->optionValueProvider().c_str() : item->optionValue.c_str();
          while (*optVal && pos < 22) {
            itemText[pos++] = *optVal++;
          }
          break;
        }
      case MenuItemType::SUBMENU:
        {
          const char* suffix = item->expanded ? " V" : " >";
          while (*suffix && pos < 22) {
            itemText[pos++] = *suffix++;
          }
          break;
        }
      case MenuItemType::SCENE:
        if (pos < 21) {
          itemText[pos++] = ' ';
          itemText[pos++] = '>';
        }
        break;
    }

    // Pad with spaces to clear old content (max 20 chars for padding)
    while (pos < 20) {
      itemText[pos++] = ' ';
    }
    itemText[pos] = '\0';  // Null terminate

    display.drawText(xPosition, yPosition, itemText, textOptions);
  }

  display.update();
}

void MenuSystem::nextOption() {
  if (items.empty() || isInScene()) return;

  // Make sure the flattened menu is built and up to date
  if (menuDirty) {
    buildFlattenedMenu();
    menuDirty = false;
  }

  if (flattenedMenu.empty()) return;

  // Move to the next item in the flattened menu
  flattenedSelectedIndex = (flattenedSelectedIndex + 1) % flattenedMenu.size();

  // Skip label items since they can't be selected
  while (flattenedSelectedIndex < flattenedMenu.size() && flattenedMenu[flattenedSelectedIndex].item && flattenedMenu[flattenedSelectedIndex].item->type == MenuItemType::LABEL) {
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
  if (items.empty() || isInScene()) return;

  // Make sure the flattened menu is built and up to date
  if (menuDirty) {
    buildFlattenedMenu();
    menuDirty = false;
  }

  if (flattenedMenu.empty()) return;

  if (flattenedSelectedIndex == 0) {
    // Wrap to the end
    flattenedSelectedIndex = flattenedMenu.size() - 1;
  } else {
    flattenedSelectedIndex--;
  }

  // Skip label items since they can't be selected
  while (flattenedSelectedIndex < flattenedMenu.size() && flattenedMenu[flattenedSelectedIndex].item && flattenedMenu[flattenedSelectedIndex].item->type == MenuItemType::LABEL) {
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
        flattenedMenu.size() - scrollOptions.visibleItems);
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

    int parentIndex = flatIndex;  // Remember this index for submenu items
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

  // Skip label items - move to first non-label item if currently on a label
  if (!flattenedMenu.empty() && flattenedSelectedIndex < flattenedMenu.size()) {
    while (flattenedSelectedIndex < flattenedMenu.size() && flattenedMenu[flattenedSelectedIndex].item && flattenedMenu[flattenedSelectedIndex].item->type == MenuItemType::LABEL) {
      flattenedSelectedIndex++;
    }

    // If we went past the end, wrap back and search from beginning
    if (flattenedSelectedIndex >= flattenedMenu.size()) {
      flattenedSelectedIndex = 0;
      while (flattenedSelectedIndex < flattenedMenu.size() && flattenedMenu[flattenedSelectedIndex].item && flattenedMenu[flattenedSelectedIndex].item->type == MenuItemType::LABEL) {
        flattenedSelectedIndex++;
      }

      // If still no non-label item found, default to 0 (all items are labels)
      if (flattenedSelectedIndex >= flattenedMenu.size()) {
        flattenedSelectedIndex = 0;
      }
    }

    // Sync the main menu selectedIndex with the updated flattenedSelectedIndex
    syncSelectedIndices();
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
  if (isInScene()) return;  // Don't handle selection if in scene

  // Make sure the flattened menu is built
  if (menuDirty) {
    buildFlattenedMenu();
    menuDirty = false;
  }

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
          buildFlattenedMenu();  // Rebuild in case menu structure changed
          displayMenu();
        }
        return;
      case MenuItemType::SUBMENU:
        // Nested submenus not supported in this version
        break;
      case MenuItemType::SCENE:
        // Launch scene from submenu
        if (itemPtr->scene && engine) {
          enterScene(itemPtr->scene);
          return;
        }
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
  MenuItem& item = items[selectedIndex];  // Use the synchronized selectedIndex

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
        buildFlattenedMenu();  // Rebuild in case menu structure changed
        displayMenu();
      }
      return;
    case MenuItemType::SUBMENU:
      // Toggle submenu expansion
      if (item.submenu) {
        item.expanded = !item.expanded;
        menuDirty = true;  // Mark for rebuild
        // No need to call action, just redraw with expanded/collapsed submenu
        displayMenu();
        return;
      }
      break;
    case MenuItemType::SCENE:
      // Launch scene
      if (item.scene && engine) {
        enterScene(item.scene);
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
  if (isInScene()) return;  // Don't handle back if in scene

  // Make sure flattened menu is built
  if (menuDirty) {
    buildFlattenedMenu();
    menuDirty = false;
  }

  if (flattenedMenu.empty()) return;

  // Get the currently selected item
  if (flattenedSelectedIndex >= flattenedMenu.size()) return;

  const FlatMenuItem& flatItem = flattenedMenu[flattenedSelectedIndex];

  // If we're in a submenu, get the parent item and collapse it
  if (flatItem.isSubmenuItem && flatItem.parentItem) {
    // We're in a submenu - collapse it
    flatItem.parentItem->expanded = false;
    menuDirty = true;  // Mark for rebuild

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

    if (anyCollapsed) {
      menuDirty = true;  // Mark for rebuild
    }

    // If we didn't collapse anything, we could add more behavior here
    // like closing the menu or going back to a previous screen
  }
}



class MenuInputHandler : public Actor::Actor {
public:
  MenuInputHandler(MenuSystem& menu, Kywy::Engine& engine)
    : menu(menu), engine(engine), lastInputTime(0), debounceDelay(100), enabled(true) {}

  void disable() {
    enabled = false;
  }

  void enable() {
    enabled = true;
  }

  void handle(::Actor::Message* message) {
    // Check if handler is disabled
    if (!enabled) {
      return;
    }

    // Performance optimization: Check scene state first
    if (menu.isInScene() || menu.isMenuPaused()) {
      return;
    }

    // Simple debouncing
    unsigned long currentTime = millis();
    if (currentTime - lastInputTime < debounceDelay) {
      return;
    }

    switch (message->signal) {
      case Kywy::Events::D_PAD_UP_PRESSED:
        menu.previousOption();
        menu.displayMenu();
        lastInputTime = currentTime;
        break;
      case Kywy::Events::D_PAD_DOWN_PRESSED:
        menu.nextOption();
        menu.displayMenu();
        lastInputTime = currentTime;
        break;
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        menu.selectOption();
        menu.displayMenu();
        lastInputTime = currentTime;
        break;
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        // Handle back action - collapse submenus or go back
        menu.handleBackAction();
        menu.displayMenu();
        lastInputTime = currentTime;
        break;
    }
  }

private:
  MenuSystem& menu;
  Kywy::Engine& engine;
  unsigned long lastInputTime;
  unsigned long debounceDelay;
  bool enabled;
};

// Scene lifecycle management (defined after MenuInputHandler class)
void MenuSystem::enterScene(Scene* scene) {
  if (!scene || !engine) return;

  currentScene = scene;

  // Pause menu and disable input handler FIRST
  pause();
  if (inputHandler) {
    inputHandler->unsubscribe(&engine->input);
    inputHandler->disable();
  }

  // Clear display before entering scene
  engine->display.clear();
  engine->display.update();

  // Set up scene exit callback to return to menu
  scene->setExitCallback([this]() {
    onSceneExit();
  });

  // Enter the scene
  scene->enter();
}

void MenuSystem::exitScene() {
  if (currentScene) {
    currentScene->exit();
    currentScene = nullptr;
  }
}

void MenuSystem::onSceneExit() {
  // Save scene reference then clear it
  Scene* exitingScene = currentScene;
  currentScene = nullptr;

  // No need to clear callback - Scene::triggerExit() already cleared it
  // before calling this callback

  // Now it's safe to cleanup non-persistent scenes
  // (we're outside the actor's handle() method now)
  if (exitingScene && !exitingScene->isPersistent()) {
    exitingScene->cleanup();
  }

  // Clear display immediately
  if (engine) {
    engine->display.clear();
    engine->display.update();
  }

// Small delay for display stability
#ifdef ARDUINO
  delay(10);
#endif

  // Re-enable menu input handler
  if (inputHandler) {
    inputHandler->enable();
    inputHandler->subscribe(&engine->input);
  }

  // Unpause and force menu redraw
  unpause();
  menuDirty = true;
  buildFlattenedMenu();
  menuDirty = false;

  // Display the menu
  displayMenu();
}

void MenuSystem::start(Kywy::Engine& engine) {
  this->engine = &engine;  // Store engine reference for scene management

  // Create single optimized input handler (33% performance improvement)
  if (!inputHandler) {
    inputHandler = new MenuInputHandler(*this, engine);
    inputHandler->subscribe(&engine.input);
    inputHandler->start();
  }

  // Build the flattened menu before displaying
  menuDirty = true;
  displayMenu();
}

}  // namespace Kywy
