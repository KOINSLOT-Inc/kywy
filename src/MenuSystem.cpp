#include "MenuSystem.hpp"
#include "Display.hpp"
#include "Actor.hpp" // Include Actor header for Actor::Actor class
#include "Events.hpp" // Include Events header for Kywy::Events namespace
#include "Kywy.hpp" // Include Kywy header for Kywy::Engine type

namespace Kywy {

MenuSystem::MenuSystem(Display::Display &display, const std::vector<MenuItem> &items, const MenuOptions &options)
    : display(display), items(items), options(options), selectedIndex(0) {}

void MenuSystem::displayMenu() {
    if (paused) return; // Skip rendering if the menu is paused

    display.clear();
    int totalHeight = scrollOptions.visibleItems * options.itemHeight;
    int startY = options.y + 5; // Offset the menu slightly down from the top of the display

    for (int i = 0; i < scrollOptions.visibleItems; ++i) {
        int itemIndex = scrollOptions.startIndex + i;
        if (itemIndex >= items.size()) break;

        std::string itemText = (itemIndex == selectedIndex ? std::string(1, options.pointer) + " " : "  ") + items[itemIndex].label;
        if (items[itemIndex].toggleable) {
            // Check the value of the toggleable boolean when drawing
            bool toggleState = *items[itemIndex].toggleable;
            Serial.println("Drawing toggleable state: " + String(toggleState)); // Debug log
            itemText += toggleState ? " [x]" : " [ ]";
        }
        int yPosition = startY + i * options.itemHeight;

        // Adjust origin for better alignment of checkboxes
        Display::TextOptions textOptions;
        textOptions._color = 0x00; // Set text color to black directly without using Display::BLACK
        textOptions._origin = Display::Origin::Text::BASELINE_LEFT; // Directly set the origin

        display.drawText(options.x, yPosition, itemText.c_str(), textOptions);
    }
    display.update();
}

void MenuSystem::previousOption() {
    if (items.empty()) return; // Prevent crash if no items exist

    selectedIndex = (selectedIndex + 1) % items.size();
    if (selectedIndex >= scrollOptions.startIndex + scrollOptions.visibleItems) {
        if (scrollOptions.startIndex + scrollOptions.visibleItems < items.size()) {
            scrollOptions.startIndex++;
        }
    }
}

void MenuSystem::nextOption() {
    if (items.empty()) return; // Prevent crash if no items exist

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

void MenuSystem::selectOption() {
    if (items[selectedIndex].toggleable) {
        // Toggle the state directly without using a static variable
        *items[selectedIndex].toggleable = !*items[selectedIndex].toggleable;
        Serial.println("Toggleable state updated: " + String(*items[selectedIndex].toggleable)); // Debug log
    }
    if (items[selectedIndex].action) {
        pause(); // Automatically pause the menu during the action
        items[selectedIndex].action(); // Execute the action
        unpause(); // Automatically unpause the menu after the action
    }
    displayMenu(); // Ensure the menu is updated after toggling
}

void MenuSystem::pauseMenu() {
    paused = true;
}

void MenuSystem::unpauseMenu() {
    paused = false;
}

bool MenuSystem::isMenuPaused() const {
    return paused;
}

class MenuInputHandler : public Actor::Actor {
public:
    MenuInputHandler(MenuSystem &menu) : menu(menu) {}

    void handle(::Actor::Message *message) override {
        switch (message->signal) {
            case Kywy::Events::D_PAD_UP_PRESSED:
                menu.previousOption();
                break;
            case Kywy::Events::D_PAD_DOWN_PRESSED:
                menu.nextOption();
                break;
            case Kywy::Events::BUTTON_RIGHT_PRESSED:
                menu.selectOption();
                break;
            default:
                break;
        }
    }

private:
    MenuSystem &menu;
};

class MenuActor : public Actor::Actor {
public:
    MenuActor(MenuSystem &menu, Kywy::Engine &engine)
        : menu(menu), engine(engine), displayingMessage(false), messageStartTime(0) {}

    void handle(::Actor::Message *message) override {
        if (displayingMessage) {
            if (millis() - messageStartTime >= messageDisplayDuration) {
                displayingMessage = false;
                menu.unpause();
                menu.displayMenu();
            }
        } else {
            if (menu.isPaused()) {
                return; // Skip processing while the menu is paused
            }

            switch (message->signal) {
                case Kywy::Events::D_PAD_UP_PRESSED:
                    menu.previousOption();
                    break;
                case Kywy::Events::D_PAD_DOWN_PRESSED:
                    menu.nextOption();
                    break;
                case Kywy::Events::BUTTON_RIGHT_PRESSED:
                    menu.selectOption();
                    break;
                case Kywy::Events::MENU_MESSAGE:
                    displayMessage("Action executed!");
                    break;
                default:
                    break;
            }

            menu.displayMenu();
        }
    }

    void displayMessage(const char *message) {
        menu.pause();
        engine.display.clear();
        engine.display.drawText(10, 10, message, Display::TextOptions().color(0x00));
        engine.display.update();
        displayingMessage = true;
        messageStartTime = millis();
    }

private:
    MenuSystem &menu;
    Kywy::Engine &engine;
    bool displayingMessage;
    unsigned long messageStartTime;
    const unsigned long messageDisplayDuration = 2000; // 2 seconds
};

void MenuSystem::start(Kywy::Engine &engine) {
    // Create and start the input handler
    auto inputHandler = new MenuInputHandler(*this);
    inputHandler->subscribe(&engine.input);
    inputHandler->start();

    // Create and start the menu actor
    auto menuActor = new MenuActor(*this, engine);
    menuActor->subscribe(&engine.input);
    menuActor->start();

    // Display the menu immediately
    displayMenu();
}

} // namespace Kywy