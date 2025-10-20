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

    // Clean up all actors
    for (uint8_t i = 0; i < MAX_ACTORS; ++i) {
      if (actors[i]) {
        // Unsubscribe from clock and input if still subscribed
        actors[i]->unsubscribe(&engine->clock);
        if (actorNeedsInput[i] && engine) {
          actors[i]->unsubscribe(&engine->input);
        }
        // Stop and disable the actor
        actors[i]->disable();
        actors[i]->stop();

        // Only delete heap-allocated actors
        if (actorOwned[i]) {
          delete actors[i];
          actors[i] = nullptr;
          actorOwned[i] = false;
          actorNeedsInput[i] = false;
        }
        // For non-owned actors (member variables), keep them in the array
        // They will be re-used when the scene is entered again
      }
    }

    onCleanup();  // Call virtual hook - for additional cleanup
    initialized = false;
  }
}

void Scene::enter() {
  if (!initialized) {
    initialize();
  }

  active = true;
  onEnter();  // Call virtual hook

  // Subscribe all actors to clock and input (if needed)
  subscribeAllActors();
  uint8_t i = 0;
  while (i < MAX_ACTORS) {
    if (actors[i] == nullptr) break;

    // Subscribe to input if needed and not already subscribed
    if (actorNeedsInput[i] && engine) {
      actors[i]->subscribe(&engine->input);
    }

    // Restart the actor if it was stopped during cleanup
    actors[i]->start();
    actors[i]->enable();
    actors[i]->dispatch(&sceneEnterMessage);
    i++;
  }
}

void Scene::exit() {
  if (!active) return;

  active = false;

  // Disable all actors FIRST to prevent new messages during exit
  uint8_t i = 0;
  while (i < MAX_ACTORS) {
    if (actors[i] == nullptr) break;
    actors[i]->disable();
    i++;
  }

  // Unsubscribe from clock and input to prevent new messages
  unsubscribeAllActors();
  i = 0;
  while (i < MAX_ACTORS) {
    if (actors[i] == nullptr) break;
    if (actorNeedsInput[i] && engine) {
      actors[i]->unsubscribe(&engine->input);
    }
    i++;
  }

  // Dispatch exit message and stop actors
  i = 0;
  while (i < MAX_ACTORS) {
    if (actors[i] == nullptr) break;
    actors[i]->dispatch(&sceneExitMessage);
    actors[i]->stop();
    i++;
  }

  onExit();
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
}

// Exit callback system
void Scene::setExitCallback(std::function<void()> callback) {
  exitCallback = callback;
}

void Scene::triggerExit() {
  // Make a local copy of the callback and clear it BEFORE calling
  // This prevents issues with the callback trying to modify exitCallback
  auto callback = exitCallback;
  exitCallback = nullptr;

  // Exit the scene
  exit();

  // Now invoke the callback if it existed
  if (callback) {
    callback();
  }
}

void Scene::add(Actor::Actor* actor, bool owned, bool subscribeToInput) {
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
  actorOwned[i] = owned;                  // Track ownership
  actorNeedsInput[i] = subscribeToInput;  // Track if actor needs input

  // Start the actor so it can receive messages
  actor->start();

  // Try to subscribe to input now if engine is available
  // Otherwise it will be done in enter()
  if (subscribeToInput && engine) {
    actor->subscribe(&engine->input);
  }

  // Enable/disable based on scene state
  if (active) {
    actor->enable();
  } else {
    actor->disable();
  }
}
void Scene::remove(Actor::Actor* actor) {
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
