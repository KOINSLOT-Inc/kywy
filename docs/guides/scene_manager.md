# Scene Manager System

The Scene Manager system provides a powerful and flexible way to manage different game states and screens in your Kywy-based applications. It's specifically designed to work with the black-on-white memory LCD display and integrates seamlessly with the existing Actor system, MenuSystem, and graphics components.

## Features

- **Scene Stack Management**: Push, pop, and change scenes with a stack-based approach
- **Smooth Transitions**: Multiple transition types optimized for memory LCD
- **Overlay Support**: Create overlay scenes that render on top of existing scenes
- **Menu Integration**: Built-in support for MenuSystem scenes
- **Sprite & Actor Management**: Each scene can manage its own sprites and actors
- **Memory LCD Optimized**: Designed specifically for black-on-white memory displays
- **Actor Integration**: Works seamlessly with the existing Actor message system

## Core Components

### Scene Class

The base `Scene` class is the foundation for all scenes in your application.

```cpp
class MyGameScene : public Kywy::Scene {
public:
    MyGameScene() : Scene("MyGameScene") {}
    
    void initialize() override {
        // Initialize scene resources
    }
    
    void enter() override {
        // Called when scene becomes active
        Scene::enter();
    }
    
    void update(float deltaTime) override {
        // Update scene logic
    }
    
    void render(Display::Display& display) override {
        // Render scene content
        display.clear();
        // ... your rendering code
        display.update();
    }
    
    void handleInput(int signal, void* data) override {
        // Handle input events
        switch (signal) {
            case Events::BUTTON_LEFT_PRESSED:
                // Handle left button
                break;
        }
    }
    
    void exit() override {
        // Called when scene becomes inactive
        Scene::exit();
    }
};
```

### SceneManager Class

The `SceneManager` manages the scene stack and handles transitions.

```cpp
// Create scene manager
auto sceneManager = std::make_unique<SceneManager>(engine);
sceneManager->initialize();
sceneManager->start();

// Subscribe to engine events
sceneManager->subscribe(&engine.input);
sceneManager->subscribe(&engine.clock);
```

### Scene Types

#### 1. Regular Scenes
Standard scenes that take over the entire display.

#### 2. Overlay Scenes
Scenes that render on top of existing scenes:
```cpp
class PauseMenuScene : public Scene {
public:
    PauseMenuScene() : Scene("PauseMenu") {
        isOverlay = true;         // Render over previous scene
        pauseUnderlying = true;   // Pause the underlying scene
    }
};
```

#### 3. Menu Scenes
Specialized scenes for menu systems:
```cpp
auto menuScene = SceneFactory::createMenuScene("MainMenu", std::move(menuSystem));
```

#### 4. Splash Scenes
For loading screens and splash screens:
```cpp
auto splash = SceneFactory::createSplashScene(
    "SplashScreen",
    splashData,     // Bitmap data
    64, 32,         // Width, height
    2000           // Duration in ms
);
```

## Scene Management

### Adding Scenes

```cpp
// Push a scene onto the stack (keeps previous scenes)
sceneManager->pushScene(newScene, SceneTransition(TransitionType::SLIDE_LEFT, 400));

// Change to a new scene (clears previous scenes)
sceneManager->changeScene(newScene, SceneTransition(TransitionType::FADE_TO_BLACK, 500));

// Pop the current scene (return to previous)
sceneManager->popScene(SceneTransition(TransitionType::SLIDE_RIGHT, 300));
```

### Scene Resources

Each scene can manage its own resources:

```cpp
void MyScene::initialize() {
    // Add sprites
    auto mySprite = std::make_shared<Sprite>(frames, numFrames, width, height);
    addSprite(mySprite);
    
    // Add actors
    auto myActor = std::make_shared<MyActor>();
    addActor(myActor);
    
    // Add graphics objects
    auto myGraphics = std::make_shared<MyGraphicsObject>();
    addGraphicsObject(myGraphics);
}
```

## Transitions

The Scene Manager supports multiple transition types optimized for memory LCD:

### Transition Types

- `TransitionType::IMMEDIATE` - Instant scene change
- `TransitionType::FADE_TO_BLACK` - Fade out to black, then fade in new scene
- `TransitionType::SLIDE_LEFT` - Slide current scene left, new scene from right
- `TransitionType::SLIDE_RIGHT` - Slide current scene right, new scene from left
- `TransitionType::SLIDE_UP` - Slide current scene up, new scene from bottom
- `TransitionType::SLIDE_DOWN` - Slide current scene down, new scene from top

### Creating Transitions

```cpp
// Simple transition
SceneTransition transition(TransitionType::SLIDE_LEFT, 400);

// Transition with callback
SceneTransition transition(TransitionType::FADE_TO_BLACK, 500, []() {
    Serial.println("Transition complete!");
});

sceneManager->pushScene(newScene, transition);
```

## Menu Integration

The Scene Manager integrates seamlessly with the MenuSystem:

```cpp
// Create menu items
std::vector<MenuSystem::MenuItem> menuItems = {
    MenuSystem::createAction("Start Game", []() {
        auto gameScene = std::make_shared<GameScene>();
        sceneManager->pushScene(gameScene, SceneTransition(TransitionType::SLIDE_LEFT));
    }),
    
    MenuSystem::createAction("Settings", []() {
        auto settingsScene = createSettingsScene();
        sceneManager->pushScene(settingsScene, SceneTransition(TransitionType::SLIDE_UP));
    })
};

// Create menu scene
auto menuSystem = std::make_unique<MenuSystem>(engine.display, menuItems, menuOptions);
auto menuScene = SceneFactory::createMenuScene("MainMenu", std::move(menuSystem));
```

## Event System

The Scene Manager publishes events through the Actor system:

```cpp
class SceneEventHandler : public Actor::Actor {
public:
    void handle(Actor::Message* message) override {
        switch (message->signal) {
            case SceneEvents::SCENE_CHANGED:
                // Handle scene change
                break;
            case SceneEvents::TRANSITION_STARTED:
                // Handle transition start
                break;
            case SceneEvents::TRANSITION_COMPLETED:
                // Handle transition completion
                break;
        }
    }
};

// Subscribe to scene events
sceneEventHandler.subscribe(sceneManager.get());
```

## Memory LCD Optimization

The Scene Manager is specifically optimized for black-on-white memory LCD displays:

### Display Characteristics
- 144x168 pixel resolution
- 1-bit per pixel (black or white)
- Memory-based (retains image without power)
- No grayscale support

### Optimization Features
- Efficient transition rendering
- Dithering for fade effects
- Minimal memory usage
- Optimized for sharp black-on-white graphics

## Best Practices

### 1. Scene Lifecycle Management
```cpp
void MyScene::initialize() {
    // Load resources, create objects
}

void MyScene::enter() {
    Scene::enter();
    // Start animations, reset timers
}

void MyScene::exit() {
    // Pause/stop ongoing processes
    Scene::exit();
}

void MyScene::destroy() {
    // Clean up resources
    clearActors();
    clearSprites();
    clearGraphicsObjects();
    Scene::destroy();
}
```

### 2. Input Handling
```cpp
void MyScene::handleInput(int signal, void* data) {
    switch (signal) {
        case Events::BUTTON_LEFT_PRESSED:
            // Always provide a way to go back
            sceneManager->popScene();
            break;
        case Events::BUTTON_RIGHT_PRESSED:
            // Scene-specific actions
            break;
    }
}
```

### 3. Overlay Scenes
```cpp
class PauseScene : public Scene {
public:
    PauseScene() : Scene("Pause") {
        isOverlay = true;         // Render over game
        pauseUnderlying = true;   // Pause game updates
    }
    
    void render(Display::Display& display) override {
        // Draw pause menu over game
        // Use borders or backgrounds to distinguish from game
    }
};
```

### 4. Memory Management
```cpp
// Use smart pointers for automatic cleanup
auto scene = std::make_shared<MyScene>();

// Clear resources when scenes are no longer needed
scene->clearActors();
scene->clearSprites();
```

## Example Usage

See `examples/utility/SceneManagerExample/SceneManagerExample.ino` for a complete example showing:
- Splash screen with auto-transition
- Main menu with multiple options
- Settings menu as overlay
- Game scene with pause functionality
- Smooth transitions between all scenes
- Event handling and state management

## Integration with Existing Systems

### With MenuSystem
```cpp
auto menuScene = SceneFactory::createMenuScene("Menu", std::move(menuSystem));
```

### With Sprites
```cpp
void GameScene::initialize() {
    auto playerSprite = std::make_shared<Sprite>(playerFrames, numFrames, 16, 16);
    addSprite(playerSprite);
}
```

### With Actors
```cpp
void GameScene::initialize() {
    auto gameLogic = std::make_shared<GameLogicActor>();
    addActor(gameLogic);
}
```

The Scene Manager provides a robust foundation for creating complex, multi-screen applications while maintaining the simplicity and efficiency that Kywy is known for.
