// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"
#include "MenuSystem.hpp"
#include "Scene.hpp"

Kywy::Engine engine;

// Simple demo scene
class DemoScene : public Scene::Scene {
private:
  // Inner Actor class to handle input
  class InputHandler : public Actor::Actor {
  private:
    DemoScene* scene;

  public:
    InputHandler(DemoScene* parentScene)
      : Actor::Actor(), scene(parentScene) {
      // Auto-register with parent scene
      scene->Scene::add(this, false);
    }

    void handle(::Actor::Message* message) override {
      if (!scene->isActive()) return;

      if (message->signal == Kywy::Events::BUTTON_LEFT_PRESSED) {
        // Exit the scene
        scene->triggerExit();
      }
    }
  };

  InputHandler inputHandler;

public:
  DemoScene()
    : Scene(), inputHandler(this) {}

  void onEnter() override {
    Scene::getEngine()->display.clear();
    Scene::getEngine()->display.drawText(10, 40, "Demo Scene Active");
    Scene::getEngine()->display.drawText(10, 60, "Press LEFT to exit");
    Scene::getEngine()->display.update();
  }
};

DemoScene demoScene;
DemoScene demoScene2;

// Menu variables
bool soundOn = true;
bool vibrationOn = false;
bool musicOn = true;
std::string difficulty = "Medium";
std::string quality = "High";

Kywy::MenuSystem* menuSystem;

// Simple action functions
void startGame() {
  engine.display.clear();
  engine.display.drawText(10, 60, "Starting game...");
  engine.display.update();
  delay(1000);
}

void showInfo() {
  engine.display.clear();
  engine.display.drawText(10, 40, "Menu Demo v1.0");
  engine.display.drawText(10, 60, "Kywy Framework");
  engine.display.update();
  delay(2000);
}

void cycleDifficulty() {
  if (difficulty == "Easy") difficulty = "Medium";
  else if (difficulty == "Medium") difficulty = "Hard";
  else difficulty = "Easy";
}

void setup() {
  engine.start();
  Scene::setEngine(&engine);

  Kywy::MenuSystem::MenuOptions opts;
  opts.x = 0;
  opts.y = 10;
  opts.itemHeight = 15;
  opts.pointer = '>';

  menuSystem = new Kywy::MenuSystem(engine.display, opts);

  // Label
  menuSystem->addLabelItem("Menu Types Demo");
  menuSystem->addLabelItem("");  // Empty line

  // Action items
  menuSystem->addActionItem("Start Game", startGame);
  menuSystem->addActionItem("Info", showInfo);
  menuSystem->addSceneItem("Demo Scene", &demoScene);
  menuSystem->addActionItem("Credits", []() {
    engine.display.clear();
    engine.display.drawText(10, 50, "Made by Kywy Team");
    engine.display.update();
    delay(1500);
  });

  // Toggle items
  menuSystem->addToggleItem("Sound", &soundOn);
  menuSystem->addToggleItem("Vibration", &vibrationOn);
  menuSystem->addToggleItem("Music", &musicOn);

  menuSystem->addLabelItem("-- Settings --");

  // Option item
  menuSystem->addItem(Kywy::MenuSystem::MenuItem(
    "Difficulty",
    Kywy::MenuSystem::MenuItemType::OPTION,
    nullptr,
    []() {
      cycleDifficulty();
    },
    "",
    []() {
      return difficulty;
    }));

  menuSystem->addItem(Kywy::MenuSystem::MenuItem(
    "Quality",
    Kywy::MenuSystem::MenuItemType::OPTION,
    nullptr,
    []() {
      if (quality == "Low") quality = "Medium";
      else if (quality == "Medium") quality = "High";
      else quality = "Low";
    },
    "",
    []() {
      return quality;
    }));

  // Submenu
  Kywy::MenuSystem* subMenu = new Kywy::MenuSystem(engine.display, opts);
  subMenu->addLabelItem("Sub Menu");
  subMenu->addActionItem("Sub Action", []() {
    engine.display.clear();
    engine.display.drawText(10, 60, "Sub menu action!");
    engine.display.update();
    delay(1000);
  });

  // Scene item in submenu
  subMenu->addSceneItem("Demo Scene", &demoScene2);

  menuSystem->addItem(Kywy::MenuSystem::MenuItem(
    "Submenu",
    Kywy::MenuSystem::MenuItemType::SUBMENU,
    nullptr,
    nullptr,
    "",
    nullptr,
    subMenu));

  menuSystem->start(engine);
}

void loop() {
}
