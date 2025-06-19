# Kywy Multi-Game Launcher

This directory contains two versions of a multi-game launcher that demonstrates the Kywy game engine's Scene Manager and MenuSystem capabilities.

## Version 1: Simple Launcher (SimpleLauncher.ino)

A streamlined demonstration with two simple games built directly into the launcher. This version is self-contained and easier to understand.

### Features
- Clean main menu interface
- Two demonstration games: Snake and Tennis
- Proper scene management with enter/exit handling
- Simple but effective game implementations

### Games Included
- **Snake Demo**: Basic snake game with movement and scoring
- **Tennis Demo**: Pong-style game with AI opponent

## Version 2: Full Launcher (Multilauncher.ino)

A more comprehensive system that wraps existing games as scenes. This version demonstrates how to integrate full-featured games into a launcher system.

### Features
- Scene wrapper system for existing games
- Factory pattern for game creation
- Exit callback system for returning to launcher
- Placeholder system for future game additions

### Games Included
- **Snake**: Full-featured wrapper (SnakeGameScene.hpp)
- **2D Tennis**: Complete tennis game wrapper (TennisGameScene.hpp)
- **Asteroids, Pong, Slime Jumper, Spelunker**: Placeholder implementations

## Controls

### Main Menu
- **D-Pad Up/Down**: Navigate menu
- **A Button (Right)**: Select game
- **B Button (Left)**: Exit launcher

### In Games
- **D-Pad**: Game-specific movement controls
- **A Button**: Confirm/Start/Restart
- **B Button**: Exit to launcher

## Architecture Overview

### Scene Management
Both versions demonstrate:
- Scene lifecycle management (enter/exit/render/update)
- Scene stacking and navigation
- Input routing to active scenes
- Resource cleanup when switching scenes

### Simple Launcher Architecture
```
Engine
├── SceneManager
    ├── LauncherMenuScene (main menu)
    ├── SimpleSnakeGameScene (self-contained snake)
    └── SimpleTennisGameScene (self-contained tennis)
```

### Full Launcher Architecture
```
Engine
├── SceneManager
    ├── MultiLauncherScene (main menu)
    └── GameScenesWrapper (factory)
        ├── SnakeGameScene.hpp
        ├── TennisGameScene.hpp
        └── [Other game scenes...]
```

## File Structure

```
Multilauncher/
├── SimpleLauncher.ino             # Self-contained simple version
├── Multilauncher.ino              # Full wrapper-based version
├── GameScenesWrapper.hpp          # Game scene factory
├── SnakeGameScene.hpp             # Snake game wrapper
├── TennisGameScene.hpp            # Tennis game wrapper
├── AsteroidsGameScene.hpp         # Asteroids placeholder
├── PongGameScene.hpp              # Pong placeholder  
├── SlimeJumperGameScene.hpp       # Slime Jumper placeholder
├── SpelunkerGameScene.hpp         # Spelunker placeholder
└── README.md                      # This file
```

## Getting Started

### For Learning (Recommended)
Start with `SimpleLauncher.ino` to understand the basic concepts:
1. Scene creation and management
2. Menu systems
3. Input handling
4. Game loop integration

### For Integration
Use `Multilauncher.ino` as a template for integrating existing games:
1. Study the wrapper pattern in `SnakeGameScene.hpp`
2. Create similar wrappers for your games
3. Add them to `GameScenesWrapper.hpp`
4. Update the main menu in `Multilauncher.ino`

## Educational Value

### Scene Management Concepts
- **Scene Lifecycle**: Understanding enter/exit/render/update patterns
- **State Management**: How scenes maintain their own state
- **Navigation**: Stack-based scene management
- **Resource Management**: Proper cleanup and initialization

### Menu System Integration
- **Menu Creation**: Building navigable menus with callbacks
- **Input Handling**: Routing input to appropriate handlers
- **Visual Feedback**: Providing user feedback for menu interactions

### Game Architecture
- **Modularization**: Breaking games into reusable scenes
- **Callback Systems**: Handling exit and transition events
- **State Isolation**: Keeping game state separate from launcher state

## Implementation Notes

### Simple Version Benefits
- Easier to understand and modify
- Self-contained game logic
- Minimal file dependencies
- Good for learning and prototyping

### Full Version Benefits
- Demonstrates professional code organization
- Shows how to wrap existing code
- Provides template for larger projects
- Supports easy addition of new games

### Memory Considerations
Both versions are designed to work within the constraints of embedded systems:
- Dynamic scene creation/destruction
- Minimal memory footprint
- Efficient resource management

## Building and Running

1. Ensure the Kywy library is properly installed
2. Choose either `SimpleLauncher.ino` or `Multilauncher.ino`
3. Open in Arduino IDE
4. Select appropriate board and port
5. Upload to your Kywy device

## Troubleshooting

### Common Issues
- **Menu not responding**: Check input event routing
- **Scene transitions fail**: Verify scene manager initialization
- **Display corruption**: Ensure proper display.clear() calls
- **Memory issues**: Monitor dynamic allocation in game scenes

### Debug Tips
- Use Serial output to trace scene transitions
- Add visual indicators for active scenes
- Monitor memory usage if experiencing crashes
- Test individual scenes before integration

This multi-launcher serves as both a practical application and a comprehensive educational example of advanced Kywy game engine features.
