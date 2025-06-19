// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SceneManager.hpp"
#include "Kywy.hpp"
#include "Events.hpp"
#include <algorithm>
#include <cstdio>

namespace Kywy {

//==============================================================================
// Scene Implementation
//==============================================================================

void Scene::addActor(std::shared_ptr<Actor::Actor> actor) {
  if (actor && std::find(actors.begin(), actors.end(), actor) == actors.end()) {
    actors.push_back(actor);
    actor->start();
  }
}

void Scene::removeActor(std::shared_ptr<Actor::Actor> actor) {
  auto it = std::find(actors.begin(), actors.end(), actor);
  if (it != actors.end()) {
    (*it)->stop();
    actors.erase(it);
  }
}

void Scene::clearActors() {
  for (auto& actor : actors) {
    if (actor) {
      actor->stop();
    }
  }
  actors.clear();
}

void Scene::addSprite(std::shared_ptr<Sprite> sprite) {
  if (sprite && std::find(sprites.begin(), sprites.end(), sprite) == sprites.end()) {
    sprites.push_back(sprite);
  }
}

void Scene::removeSprite(std::shared_ptr<Sprite> sprite) {
  auto it = std::find(sprites.begin(), sprites.end(), sprite);
  if (it != sprites.end()) {
    sprites.erase(it);
  }
}

void Scene::clearSprites() {
  sprites.clear();
}

void Scene::addGraphicsObject(std::shared_ptr<GraphicsObject> obj) {
  if (obj && std::find(graphicsObjects.begin(), graphicsObjects.end(), obj) == graphicsObjects.end()) {
    graphicsObjects.push_back(obj);
  }
}

void Scene::removeGraphicsObject(std::shared_ptr<GraphicsObject> obj) {
  auto it = std::find(graphicsObjects.begin(), graphicsObjects.end(), obj);
  if (it != graphicsObjects.end()) {
    graphicsObjects.erase(it);
  }
}

void Scene::clearGraphicsObjects() {
  graphicsObjects.clear();
}

//==============================================================================
// MenuScene Implementation  
//==============================================================================

void MenuScene::initialize() {
  Scene::initialize();
  if (menuSystem) {
    // Menu system will be started when scene becomes active
  }
}

void MenuScene::enter() {
  Scene::enter();
  if (menuSystem && engineRef) {
    // Use the new method that doesn't create an input handler
    menuSystem->startWithoutInputHandler(*engineRef);
    menuSystem->unpauseMenu();
  }
}

void MenuScene::update(float deltaTime) {
  // Menu system handles its own updates through input events
  // Update any actors in the scene
  for (auto& actor : actors) {
    if (actor) {
      // Actors handle their own updates through the Actor system
    }
  }
}

void MenuScene::render(Display::Display& display) {
  // Don't call menuSystem->displayMenu() as it includes display.update()
  // Instead, use renderMenuContent which only draws without clearing/updating
  if (menuSystem) {
    menuSystem->renderMenuContent();
  }
  
  // Render any additional graphics objects
  for (auto& obj : graphicsObjects) {
    if (obj) {
      obj->render();
    }
  }
  
  // Render sprites
  for (auto& sprite : sprites) {
    if (sprite) {
      sprite->render();
    }
  }
  
  // Don't call display.update() here - let SceneManager handle it
}

void MenuScene::exit() {
  if (menuSystem) {
    menuSystem->pauseMenu();
  }
  Scene::exit();
}

void MenuScene::handleInput(int signal, void* data) {
  // Handle input directly instead of using MenuSystem's input handler
  if (menuSystem && !menuSystem->isMenuPaused()) {
    switch (signal) {
      case Kywy::Events::D_PAD_UP_PRESSED:
        menuSystem->previousOptionNoRender();
        break;
      case Kywy::Events::D_PAD_DOWN_PRESSED:
        menuSystem->nextOptionNoRender();
        break;
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        menuSystem->selectOptionNoRender();
        break;
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        menuSystem->handleBackAction();
        break;
    }
  }
}

//==============================================================================
// SplashScene Implementation
//==============================================================================

void SplashScene::enter() {
  Scene::enter();
  timer = 0;
}

void SplashScene::update(float deltaTime) {
  timer += static_cast<uint16_t>(deltaTime);
  
  if (timer >= displayDuration) {
    if (onSplashComplete) {
      onSplashComplete();
    }
  }
}

void SplashScene::render(Display::Display& display) {
  if (splashData) {
    // Clear display first for memory LCD
    display.clear();
    
    // Calculate center position for splash image
    int16_t x = (KYWY_DISPLAY_WIDTH - width) / 2;
    int16_t y = (KYWY_DISPLAY_HEIGHT - height) / 2;
    
    // Render the splash bitmap
    display.drawBitmap(x, y, width, height, const_cast<uint8_t*>(splashData));
    display.update();
  }
}

//==============================================================================
// SceneManager Implementation
//==============================================================================

SceneManager::SceneManager(Engine& engine) : engine(engine) {
  allocateTransitionBuffers();
}

SceneManager::~SceneManager() {
  clearScenes();
  deallocateTransitionBuffers();
}

void SceneManager::handle(::Actor::Message* message) {
  if (!message) return;
  
  switch (message->signal) {
    case Kywy::Events::TICK:
      {
        // Update with fixed delta time (assuming 60 FPS for now)
        float deltaTime = 16.67f; // ~1/60 second in milliseconds
        update(deltaTime);
        render();
      }
      break;
      
    // Forward input events to current scene
    case Kywy::Events::BUTTON_LEFT_PRESSED:
    case Kywy::Events::BUTTON_RIGHT_PRESSED:
    case Kywy::Events::D_PAD_UP_PRESSED:
    case Kywy::Events::D_PAD_DOWN_PRESSED:
    case Kywy::Events::D_PAD_LEFT_PRESSED:
    case Kywy::Events::D_PAD_RIGHT_PRESSED:
      forwardInputToCurrentScene(message->signal, message->data);
      break;
      
    default:
      // Handle other messages as needed
      break;
  }
}

void SceneManager::initialize() {
  Actor::initialize();
  // Subscribe to engine events for updates and input
}

void SceneManager::pushScene(std::shared_ptr<Scene> scene, const SceneTransition& transition) {
  if (!scene) return;
  
  std::shared_ptr<Scene> currentScene = getCurrentScene();
  
  // Initialize the new scene
  scene->initialize();
  
  // Add to stack
  sceneStack.push_back(scene);
  
  // Handle overlay scenes
  if (scene->isOverlay && currentScene) {
    if (scene->pauseUnderlying) {
      currentScene->pause();
    }
  }
  
  // Start transition
  if (transition.type != TransitionType::IMMEDIATE && currentScene) {
    startTransition(currentScene, scene, transition);
  } else {
    activateScene(scene);
  }
  
  // Publish scene change event
  ::Actor::Message msg(Kywy::SceneEvents::SCENE_PUSHED, scene.get());
  publish(&msg);
}

void SceneManager::popScene(const SceneTransition& transition) {
  if (sceneStack.empty()) return;
  
  std::shared_ptr<Scene> currentScene = sceneStack.back();
  sceneStack.pop_back();
  
  std::shared_ptr<Scene> newCurrentScene = getCurrentScene();
  
  // Deactivate the popped scene
  deactivateScene(currentScene);
  
  // Resume underlying scene if it was paused
  if (newCurrentScene && newCurrentScene->isPaused()) {
    newCurrentScene->resume();
  }
  
  // Start transition
  if (transition.type != TransitionType::IMMEDIATE && newCurrentScene) {
    startTransition(currentScene, newCurrentScene, transition);
  } else if (newCurrentScene) {
    activateScene(newCurrentScene);
  }
  
  // Publish scene change event
  ::Actor::Message msg(Kywy::SceneEvents::SCENE_POPPED, currentScene.get());
  publish(&msg);
}

void SceneManager::changeScene(std::shared_ptr<Scene> scene, const SceneTransition& transition) {
  if (!scene) return;
  
  std::shared_ptr<Scene> currentScene = getCurrentScene();
  
  // Clear current scenes
  clearScenes();
  
  // Initialize new scene
  scene->initialize();
  sceneStack.push_back(scene);
  
  // Start transition
  if (transition.type != TransitionType::IMMEDIATE && currentScene) {
    startTransition(currentScene, scene, transition);
  } else {
    activateScene(scene);
  }
  
  // Publish scene change event
  ::Actor::Message msg(Kywy::SceneEvents::SCENE_CHANGED, scene.get());
  publish(&msg);
}

void SceneManager::clearScenes() {
  for (auto& scene : sceneStack) {
    if (scene) {
      deactivateScene(scene);
    }
  }
  sceneStack.clear();
}

std::shared_ptr<Scene> SceneManager::getCurrentScene() const {
  return sceneStack.empty() ? nullptr : sceneStack.back();
}

std::shared_ptr<Scene> SceneManager::getScene(const std::string& name) const {
  for (auto& scene : sceneStack) {
    if (scene && scene->getName() == name) {
      return scene;
    }
  }
  return nullptr;
}

bool SceneManager::hasScene(const std::string& name) const {
  return getScene(name) != nullptr;
}

void SceneManager::update(float deltaTime) {
  if (transitionInProgress) {
    updateTransition(deltaTime);
    return;
  }
  
  // Update active scenes (from bottom to top)
  for (auto& scene : sceneStack) {
    if (scene && scene->isActive()) {
      scene->update(deltaTime);
      
      // If scene is not an overlay, don't update scenes below it
      if (!scene->isOverlay) {
        break;
      }
    }
  }
}

void SceneManager::render() {
  if (transitionInProgress) {
    renderTransition();
    return;
  }
  
  // Find the bottom-most non-overlay scene to start rendering from
  int startIndex = sceneStack.size() - 1;
  for (int i = sceneStack.size() - 1; i >= 0; i--) {
    if (sceneStack[i] && !sceneStack[i]->isOverlay) {
      startIndex = i;
      break;
    }
  }
  
  // Clear display first for memory LCD
  engine.display.clear();
  
  // Render scenes from bottom to top
  for (int i = startIndex; i < static_cast<int>(sceneStack.size()); i++) {
    auto& scene = sceneStack[i];
    if (scene && (scene->isActive() || scene->isPaused())) {
      scene->render(engine.display);
    }
  }
  
  // Update display
  engine.display.update();
}

void SceneManager::startTransition(std::shared_ptr<Scene> fromScene, std::shared_ptr<Scene> toScene, const SceneTransition& transition) {
  transitionInProgress = true;
  currentTransition = transition;
  transitionTimer = 0;
  transitioningFromScene = fromScene;
  transitioningToScene = toScene;
  
  if (toScene) {
    toScene->setState(SceneState::TRANSITIONING);
  }
  if (fromScene) {
    fromScene->setState(SceneState::TRANSITIONING);
  }
  
  // Publish transition started event
  ::Actor::Message msg(Kywy::SceneEvents::TRANSITION_STARTED);
  publish(&msg);
}

void SceneManager::updateTransition(float deltaTime) {
  transitionTimer += static_cast<uint16_t>(deltaTime);
  
  if (transitionTimer >= currentTransition.duration) {
    finishTransition();
  }
}

void SceneManager::finishTransition() {
  transitionInProgress = false;
  
  // Activate new scene
  if (transitioningToScene) {
    activateScene(transitioningToScene);
  }
  
  // Deactivate old scene if it's not in the stack
  if (transitioningFromScene && 
      std::find(sceneStack.begin(), sceneStack.end(), transitioningFromScene) == sceneStack.end()) {
    deactivateScene(transitioningFromScene);
  }
  
  // Call completion callback
  if (currentTransition.onComplete) {
    currentTransition.onComplete();
  }
  
  if (transitionCompleteCallback) {
    transitionCompleteCallback();
  }
  
  // Clean up transition state
  transitioningFromScene = nullptr;
  transitioningToScene = nullptr;
  transitionTimer = 0;
  
  // Publish transition completed event
  ::Actor::Message msg(Kywy::SceneEvents::TRANSITION_COMPLETED);
  publish(&msg);
}

void SceneManager::renderTransition() {
  float progress = static_cast<float>(transitionTimer) / static_cast<float>(currentTransition.duration);
  progress = std::min(progress, 1.0f);
  
  switch (currentTransition.type) {
    case TransitionType::FADE_TO_BLACK:
      renderFadeTransition(progress);
      break;
      
    case TransitionType::SLIDE_LEFT:
      renderSlideTransition(progress, true, false);
      break;
      
    case TransitionType::SLIDE_RIGHT:
      renderSlideTransition(progress, true, true);
      break;
      
    case TransitionType::SLIDE_UP:
      renderSlideTransition(progress, false, false);
      break;
      
    case TransitionType::SLIDE_DOWN:
      renderSlideTransition(progress, false, true);
      break;
      
    case TransitionType::IMMEDIATE:
    default:
      // Should not reach here, but render new scene if it does
      if (transitioningToScene) {
        engine.display.clear();
        transitioningToScene->render(engine.display);
        engine.display.update();
      }
      break;
  }
}

void SceneManager::renderFadeTransition(float progress) {
  // For memory LCD, we'll simulate fade by dithering
  // This is a simplified approach - could be enhanced with better dithering patterns
  
  engine.display.clear();
  
  if (progress < 0.5f) {
    // Fade out old scene
    if (transitioningFromScene) {
      transitioningFromScene->render(engine.display);
      // Apply fade effect by modifying display buffer (simplified)
    }
  } else {
    // Fade in new scene
    if (transitioningToScene) {
      transitioningToScene->render(engine.display);
      // Apply fade effect by modifying display buffer (simplified)
    }
  }
  
  engine.display.update();
}

void SceneManager::renderSlideTransition(float progress, bool horizontal, bool positive) {
  engine.display.clear();
  
  if (horizontal) {
    // Horizontal slide
    int16_t offset = static_cast<int16_t>(progress * KYWY_DISPLAY_WIDTH);
    if (!positive) offset = -offset;
    
    // This is a simplified implementation
    // In a full implementation, you'd need to render to separate buffers
    // and then composite them with the appropriate offsets
    
    if (transitioningToScene) {
      transitioningToScene->render(engine.display);
    }
  } else {
    // Vertical slide
    int16_t offset = static_cast<int16_t>(progress * KYWY_DISPLAY_HEIGHT);
    if (!positive) offset = -offset;
    
    if (transitioningToScene) {
      transitioningToScene->render(engine.display);
    }
  }
  
  engine.display.update();
}

void SceneManager::activateScene(std::shared_ptr<Scene> scene) {
  if (scene) {
    // Set engine reference for scenes that need it
    scene->setEngineReference(&engine);
    
    scene->enter();
    
    // Set display reference for graphics objects
    for (auto& obj : scene->graphicsObjects) {
      if (obj) {
        obj->setDisplay(&engine.display);
      }
    }
    
    for (auto& sprite : scene->sprites) {
      if (sprite) {
        sprite->setDisplay(&engine.display);
      }
    }
  }
}

void SceneManager::deactivateScene(std::shared_ptr<Scene> scene) {
  if (scene) {
    scene->exit();
    scene->destroy();
  }
}

void SceneManager::forwardInputToCurrentScene(int signal, void* data) {
  std::shared_ptr<Scene> currentScene = getCurrentScene();
  if (currentScene && currentScene->isActive()) {
    currentScene->handleInput(signal, data);
  }
}

void SceneManager::allocateTransitionBuffers() {
  // Allocate buffers for transition effects if needed
  // For memory LCD, we might need frame buffers for complex transitions
  size_t bufferSize = (KYWY_DISPLAY_WIDTH * KYWY_DISPLAY_HEIGHT) / 8; // 1 bit per pixel
  transitionBuffer = new uint8_t[bufferSize];
  sceneBuffer = new uint8_t[bufferSize];
}

void SceneManager::deallocateTransitionBuffers() {
  delete[] transitionBuffer;
  delete[] sceneBuffer;
  transitionBuffer = nullptr;
  sceneBuffer = nullptr;
}

//==============================================================================
// SceneFactory Implementation
//==============================================================================

std::shared_ptr<SplashScene> SceneFactory::createSplashScene(
  const std::string& name,
  const uint8_t* splashData,
  uint16_t width,
  uint16_t height,
  uint16_t duration
) {
  return std::make_shared<SplashScene>(name, splashData, width, height, duration);
}

std::shared_ptr<MenuScene> SceneFactory::createMenuScene(
  const std::string& name,
  std::unique_ptr<MenuSystem> menuSystem
) {
  return std::make_shared<MenuScene>(name, std::move(menuSystem));
}

std::shared_ptr<Scene> SceneFactory::createGameScene(const std::string& name) {
  return std::make_shared<Scene>(name);
}

}  // namespace Kywy
