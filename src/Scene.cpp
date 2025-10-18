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
  #ifdef ARDUINO
  Serial.println("Scene::enter() - starting");
  #endif
  
  if (!initialized) {
    #ifdef ARDUINO
    Serial.println("Scene::enter() - calling initialize()");
    #endif
    initialize();
  }
  
  #ifdef ARDUINO
  Serial.println("Scene::enter() - setting active=true");
  #endif
  active = true;
  
  #ifdef ARDUINO
  Serial.println("Scene::enter() - calling onEnter()");
  #endif
  onEnter();  // Call virtual hook

  #ifdef ARDUINO
  Serial.println("Scene::enter() - subscribing all actors to clock");
  #endif
  // Subscribe all actors to clock and input (if needed)
  subscribeAllActors();
  
  #ifdef ARDUINO
  Serial.println("Scene::enter() - processing actors");
  #endif
  uint8_t i = 0;
  while (i < MAX_ACTORS) {
    if (actors[i] == nullptr) break;
    
    #ifdef ARDUINO
    Serial.print("Scene::enter() - processing actor ");
    Serial.println(i);
    #endif
    
    // Subscribe to input if needed and not already subscribed
    if (actorNeedsInput[i] && engine) {
      #ifdef ARDUINO
      Serial.println("  - subscribing to input");
      #endif
      actors[i]->subscribe(&engine->input);
    }
    
    // Restart the actor if it was stopped during cleanup
    #ifdef ARDUINO
    Serial.println("  - starting actor");
    #endif
    actors[i]->start();
    
    #ifdef ARDUINO
    Serial.println("  - enabling actor");
    #endif
    actors[i]->enable();
    
    #ifdef ARDUINO
    Serial.println("  - dispatching SCENE_ENTER message");
    #endif
    actors[i]->dispatch(&sceneEnterMessage);
    
    i++;
  }
  
  #ifdef ARDUINO
  Serial.print("Scene::enter() - completed, processed ");
  Serial.print(i);
  Serial.println(" actors");
  #endif
}

void Scene::exit() {
  if (!active) return;
  
  #ifdef ARDUINO
  Serial.println("Scene::exit() - starting exit sequence");
  #endif
  
  active = false;

  // Unsubscribe all actors from clock and input, then disable
  unsubscribeAllActors();
  uint8_t i = 0;
  while (i < MAX_ACTORS) {
    if (actors[i] == nullptr) break;
    
    // Unsubscribe from input if needed
    if (actorNeedsInput[i] && engine) {
      actors[i]->unsubscribe(&engine->input);
    }
    
    actors[i]->disable();
    actors[i]->dispatch(&sceneExitMessage);
    i++;
  }

  #ifdef ARDUINO
  Serial.print("Scene::exit() - disabled ");
  Serial.print(i);
  Serial.println(" actors");
  #endif

  // Call virtual hook
  onExit();

  #ifdef ARDUINO
  Serial.println("Scene::exit() - completed");
  #endif

  // NOTE: Display clearing is now handled by MenuSystem for better control
  // Scenes should NOT clear the display themselves

  // NOTE: We do NOT call cleanup() here even if not persistent
  // This is because exit() might be called from within an actor's handle() method,
  // and cleanup() would delete that actor while it's still executing
  // Instead, cleanup is called from the destructor or explicitly when safe
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
  #ifdef ARDUINO
  Serial.println("Scene::triggerExit() called");
  #endif
  
  // Make a local copy of the callback and clear it BEFORE calling
  // This prevents issues with the callback trying to modify exitCallback
  auto callback = exitCallback;
  exitCallback = nullptr;
  
  #ifdef ARDUINO
  Serial.println("Scene::triggerExit() - about to call exit()");
  #endif
  
  // Exit the scene
  exit();
  
  #ifdef ARDUINO
  Serial.println("Scene::triggerExit() - exit() completed");
  #endif
  
  // Now invoke the callback if it existed
  if (callback) {
    #ifdef ARDUINO
    Serial.println("Scene::triggerExit() - invoking exit callback");
    #endif
    callback();
    #ifdef ARDUINO
    Serial.println("Scene::triggerExit() - callback completed");
    #endif
  } else {
    #ifdef ARDUINO
    Serial.println("Scene::triggerExit() - NO CALLBACK SET!");
    #endif
  }
}

void Scene::add(Actor::Actor *actor, bool owned, bool subscribeToInput) {
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
  actorOwned[i] = owned;  // Track ownership
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
