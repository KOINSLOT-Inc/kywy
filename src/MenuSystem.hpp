#ifndef KYWY_MENU_SYSTEM_HPP
#define KYWY_MENU_SYSTEM_HPP

#include "Display.hpp"
#include "Kywy.hpp" // Include Kywy header for Kywy::Engine type
#include <vector>
#include <string>
#include <functional>

namespace Kywy {

class MenuSystem {
public:
    struct MenuItem {
        std::string label;
        bool *toggleable = nullptr; // Pointer to a boolean for toggleable items
        std::function<void()> action; // Function to call when selected

        MenuItem(const std::string &label, bool *toggleable, const std::function<void()> &action)
            : label(label), toggleable(toggleable), action(action) {}
    };

    struct MenuOptions {
        MenuOptions() : x(10), y(10), itemHeight(10), pointer('>') {}
        int x;
        int y;
        int itemHeight;
        char pointer; // Character to indicate the current selection
    };

    struct TextOptions {
        uint16_t color = 0x00;
        uint8_t *font = nullptr;
        bool opaque = false;

        TextOptions &setColor(uint16_t newColor) {
            color = newColor;
            return *this;
        }

        TextOptions &setFont(uint8_t *newFont) {
            font = newFont;
            return *this;
        }

        TextOptions &setOpaque(bool isOpaque) {
            opaque = isOpaque;
            return *this;
        }
    };

    struct ScrollOptions {
        int startIndex = 0; // Index where the menu starts
        int visibleItems = 5; // Number of items visible at a time

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

    void start(Kywy::Engine &engine); // Start the menu system with an engine reference

    // Method to get the state of a toggleable menu item
    bool getToggleableState(size_t index) const {
        if (index < items.size() && items[index].toggleable) {
            return *items[index].toggleable;
        }
        return false; // Return false if index is out of bounds or not toggleable
    }

private:
    Display::Display &display;
    std::vector<MenuItem> items;
    MenuOptions options;
    size_t selectedIndex;
};

} // namespace Kywy

#endif // KYWY_MENU_SYSTEM_HPP