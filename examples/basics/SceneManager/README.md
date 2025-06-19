# Scene Manager Tutorial

## 📚 Learning Objectives

This comprehensive tutorial teaches you how to use the Kywy Scene Manager system to create sophisticated multi-screen applications. By the end of this tutorial, you'll understand:

1. **Scene Concepts**: What scenes are and how they work in interactive applications
2. **Scene Lifecycle**: How scenes are created, managed, and destroyed
3. **Scene Navigation**: How to navigate between scenes using the scene stack
4. **Menu Integration**: How to combine menus with scene management
5. **Input Handling**: How scenes process user input independently
6. **State Management**: How to maintain state across scene changes
7. **Best Practices**: Professional patterns for scene-based applications

## 🎯 What You'll Build

This example creates an interactive learning environment with:

- **Welcome Splash Screen** - Shows how splash scenes work with auto-navigation
- **Main Menu** - Demonstrates menu integration with scenes and navigation
- **Text Demo Scene** - Basic scene with text display and input handling
- **Animation Demo Scene** - Scene with real-time animations and state management
- **Input Demo Scene** - Comprehensive input handling with visual feedback
- **Educational Content** - Detailed information about scene concepts and best practices

## 🏗️ Code Structure

### Core Components

```cpp
// Scene Manager - manages all scenes
std::unique_ptr<SceneManager> sceneManager;

// Engine - provides display, input, and other systems
Engine engine;
```

### Scene Types Demonstrated

1. **BasicTextScene** - Custom scene class showing fundamental concepts
2. **AnimationDemoScene** - Scene with real-time updates and animation
3. **InputDemoScene** - Comprehensive input handling
4. **MenuScene** - Automatically created by SceneFactory
5. **SplashScene** - Automatically created by SceneFactory

## 📖 Lessons Breakdown

### Lesson 1: Basic Scene Creation

**File Location**: `BasicTextScene` class

**What You'll Learn**:
- How to inherit from the Scene base class
- Scene lifecycle methods (enter, render, handleInput, exit)
- Basic text rendering and word wrapping
- Simple input handling

**Key Concepts**:
```cpp
class BasicTextScene : public Scene {
  void enter() override { /* Called when scene becomes active */ }
  void render(Display::Display& display) override { /* Draw scene content */ }
  void handleInput(int signal, void* data) override { /* Process input */ }
  void exit() override { /* Called when scene becomes inactive */ }
};
```

### Lesson 2: Animation and State Management

**File Location**: `AnimationDemoScene` class

**What You'll Learn**:
- How to maintain scene state
- Real-time updates with the update() method
- Creating simple animations
- Controlling animation state with input

**Key Concepts**:
```cpp
void update(float deltaTime) override {
  // Update animations, timers, game logic
  if (animationEnabled) {
    timer += deltaTime;
    // Update animation state
  }
}
```

### Lesson 3: Input Handling

**File Location**: `InputDemoScene` class

**What You'll Learn**:
- Handling all input types (buttons and D-pad)
- Tracking input state
- Creating interactive responses to input
- Input-driven state changes

**Key Input Events**:
- `Kywy::Events::BUTTON_LEFT_PRESSED`
- `Kywy::Events::BUTTON_RIGHT_PRESSED`
- `Kywy::Events::D_PAD_UP_PRESSED`
- `Kywy::Events::D_PAD_DOWN_PRESSED`
- `Kywy::Events::D_PAD_LEFT_PRESSED`
- `Kywy::Events::D_PAD_RIGHT_PRESSED`

### Lesson 4: Scene Creation Functions

**File Location**: `createWelcomeScene()`, `createMainMenu()`, etc.

**What You'll Learn**:
- How to organize scene creation code
- Using SceneFactory for common scene types
- Setting up scene callbacks and navigation
- Menu system integration

**Scene Factory Usage**:
```cpp
// Create splash scene
auto splash = SceneFactory::createSplashScene(name, data, width, height, duration);

// Create menu scene
auto menu = SceneFactory::createMenuScene(name, std::move(menuSystem));
```

### Lesson 5: Scene Events (Advanced)

**File Location**: `LearningEventHandler` class

**What You'll Learn**:
- How to listen for scene management events
- Understanding the scene lifecycle from an observer perspective
- Debugging scene changes

**Available Events**:
- `SceneEvents::SCENE_CHANGED`
- `SceneEvents::SCENE_PUSHED`
- `SceneEvents::SCENE_POPPED`

## 🎮 How to Use

### Setup Instructions

1. **Open the Serial Monitor** in the Arduino IDE (Tools → Serial Monitor)
2. **Set baud rate to 9600** for educational messages
3. **Upload the sketch** to your Kywy device
4. **Watch both the display and Serial Monitor** for the full learning experience

### Controls

- **D-Pad Up/Down**: Navigate menu items
- **Right Button**: Select menu item / Confirm action
- **Left Button**: Go back / Cancel (note: Input Demo requires 2 presses)

### Educational Features

- **Real-time Serial Monitor feedback** with educational messages for every action
- **Step-by-step lesson progression** from basic concepts to advanced features
- **Detailed code comments** explaining every concept as you encounter it
- **Interactive demonstrations** that respond to your input and show concepts in action
- **Periodic learning tips** displayed automatically while the program runs
- **Complete lifecycle tracking** so you can see exactly what happens when

### Navigation Flow

1. **Welcome Screen** (2 seconds) → **Main Menu**
2. **Main Menu** → Choose from:
   - **Text Demo** (Lesson 1: Basic scene creation)
   - **Animation Demo** (Lesson 2: State and animation)
   - **Input Demo** (Lesson 3: Comprehensive input handling)
   - **Learning content** (Educational explanations)
3. **Any Scene** → Press LEFT to return to previous scene

## 🔧 Technical Details

### Scene Lifecycle

```
initialize() → enter() → [update()/render() loop] → exit() → destroy()
```

### Memory LCD Optimization

This example is optimized for the Kywy's black-on-white memory LCD:
- Efficient pixel-perfect rendering
- Minimal memory usage
- Sharp text and graphics
- Fast scene switching optimized for 1-bit display

## 💡 Study Tips

1. **Start with Serial Monitor**: Open the Serial Monitor to see educational messages as you navigate - this provides real-time explanations of what's happening
2. **Follow the Lesson Order**: Start with Text Demo (Lesson 1), then Animation Demo (Lesson 2), then Input Demo (Lesson 3), for the best learning progression
3. **Read the Serial Feedback**: Every action you take generates educational feedback in the Serial Monitor explaining the concepts
4. **Examine Each Lesson**: Look at the code for each scene type to understand the patterns and best practices
5. **Experiment**: Modify the scenes to see how changes affect behavior - the educational framework will help you understand what's happening
5. **Read the Comments**: The code is heavily commented to explain concepts as you encounter them
7. **Watch the Event Messages**: The advanced event handler shows you the complete scene lifecycle in real-time
8. **Read the Comments**: The code is heavily commented to explain concepts as you encounter them

## 🎓 Learning Experience Features

### Real-Time Education
- **Live Serial Monitor feedback** for every action you take
- **Contextual explanations** that appear exactly when you need them
- **Step-by-step guidance** through each concept
- **Automatic tips** that appear periodically while you explore

### Progressive Difficulty
- **Lesson 1**: Basic scene creation and lifecycle
- **Lesson 2**: Animation and state management
- **Lesson 3**: Comprehensive input handling
- **Lesson 4**: Scene creation and menu integration

### Interactive Demonstrations
- **Animation Demo**: See how scenes can animate and manage state
- **Input Demo**: Experience comprehensive input handling with visual feedback
- **Event Monitoring**: Watch the complete scene lifecycle in action

## 🚀 Next Steps

After mastering this tutorial, you can:

1. **Create Your Own Scenes**: Use the patterns shown to build custom scenes
2. **Build Complete Apps**: Combine multiple scenes into full applications
3. **Add Sprites and Actors**: Enhance scenes with moving sprites and game logic
4. **Create Games**: Use scene management for game states (menu → game → game over)
4. **Advanced Features**: Explore overlay scenes and resource management

## 📝 Exercise Ideas

Try these modifications to deepen your understanding:

1. **Custom Scene**: Create a scene that displays your name and favorite color
2. **Timer Scene**: Make a scene with a countdown timer
3. **Drawing Scene**: Create a simple drawing scene using D-pad input
4. **Settings Scene**: Build a settings menu that remembers preferences
5. **Game Scene**: Create a simple game like a moving dot you can control

## 🐛 Common Issues and Solutions

### Scene Not Responding to Input
- Check that `handleInput()` is implemented
- Verify the correct event constants are used
- Make sure the scene is active (not paused)

### Animation Not Smooth
- Check that `update()` is being called
- Verify deltaTime is being used correctly
- Ensure the scene manager is subscribed to clock events

### Menu Not Working
- Confirm the MenuSystem is created correctly
- Check that menu items have proper callbacks
- Verify the scene manager is handling menu scenes properly

Remember: The Scene Manager handles most of the complexity for you. Focus on creating great scene content and let the system handle the navigation and management!
