// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_SCENE_MANAGER
#define KYWY_LIB_SCENE_MANAGER 1

#include "Actor.hpp"
#include "Display.hpp"
#include "Sprite.hpp"
#include "MenuSystem.hpp"
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>

namespace Kywy {

// Forward declarations
class Engine;
class Scene;
class SceneManager;

// Scene transition types
enum class TransitionType {
  IMMEDIATE,      // Instant scene change
  FADE_TO_BLACK,  // Fade to black then fade in new scene
  SLIDE_LEFT,     // Slide current scene left, new scene comes from right
  SLIDE_RIGHT,    // Slide current scene right, new scene comes from left
  SLIDE_UP,       // Slide current scene up, new scene comes from bottom
  SLIDE_DOWN      // Slide current scene down, new scene comes from top
};

// Scene states
enum class SceneState {
  INACTIVE,       // Scene is not running
  INITIALIZING,   // Scene is being initialized
  ACTIVE,         // Scene is actively running
  PAUSED,         // Scene is paused (another scene may be overlaid)
  TRANSITIONING,  // Scene is in transition (entering or exiting)
  DESTROYED       // Scene has been destroyed
};

// Scene transition configuration
struct SceneTransition {
  TransitionType type = TransitionType::IMMEDIATE;
  uint16_t duration = 500;  // Duration in milliseconds
  std::function<void()> onComplete = nullptr;  // Callback when transition completes
  
  SceneTransition() = default;
  SceneTransition(TransitionType t, uint16_t d = 500, std::function<void()> callback = nullptr)
    : type(t), duration(d), onComplete(callback) {}
};

// Base Scene class - inherit from this to create specific scenes
class Scene {
public:
  Scene(const std::string& name) : sceneName(name), state(SceneState::INACTIVE) {}
  virtual ~Scene() = default;

  // Scene lifecycle methods
  virtual void initialize() {}
  virtual void enter() { state = SceneState::ACTIVE; }
  virtual void update(float deltaTime) {}
  virtual void render(Display::Display& display) {}
  virtual void exit() { state = SceneState::INACTIVE; }
  virtual void destroy() { state = SceneState::DESTROYED; }
  
  // Pause/Resume for overlay scenes
  virtual void pause() { state = SceneState::PAUSED; }
  virtual void resume() { state = SceneState::ACTIVE; }
  
  // Input handling
  virtual void handleInput(int signal, void* data) {}
  
  // Scene management
  const std::string& getName() const { return sceneName; }
  SceneState getState() const { return state; }
  bool isActive() const { return state == SceneState::ACTIVE; }
  bool isPaused() const { return state == SceneState::PAUSED; }
  
  // Actor management within scene
  void addActor(std::shared_ptr<Actor::Actor> actor);
  void removeActor(std::shared_ptr<Actor::Actor> actor);
  void clearActors();
  
  // Sprite management within scene
  void addSprite(std::shared_ptr<Sprite> sprite);
  void removeSprite(std::shared_ptr<Sprite> sprite);
  void clearSprites();
  
  // Graphics object management
  void addGraphicsObject(std::shared_ptr<GraphicsObject> obj);
  void removeGraphicsObject(std::shared_ptr<GraphicsObject> obj);
  void clearGraphicsObjects();
  
  // Scene properties
  bool isOverlay = false;  // If true, scene renders over previous scene
  bool pauseUnderlying = true;  // If true, pauses underlying scenes when overlay
  
  // Virtual method for scene-specific engine setup
  virtual void setEngineReference(Engine* engine) {}
  
protected:
  std::string sceneName;
  SceneState state;
  
  // Scene resources
  std::vector<std::shared_ptr<Actor::Actor>> actors;
  std::vector<std::shared_ptr<Sprite>> sprites;
  std::vector<std::shared_ptr<GraphicsObject>> graphicsObjects;
  
  // Helper methods for derived classes
  void setState(SceneState newState) { state = newState; }
  
  friend class SceneManager;
};

// Menu Scene - specialized scene for menu systems
class MenuScene : public Scene {
public:
  MenuScene(const std::string& name, std::unique_ptr<MenuSystem> menu)
    : Scene(name), menuSystem(std::move(menu)), engineRef(nullptr) {}
  
  void initialize() override;
  void enter() override;
  void update(float deltaTime) override;
  void render(Display::Display& display) override;
  void exit() override;
  void handleInput(int signal, void* data) override;
  
  MenuSystem* getMenuSystem() { return menuSystem.get(); }
  
  // Override setEngineReference to set up menu system
  void setEngineReference(Engine* engine) override { 
    engineRef = engine; 
  }
  
private:
  std::unique_ptr<MenuSystem> menuSystem;
  Engine* engineRef;
};

// Splash Scene - for loading screens, splash screens, etc.
class SplashScene : public Scene {
public:
  SplashScene(const std::string& name, const uint8_t* splashData, 
              uint16_t width, uint16_t height, uint16_t displayDuration = 3000)
    : Scene(name), splashData(splashData), width(width), height(height), 
      displayDuration(displayDuration), timer(0) {}
  
  void enter() override;
  void update(float deltaTime) override;
  void render(Display::Display& display) override;
  
  // Callback when splash is complete
  std::function<void()> onSplashComplete = nullptr;
  
private:
  const uint8_t* splashData;
  uint16_t width, height;
  uint16_t displayDuration;  // Duration in milliseconds
  uint16_t timer;
};

// Scene Manager - manages scene stack and transitions
class SceneManager : public Actor::Actor {
public:
  SceneManager(Engine& engine);
  ~SceneManager();
  
  // Debug control
  static bool debugMode;
  static void setDebugMode(bool enabled) { debugMode = enabled; }
  static bool isDebugMode() { return debugMode; }
  
  // Actor interface
  void handle(::Actor::Message* message) override;
  void initialize() override;
  
  // Scene management
  void pushScene(std::shared_ptr<Scene> scene, const SceneTransition& transition = SceneTransition());
  void popScene(const SceneTransition& transition = SceneTransition());
  void changeScene(std::shared_ptr<Scene> scene, const SceneTransition& transition = SceneTransition());
  void clearScenes();
  
  // Get current scene
  std::shared_ptr<Scene> getCurrentScene() const;
  std::shared_ptr<Scene> getScene(const std::string& name) const;
  bool hasScene(const std::string& name) const;
  
  // Scene stack management
  size_t getSceneCount() const { return sceneStack.size(); }
  bool isEmpty() const { return sceneStack.empty(); }
  
  // Update and render
  void update(float deltaTime);
  void render();
  
  // Transition management
  bool isTransitioning() const { return transitionInProgress; }
  void setTransitionCallback(std::function<void()> callback) { transitionCompleteCallback = callback; }
  
  // Engine access
  Engine& getEngine() { return engine; }
  
private:
  Engine& engine;
  std::vector<std::shared_ptr<Scene>> sceneStack;
  
  // Transition state
  bool transitionInProgress = false;
  SceneTransition currentTransition;
  uint16_t transitionTimer = 0;
  std::shared_ptr<Scene> transitioningFromScene = nullptr;
  std::shared_ptr<Scene> transitioningToScene = nullptr;
  std::function<void()> transitionCompleteCallback = nullptr;
  
  // Transition rendering buffers for memory LCD
  uint8_t* transitionBuffer = nullptr;
  uint8_t* sceneBuffer = nullptr;
  
  // Internal methods
  void startTransition(std::shared_ptr<Scene> fromScene, std::shared_ptr<Scene> toScene, const SceneTransition& transition);
  void updateTransition(float deltaTime);
  void finishTransition();
  void renderTransition();
  
  // Transition effect implementations optimized for black-on-white memory LCD
  void renderFadeTransition(float progress);
  void renderSlideTransition(float progress, bool horizontal, bool positive);
  
  // Scene lifecycle management
  void activateScene(std::shared_ptr<Scene> scene);
  void deactivateScene(std::shared_ptr<Scene> scene);
  
  // Input handling
  void forwardInputToCurrentScene(int signal, void* data);
  
  // Memory management for transitions
  void allocateTransitionBuffers();
  void deallocateTransitionBuffers();
};

// Scene Factory - utility class for creating common scene types
class SceneFactory {
public:
  // Create a splash scene
  static std::shared_ptr<SplashScene> createSplashScene(
    const std::string& name,
    const uint8_t* splashData,
    uint16_t width,
    uint16_t height,
    uint16_t duration = 3000
  );
  
  // Create a menu scene
  static std::shared_ptr<MenuScene> createMenuScene(
    const std::string& name,
    std::unique_ptr<MenuSystem> menuSystem
  );
  
  // Create an empty game scene
  static std::shared_ptr<Scene> createGameScene(const std::string& name);
};

// Scene Manager events for integration with the Actor system
namespace SceneEvents {
  enum {
    SCENE_CHANGED = 1000,  // Use higher numbers to avoid conflicts
    SCENE_PUSHED,
    SCENE_POPPED,
    TRANSITION_STARTED,
    TRANSITION_COMPLETED,
    SCENE_PAUSED,
    SCENE_RESUMED
  };
}

}  // namespace Kywy

#endif  // KYWY_LIB_SCENE_MANAGER
