// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_SCENE
#define KYWY_LIB_SCENE 1

#include "Actor.hpp"
#include "Events.hpp"
#include <functional>

// Forward declaration
namespace Kywy {
  class Engine;
}

const uint8_t MAX_ACTORS = 5;

class Scene {
  void subscribeAllActors();
  void unsubscribeAllActors();
private:
  Actor::Actor *actors[MAX_ACTORS] = {};
  bool active = false;
  bool initialized = false;
  
  static Kywy::Engine* engine;  // Static engine reference for display operations
  std::function<void()> exitCallback = nullptr;  // Callback for scene exit

  ::Actor::Message sceneEnterMessage = ::Actor::Message(Kywy::Events::KywyEvents::SCENE_ENTER);
  ::Actor::Message sceneExitMessage = ::Actor::Message(Kywy::Events::KywyEvents::SCENE_EXIT);

protected:
  bool persistent = false;        // Control whether scene persists across exits
  bool autoClearDisplay = true;   // Automatically clear display on exit

public:
  Scene(bool persistent = false, bool autoClearDisplay = true);
  virtual ~Scene();

  // Static engine management
  static void setEngine(Kywy::Engine* eng);
  static Kywy::Engine* getEngine();

  // Virtual lifecycle hooks for derived classes
  virtual void onInitialize() {};  // Called once when scene is first initialized
  virtual void onCleanup() {};     // Called when scene is being destroyed
  virtual void onEnter() {};       // Called each time scene becomes active
  virtual void onExit() {};        // Called each time scene becomes inactive

  // Core lifecycle methods
  void initialize();  // Initialize scene (calls onInitialize)
  void cleanup();     // Cleanup scene (calls onCleanup)
  void enter();       // Enter scene (calls onEnter)
  void exit();        // Exit scene (calls onExit)

  // Actor management
  void add(Actor::Actor *actor);
  void remove(Actor::Actor *actor);

  // State queries
  bool isActive() const { return active; }
  bool isInitialized() const { return initialized; }
  bool isPersistent() const { return persistent; }

  // Exit callback system
  void setExitCallback(std::function<void()> callback);
  void triggerExit();  // Trigger exit with callback
};

#endif
