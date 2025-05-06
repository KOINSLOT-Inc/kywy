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

        std::string itemText = (itemIndex == selectedIndex ? std::string(1, options.pointer) + " " : "  ") + items[itemIndex].label;
        if (items[itemIndex].toggleable) {
            bool toggleState = *items[itemIndex].toggleable;
            itemText += toggleState ? " [X]" : " [ ]";
        }
        int yPosition = startY + i * options.itemHeight;

        Display::TextOptions textOptions;
        textOptions._color = 0x00;
        textOptions._origin = Display::Origin::Text::BASELINE_LEFT;
        textOptions._font = options.font;

        display.drawText(options.x, yPosition, itemText.c_str(), textOptions);
    }

    display.update();
}

void MenuSystem::nextOption() {
    if (items.empty()) return;

    selectedIndex = (selectedIndex + 1) % items.size();
    if (selectedIndex >= scrollOptions.startIndex + scrollOptions.visibleItems) {
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

void MenuSystem::selectOption() {
    if (items[selectedIndex].toggleable) {
        bool currentState = *items[selectedIndex].toggleable;
        *items[selectedIndex].toggleable = !currentState;
    }

    if (items[selectedIndex].action) {
        pause();
        items[selectedIndex].action();
        unpause();
    }
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
    MenuInputHandler(MenuSystem &menu, Kywy::Engine &engine)
        : menu(menu), engine(engine), rightButtonPressed(false) {}

    void handle(::Actor::Message *message) override {
        if (menu.isPaused()) {
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
                if (!rightButtonPressed) {
                    rightButtonPressed = true;
                    menu.selectOption();
                    menu.displayMenu();
                }
                break;
            case Kywy::Events::BUTTON_RIGHT_RELEASED:
                rightButtonPressed = false;
                break;
        }
    }

private:
    MenuSystem &menu;
    Kywy::Engine &engine;
    bool rightButtonPressed;
};

void MenuSystem::start(Kywy::Engine &engine) {
    // Create and start the input handler
    auto inputHandler = new MenuInputHandler(*this, engine);
    inputHandler->subscribe(&engine.input);
    inputHandler->start();

    // Display the menu immediately
    displayMenu();
}

} // namespace Kywy
