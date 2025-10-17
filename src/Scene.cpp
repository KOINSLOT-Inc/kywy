// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Scene.hpp"
#include "Kywy.hpp"

// Static engine reference initialization
Kywy::Engine* Scene::engine = nullptr;

Scene::Scene(bool persistent, bool autoClearDisplay) 
  : persistent(persistent), autoClearDisplay(autoClearDisplay) {
}

Scene::~Scene() {
  if (initialized) {
    cleanup();
  }
}

// Static engine management
void Scene::setEngine(Kywy::Engine* eng) {
  engine = eng;
}

Kywy::Engine* Scene::getEngine() {
  return engine;
}

// Core lifecycle methods
void Scene::initialize() {
  if (!initialized) {
    onInitialize();  // Call virtual hook
    initialized = true;
  }
}

void Scene::cleanup() {
  if (initialized) {
    if (active) {
      exit();  // Ensure scene is properly exited
    }
    onCleanup();  // Call virtual hook
    initialized = false;
  }
}

void Scene::enter() {
  if (!initialized) {
    initialize();
  }
  
  active = true;
  onEnter();  // Call virtual hook

  // Enable all actors, subscribe to clock, and send scene enter message
  subscribeAllActors();
  uint8_t i = 0;
  while (i < MAX_ACTORS) {
    if (actors[i] == nullptr) break;
    actors[i]->enable();
    actors[i]->dispatch(&sceneEnterMessage);
    i++;
  }
}

void Scene::exit() {
  if (!active) return;
  
  active = false;

  // Disable all actors, unsubscribe from clock, and send scene exit message
  unsubscribeAllActors();
  uint8_t i = 0;
  while (i < MAX_ACTORS) {
    if (actors[i] == nullptr) break;
    actors[i]->disable();
    actors[i]->dispatch(&sceneExitMessage);
    i++;
  }

}

void Scene::subscribeAllActors() {
  if (!engine) return;
  for (uint8_t i = 0; i < MAX_ACTORS; ++i) {
    if (actors[i]) {
      actors[i]->subscribe(&engine->clock);
    }
  }
}

void Scene::unsubscribeAllActors() {
  if (!engine) return;
  for (uint8_t i = 0; i < MAX_ACTORS; ++i) {
    if (actors[i]) {
      actors[i]->unsubscribe(&engine->clock);
    }
  }

  onExit();  // Call virtual hook

  // Automatic display clearing if enabled
  if (autoClearDisplay && engine) {
    engine->display.clear();
  }

  // If not persistent, cleanup the scene
  if (!persistent) {
    cleanup();
  }
}

// Exit callback system
void Scene::setExitCallback(std::function<void()> callback) {
  exitCallback = callback;
}

void Scene::triggerExit() {
  exit();
  if (exitCallback) {
    exitCallback();
  }
}

void Scene::add(Actor::Actor *actor) {
  uint8_t i = 0;
  while (i < MAX_ACTORS && actors[i] != nullptr) {
    if (actors[i] == actor) {
      // TODO: error
      return;  // already subscribed
    }
    i++;
  }

  if (i == MAX_ACTORS) {
    // TODO: error
    return;  // subscriber limit reached
  }

  actors[i] = actor;
  if (active) {
    actors[i]->enable();
  } else {
    actors[i]->disable();
  }
}

void Scene::remove(Actor::Actor *actor) {
  uint8_t i = 0;
  bool removed = false;
  while (i < MAX_ACTORS && actors[i] != nullptr) {
    if (removed) {
      // shift remaining actors down so that loops don't have to iterate
      // over the entire list, just the beginning populated section
      actors[i - 1] = actors[i];
      actors[i] = nullptr;
    } else if (actors[i] == actor) {
      actors[i] = nullptr;
      removed = true;
    }
    i++;
  }
}
