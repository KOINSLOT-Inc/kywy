// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * SceneManager.ino - Learning Scene Management
 * 
 * This educational example teaches you how to use the Scene Manager system
 * to create multi-screen applications with smooth transitions.
 * 
 * LEARNING OBJECTIVES:
 * 1. Understand the Scene concept and lifecycle
 * 2. Learn how to create different types of scenes
 * 3. Practice scene navigation and stacking
 * 4. Integrate menus with scene management
 * 5. Handle input and state across scenes
 * 
 * HARDWARE: Kywy device with black-on-white memory LCD (144x168)
 * 
 * WHAT YOU'LL BUILD:
 * - A welcome splash screen
 * - A main menu with multiple options
 * - Different demo scenes showing various concepts
 * - Scene navigation and stacking
 * - Proper resource management
 * 
 * INSTRUCTIONS:
 * 1. Open the Serial Monitor to see educational messages
 * 2. Use D-pad UP/DOWN to navigate menus
 * 3. Use RIGHT button to select items
 * 4. Use LEFT button to go back
 * 5. Follow the lessons in order for best learning experience
 */

#include "Kywy.hpp"

using namespace Kywy;

// Global objects - needed by all scenes
Engine engine;
std::unique_ptr<SceneManager> sceneManager;

// Forward declarations - we'll implement these step by step
std::shared_ptr<SplashScene> createWelcomeScene();
std::shared_ptr<MenuScene> createMainMenu();
std::shared_ptr<Scene> createTextDemoScene();
std::shared_ptr<Scene> createAnimationDemoScene();
std::shared_ptr<Scene> createInputDemoScene();

//==============================================================================
// LESSON 1: Basic Scene Creation
// Learn how to create a simple scene that displays text and handles input
//==============================================================================

class BasicTextScene : public Scene {
private:
  std::string message;
  std::string instruction;
  
public:
  BasicTextScene(const std::string& sceneName, 
                 const std::string& displayMessage,
                 const std::string& backInstruction = "Press LEFT to go back")
    : Scene(sceneName), message(displayMessage), instruction(backInstruction) {}
  
  // Called when this scene becomes the active scene
  void enter() override {
    Scene::enter();  // Always call parent implementation first
    Serial.print("📖 LESSON 1: Entered scene: ");
    Serial.println(sceneName.c_str());
    Serial.println("💡 CONCEPT: enter() is called each time a scene becomes active");
  }
  
  // Called every frame to draw the scene
  void render(Display::Display& display) override {
    display.clear();  // Start with a clean screen
    
    // Set up text rendering options
    Display::TextOptions textOpt;
    textOpt._color = BLACK;  // Black text on white background (memory LCD)
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    // Scene title
    display.drawText(20, 20, sceneName.c_str(), textOpt);
    
    // Main message - implement simple word wrapping for longer text
    int yPos = 50;
    if (message.length() <= 18) {
      // Short message - draw on one line
      display.drawText(10, yPos, message.c_str(), textOpt);
    } else {
      // Long message - wrap text across multiple lines
      size_t pos = 0;
      while (pos < message.length()) {
        std::string line = message.substr(pos, 18);
        if (pos + 18 < message.length()) {
          // Find last space to avoid breaking words
          size_t lastSpace = line.find_last_of(' ');
          if (lastSpace != std::string::npos) {
            line = line.substr(0, lastSpace);
            pos += lastSpace + 1;
          } else {
            pos += 18;
          }
        } else {
          pos = message.length();
        }
        display.drawText(10, yPos, line.c_str(), textOpt);
        yPos += 15;  // Move to next line
      }
    }
    
    // Instructions at bottom of screen
    display.drawText(10, KYWY_DISPLAY_HEIGHT - 20, instruction.c_str(), textOpt);
    
    display.update();  // Apply all changes to the display
  }
  
  // Called when user provides input
  void handleInput(int signal, void* data) override {
    Serial.print("📖 LESSON 1: Input received - Signal: ");
    Serial.println(signal);
    
    switch (signal) {
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        Serial.println("💡 CONCEPT: popScene() removes current scene from stack");
        sceneManager->popScene();
        break;
        
      default:
        // Ignore other inputs for this simple scene
        break;
    }
  }
  
  // Called when this scene is no longer active
  void exit() override {
    Serial.print("📖 LESSON 1: Exited scene: ");
    Serial.println(sceneName.c_str());
    Serial.println("💡 CONCEPT: exit() is called when a scene becomes inactive");
    Scene::exit();  // Always call parent implementation last
  }
};

//==============================================================================
// LESSON 2: Animation and State Management
// Learn how scenes can maintain state and animate content over time
//==============================================================================

class AnimationDemoScene : public Scene {
private:
  float timer = 0.0f;        // Tracks total time since scene started
  int bounceX = 20;          // X position of bouncing object
  int bounceDirection = 1;   // Direction of movement (-1 or 1)
  bool animationEnabled = true;  // Whether animation is running
  
public:
  AnimationDemoScene() : Scene("Animation Demo") {}
  
  void enter() override {
    Scene::enter();
    // Reset animation state when entering the scene
    timer = 0.0f;
    bounceX = 20;
    bounceDirection = 1;
    animationEnabled = true;
    
    Serial.println("📖 LESSON 2: Animation Demo Scene entered");
    Serial.println("💡 CONCEPT: Scenes can maintain state and animate content");
    Serial.println("💡 TRY: Press RIGHT to toggle animation, UP to reset");
    Serial.println("💡 DEBUG: Animation starting - watch for bouncing square");
  }
  
  // Called every frame with the time elapsed since last frame
  void update(float deltaTime) override {
    Scene::update(deltaTime);  // Always call parent first
    
    if (animationEnabled) {
      timer += deltaTime;  // deltaTime is in milliseconds
      
      // Debug: Print position occasionally
      static int debugCounter = 0;
      debugCounter++;
      if (debugCounter % 60 == 0) {  // Every 60 frames (about once per second)
        Serial.print("📖 DEBUG: Animation position X=");
        Serial.print(bounceX);
        Serial.print(", Timer=");
        Serial.println(timer / 1000.0f);
      }
      
      // Simple bouncing animation - move at a reasonable speed
      // Move about 60 pixels per second (1 pixel per frame at 60fps)
      bounceX += bounceDirection * 1;  // Move 1 pixel per frame
      
      // Bounce off edges of screen (leave some margin)
      if (bounceX >= KYWY_DISPLAY_WIDTH - 20 || bounceX <= 10) {
        bounceDirection *= -1;  // Reverse direction
        Serial.println("📖 LESSON 2: Object bounced - direction reversed");
      }
    }
  }
  
  void render(Display::Display& display) override {
    display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    // Title
    display.drawText(20, 15, "Animation Demo", textOpt);
    
    // Draw bouncing square (10x10 pixels for better visibility)
    for (int x = bounceX; x < bounceX + 10; x++) {
      for (int y = 40; y < 50; y++) {
        display.drawPixel(x, y, BLACK);
      }
    }
    
    // Display timer (convert milliseconds to seconds)
    char timeStr[32];
    snprintf(timeStr, sizeof(timeStr), "Time: %.1fs", timer / 1000.0f);
    display.drawText(10, 60, timeStr, textOpt);
    
    // Show animation state
    display.drawText(10, 80, animationEnabled ? "Status: Running" : "Status: Paused", textOpt);
    
    // Display position and direction for debugging
    char posStr[32];
    snprintf(posStr, sizeof(posStr), "X: %d, Dir: %s", bounceX, bounceDirection > 0 ? "Right" : "Left");
    display.drawText(10, 100, posStr, textOpt);
    
    // Controls help
    display.drawText(10, 115, "RIGHT: Toggle anim", textOpt);
    display.drawText(10, 130, "UP: Reset timer", textOpt);
    display.drawText(10, 145, "LEFT: Back to menu", textOpt);
    
    display.update();
  }
  
  void handleInput(int signal, void* data) override {
    Serial.print("📖 LESSON 2: Input in animation scene - Signal: ");
    Serial.println(signal);
    
    switch (signal) {
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        Serial.println("💡 CONCEPT: Returning to previous scene with popScene()");
        sceneManager->popScene();
        break;
        
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        animationEnabled = !animationEnabled;
        Serial.print("💡 STATE: Animation ");
        Serial.println(animationEnabled ? "enabled" : "disabled");
        Serial.print("💡 DEBUG: Current position X=");
        Serial.println(bounceX);
        break;
        
      case Kywy::Events::D_PAD_UP_PRESSED:
        timer = 0.0f;
        bounceX = 20;
        bounceDirection = 1;
        Serial.println("💡 STATE: Animation reset to initial state");
        break;
        
      default:
        // Ignore other inputs
        break;
    }
  }
};

//==============================================================================
// LESSON 3: Comprehensive Input Handling
// Learn how to handle all types of user input in scenes
//==============================================================================

class InputDemoScene : public Scene {
private:
  std::string lastButtonPressed = "None";
  int buttonPressCount = 0;
  bool dpadActive = false;
  unsigned long lastInputTime = 0;  // Track when last input occurred
  
public:
  InputDemoScene() : Scene("Input Demo") {}
  
  void enter() override {
    Scene::enter();
    // Reset input tracking when entering scene
    lastButtonPressed = "None";
    buttonPressCount = 0;
    dpadActive = false;
    lastInputTime = millis();
    
    Serial.println("📖 LESSON 3: Input Demo Scene entered");
    Serial.println("💡 CONCEPT: Scenes can handle multiple types of input");
    Serial.println("💡 TRY: Press all buttons and D-pad directions to see them detected");
  }
  
  void render(Display::Display& display) override {
    display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    // Title
    display.drawText(30, 15, "Input Demo", textOpt);
    
    // Last button info
    display.drawText(10, 40, "Last Input:", textOpt);
    display.drawText(10, 55, lastButtonPressed.c_str(), textOpt);
    
    // Press count
    char countStr[32];
    snprintf(countStr, sizeof(countStr), "Total Presses: %d", buttonPressCount);
    display.drawText(10, 75, countStr, textOpt);
    
    // D-pad activity indicator
    display.drawText(10, 95, dpadActive ? "D-Pad: ACTIVE" : "D-Pad: inactive", textOpt);
    
    // Show time since last input
    unsigned long timeSinceInput = millis() - lastInputTime;
    if (timeSinceInput < 5000) {  // Show for 5 seconds
      char timeStr[32];
      snprintf(timeStr, sizeof(timeStr), "%.1fs ago", timeSinceInput / 1000.0f);
      display.drawText(80, 55, timeStr, textOpt);
    }
    
    // Instructions
    display.drawText(10, 120, "Try all controls!", textOpt);
    display.drawText(10, 135, "LEFT: Back (2nd press)", textOpt);
    display.drawText(10, 150, "Watch Serial Monitor!", textOpt);
    
    display.update();
  }
  
  void handleInput(int signal, void* data) override {
    buttonPressCount++;
    dpadActive = false;  // Reset D-pad indicator
    lastInputTime = millis();  // Update last input time
    
    Serial.print("📖 LESSON 3: Input detected - Signal: ");
    Serial.print(signal);
    Serial.print(", Total count: ");
    Serial.println(buttonPressCount);
    
    switch (signal) {
      case Kywy::Events::BUTTON_LEFT_PRESSED:
        lastButtonPressed = "LEFT BUTTON";
        if (buttonPressCount > 1) {
          // Only go back after second press to demonstrate input handling
          Serial.println("💡 CONCEPT: LEFT button pressed twice - going back");
          sceneManager->popScene();
        } else {
          Serial.println("💡 INFO: Press LEFT again to go back to menu");
        }
        break;
        
      case Kywy::Events::BUTTON_RIGHT_PRESSED:
        lastButtonPressed = "RIGHT BUTTON";
        Serial.println("💡 INFO: RIGHT button recognized");
        break;
        
      case Kywy::Events::D_PAD_UP_PRESSED:
        lastButtonPressed = "D-PAD UP";
        dpadActive = true;
        Serial.println("💡 INFO: D-pad UP direction detected");
        break;
        
      case Kywy::Events::D_PAD_DOWN_PRESSED:
        lastButtonPressed = "D-PAD DOWN";
        dpadActive = true;
        Serial.println("💡 INFO: D-pad DOWN direction detected");
        break;
        
      case Kywy::Events::D_PAD_LEFT_PRESSED:
        lastButtonPressed = "D-PAD LEFT";
        dpadActive = true;
        Serial.println("💡 INFO: D-pad LEFT direction detected");
        break;
        
      case Kywy::Events::D_PAD_RIGHT_PRESSED:
        lastButtonPressed = "D-PAD RIGHT";
        dpadActive = true;
        Serial.println("💡 INFO: D-pad RIGHT direction detected");
        break;
        
      default:
        // Handle any other input signals
        char unknownStr[32];
        snprintf(unknownStr, sizeof(unknownStr), "UNKNOWN (%d)", signal);
        lastButtonPressed = unknownStr;
        Serial.print("💡 WARNING: Unknown input signal: ");
        Serial.println(signal);
        break;
    }
  }
  
  void exit() override {
    Serial.println("📖 LESSON 3: Exiting Input Demo");
    Serial.print("💡 SUMMARY: Handled ");
    Serial.print(buttonPressCount);
    Serial.println(" input events total");
    Scene::exit();
  }
};

//==============================================================================
// LESSON 4: Advanced Scene Creation Functions
// Learn how to organize and create different types of scenes
//==============================================================================

std::shared_ptr<SplashScene> createWelcomeScene() {
  Serial.println("📖 CREATING: Welcome splash scene");
  Serial.println("💡 CONCEPT: SplashScene shows content for a fixed duration, then auto-transitions");
  
  // Create simple splash screen data (8x5 pattern for demo)
  // In a real app, this would be your logo or welcome graphic
  static const uint8_t welcomeData[] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // Border
    0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,  // Empty with border
    0x81, 0x18, 0x3C, 0x7E, 0x7E, 0x3C, 0x18, 0x81,  // Simple pattern
    0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81,  // Empty with border
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,  // Border
  };
  
  // Create splash scene using SceneFactory
  auto splash = SceneFactory::createSplashScene(
    "Welcome",           // Scene name
    welcomeData,         // Bitmap data
    64, 40,             // Width and height in pixels
    2000                // Duration in milliseconds (2 seconds)
  );
  
  // Set callback for when splash completes
  splash->onSplashComplete = []() {
    Serial.println("📖 SPLASH: Completed - transitioning to main menu");
    Serial.println("💡 CONCEPT: Splash callback automatically triggers next scene");
    
    auto mainMenu = createMainMenu();
    sceneManager->changeScene(mainMenu);
  };
  
  Serial.println("💡 USAGE: Splash will show for 2 seconds, then change to main menu");
  return splash;
}

std::shared_ptr<MenuScene> createMainMenu() {
  Serial.println("📖 LESSON 4: Creating Main Menu");
  Serial.println("💡 CONCEPT: MenuScene integrates MenuSystem with Scene management");
  
  std::vector<MenuSystem::MenuItem> menuItems = {
    MenuSystem::createLabel("-- SCENE DEMOS --"),
    
    MenuSystem::createAction("Text Demo", []() {
      Serial.println("📖 LESSON 4: Navigating to Text Demo (Lesson 1)");
      auto textScene = std::make_shared<BasicTextScene>(
        "Text Demo",
        "This scene shows how to display text and handle basic input. Scenes are the building blocks of your app!"
      );
      sceneManager->pushScene(textScene);
    }),
    
    MenuSystem::createAction("Animation Demo", []() {
      Serial.println("📖 LESSON 4: Navigating to Animation Demo (Lesson 2)");
      auto animScene = std::make_shared<AnimationDemoScene>();
      sceneManager->pushScene(animScene);
    }),
    
    MenuSystem::createAction("Input Demo", []() {
      Serial.println("📖 LESSON 4: Navigating to Input Demo (Lesson 3)");
      auto inputScene = std::make_shared<InputDemoScene>();
      sceneManager->pushScene(inputScene);
    }),
    
    MenuSystem::createLabel("-- LEARNING --"),
    
    MenuSystem::createAction("About Scenes", []() {
      Serial.println("📖 EDUCATIONAL: Showing scene concept explanation");
      auto aboutScene = std::make_shared<BasicTextScene>(
        "About Scenes",
        "Scenes represent different screens or states in your app. Each scene can have sprites, actors, and handle input independently. They form the foundation of your user interface."
      );
      sceneManager->pushScene(aboutScene);
    }),
    
    MenuSystem::createAction("Scene Lifecycle", []() {
      Serial.println("📖 EDUCATIONAL: Showing scene lifecycle explanation");
      auto lifecycleScene = std::make_shared<BasicTextScene>(
        "Scene Lifecycle",
        "Scenes go through: initialize() -> enter() -> update()/render() loop -> exit() -> destroy(). Use these methods to manage resources properly."
      );
      sceneManager->pushScene(lifecycleScene);
    }),
    
    MenuSystem::createAction("Best Practices", []() {
      Serial.println("📖 EDUCATIONAL: Showing best practices guide");
      auto practicesScene = std::make_shared<BasicTextScene>(
        "Best Practices",
        "1. Always call parent methods first/last. 2. Keep render() fast and efficient. 3. Handle input appropriately. 4. Clean up resources in exit()."
      );
      sceneManager->pushScene(practicesScene);
    }),
    
    MenuSystem::createAction("Restart Demo", []() {
      Serial.println("📖 LESSON 4: Restarting tutorial from beginning");
      // Clear all scenes and start over
      sceneManager->clearScenes();
      auto welcomeScene = createWelcomeScene();
      sceneManager->pushScene(welcomeScene);
    })
  };
  
  // Configure menu appearance and behavior
  MenuSystem::MenuOptions menuOptions;
  menuOptions.x = 10;              // 10 pixels from left edge
  menuOptions.y = 15;              // 15 pixels from top
  menuOptions.itemHeight = 12;     // 12 pixels per menu item
  menuOptions.pointer = '>';       // Use '>' as selection pointer
  menuOptions.font = Display::Font::intel_one_mono_8_pt;
  
  // Create the menu system
  auto menuSystem = std::make_unique<MenuSystem>(engine.display, menuItems, menuOptions);
  menuSystem->scrollOptions.setVisibleItems(10);  // Show up to 10 items at once
  
  Serial.println("💡 CONCEPT: SceneFactory creates standardized scene types");
  return SceneFactory::createMenuScene("MainMenu", std::move(menuSystem));
}

//==============================================================================
// Debug Event Handler - for checking if clock is working
//==============================================================================

class DebugEventHandler : public Actor::Actor {
public:
  void handle(::Actor::Message* message) override {
    switch (message->signal) {
      case Kywy::Events::TICK:
        // Debug: Print tick every few seconds to verify clock is working
        static int tickCount = 0;
        tickCount++;
        if (tickCount % 180 == 0) {  // Every ~3 seconds at 60fps
          Serial.println("📖 DEBUG: Clock TICK events are being received");
        }
        break;
    }
  }
} debugEventHandler;

//==============================================================================
// LESSON 4: Scene Event Monitoring (Advanced Topic)
// Learn how to observe and respond to scene management events
//==============================================================================

class LearningEventHandler : public Actor::Actor {
public:
  void handle(::Actor::Message* message) override {
    // This handler receives notifications about scene management events
    // It's useful for debugging, logging, and understanding the scene lifecycle
    
    switch (message->signal) {
      case Kywy::SceneEvents::SCENE_CHANGED:
        Serial.println("📖 EVENT: Scene changed - new scene is now active");
        Serial.println("💡 CONCEPT: SCENE_CHANGED fires when the active scene switches");
        break;
        
      case Kywy::SceneEvents::SCENE_PUSHED:
        Serial.println("📖 EVENT: Scene pushed - added to scene stack");
        Serial.println("💡 CONCEPT: SCENE_PUSHED fires when pushScene() is called");
        break;
        
      case Kywy::SceneEvents::SCENE_POPPED:
        Serial.println("📖 EVENT: Scene popped - removed from scene stack");
        Serial.println("💡 CONCEPT: SCENE_POPPED fires when popScene() is called");
        break;
        
      case Kywy::SceneEvents::TRANSITION_STARTED:
        Serial.println("📖 EVENT: Transition started - scene animation beginning");
        Serial.println("💡 CONCEPT: TRANSITION_STARTED fires when visual transition begins");
        break;
        
      case Kywy::SceneEvents::TRANSITION_COMPLETED:
        Serial.println("📖 EVENT: Transition completed - scene animation finished");
        Serial.println("💡 CONCEPT: TRANSITION_COMPLETED fires when scene is fully visible");
        break;
        
      default:
        // Handle any other scene-related events
        Serial.print("📖 EVENT: Unknown scene event: ");
        Serial.println(message->signal);
        break;
    }
  }
} learningEventHandler;

//==============================================================================
// SETUP AND MAIN LOOP
// This is where everything comes together
//==============================================================================

void setup() {
  Serial.begin(9600);
  delay(1000);  // Give serial time to initialize
  
  Serial.println("=====================================");
  Serial.println("    KYWY SCENE MANAGER TUTORIAL");
  Serial.println("=====================================");
  Serial.println();
  Serial.println("Welcome to the Scene Manager learning experience!");
  Serial.println("This tutorial will teach you:");
  Serial.println("• Scene creation and lifecycle");
  Serial.println("• Input handling across scenes");
  Serial.println("• Scene navigation and stacking");
  Serial.println("• Menu integration");
  Serial.println("• Best practices for scene management");
  Serial.println();
  Serial.println("📚 INSTRUCTIONS:");
  Serial.println("1. Keep this Serial Monitor open for tips!");
  Serial.println("2. Use D-pad UP/DOWN to navigate menus");
  Serial.println("3. Use RIGHT button to select/confirm");
  Serial.println("4. Use LEFT button to go back");
  Serial.println("5. Follow lessons in order for best learning");
  Serial.println();
  
  // Initialize the engine
  Serial.println("🔧 SETUP: Initializing Kywy engine...");
  engine.start();
  
  // Create and initialize scene manager
  Serial.println("🎬 SETUP: Creating Scene Manager...");
  sceneManager = std::make_unique<SceneManager>(engine);
  sceneManager->initialize();
  sceneManager->start();
  
  // Subscribe to engine events for updates and input
  Serial.println("🔗 SETUP: Connecting Scene Manager to engine events...");
  sceneManager->subscribe(&engine.input);
  sceneManager->subscribe(&engine.clock);
  
  // Subscribe to scene events for learning (optional)
  learningEventHandler.subscribe(sceneManager.get());
  learningEventHandler.start();
  
  // Subscribe debug handler to clock for tick monitoring
  debugEventHandler.subscribe(&engine.clock);
  debugEventHandler.start();
  
  // Start with the welcome scene
  Serial.println("🎯 SETUP: Starting with welcome scene...");
  auto welcomeScene = createWelcomeScene();
  sceneManager->pushScene(welcomeScene);
  
  Serial.println("✅ SETUP: Complete! Enjoy learning about Scene Management!");
  Serial.println();
  Serial.println("💡 The welcome screen will appear for 2 seconds, then the main menu.");
  Serial.println("💡 Try each demo scene to learn different concepts step by step.");
  Serial.println();
}

void loop() {
  // The Scene Manager handles everything through the Actor system
  // This is one of the powerful features - your main loop stays simple!
  delay(16);  // ~60 FPS (16ms ≈ 60 Hz)
  
  // Optional: Add periodic learning tips
  static unsigned long lastTip = 0;
  static int tipIndex = 0;
  
  if (millis() - lastTip > 45000) {  // Every 45 seconds
    lastTip = millis();
    
    const char* tips[] = {
      "💡 TIP: Each scene has its own state and can handle input independently.",
      "💡 TIP: The Scene Manager automatically handles resource management.",
      "💡 TIP: Menu scenes integrate seamlessly with the scene stack system.",
      "💡 TIP: Use overlay scenes for dialogs and temporary information.",
      "💡 TIP: Scene events help you debug and understand the lifecycle.",
      "💡 TIP: Keep render() methods fast - they're called every frame!",
      "💡 TIP: Use enter() and exit() for setup and cleanup operations.",
      "💡 TIP: Scene stack allows you to return to previous scenes easily.",
      "🎓 CONGRATS: You're learning scene management - try building your own scenes!"
    };
    
    int numTips = sizeof(tips) / sizeof(tips[0]);
    Serial.println(tips[tipIndex % numTips]);
    tipIndex++;
  }
}

//==============================================================================
// 🎓 CONGRATULATIONS!
//==============================================================================
//
// If you've made it this far, you've learned the fundamentals of scene management!
//
// WHAT YOU'VE LEARNED:
// ✅ How to create custom scene classes
// ✅ Scene lifecycle methods (enter, render, handleInput, exit)
// ✅ Animation and state management within scenes
// ✅ Comprehensive input handling
// ✅ Scene navigation and stacking
// ✅ Menu integration with scenes
// ✅ Event handling and debugging
//
// NEXT STEPS:
// 1. Try modifying the existing scenes to see how changes affect behavior
// 2. Create your own custom scene with unique functionality
// 3. Build a complete application using multiple scenes
// 4. Explore advanced features like overlay scenes
// 5. Check out other Kywy examples to see scenes used in real games
//
// RESOURCES:
// • examples/basics/SceneManager/README.md - Detailed tutorial guide
// • examples/utility/SceneTemplate/ - Template for creating new scenes
// • examples/games/ - See scenes used in complete games
// • src/SceneManager.hpp - Scene Manager source code and documentation
//
// Happy coding with Kywy! 🚀
//
//==============================================================================
