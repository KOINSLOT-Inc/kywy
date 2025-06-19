// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_UTILITY_SCENES
#define KYWY_LIB_UTILITY_SCENES 1

#include "SceneManager.hpp"
#include "MenuSystem.hpp"

namespace Kywy {

//==============================================================================
// Pause Scene - Universal pause overlay for games
//==============================================================================

class PauseScene : public Scene {
private:
  std::unique_ptr<MenuSystem> pauseMenu;
  std::function<void()> onResume;
  std::function<void()> onQuit;
  
public:
  PauseScene(std::function<void()> resumeCallback = nullptr, 
             std::function<void()> quitCallback = nullptr) 
    : Scene("PauseScene"), onResume(resumeCallback), onQuit(quitCallback) {
    isOverlay = true;
    pauseUnderlying = true;
    
    createPauseMenu();
  }
  
  void createPauseMenu() {
    std::vector<MenuSystem::MenuItem> pauseItems = {
      MenuSystem::createLabel("-- PAUSED --"),
      MenuSystem::createAction("Resume", [this]() {
        if (onResume) onResume();
        // Pop this pause scene to return to game
        // Scene manager will be accessed through global or passed reference
      }),
      MenuSystem::createAction("Settings", [this]() {
        // Create settings overlay on top of pause menu
        auto settingsScene = createSettingsOverlay();
        // Push settings scene
      }),
      MenuSystem::createAction("Quit to Menu", [this]() {
        if (onQuit) onQuit();
        // Return to main menu
      })
    };
    
    MenuSystem::MenuOptions menuOptions;
    menuOptions.x = 20;
    menuOptions.y = 50;
    menuOptions.itemHeight = 15;
    menuOptions.pointer = '>';
    menuOptions.font = Display::Font::intel_one_mono_8_pt;
    
    // Create temporary display reference - will be set properly when scene activates
    // pauseMenu = std::make_unique<MenuSystem>(display, pauseItems, menuOptions);
  }
  
  void render(Display::Display& display) override {
    // Draw semi-transparent overlay effect for memory LCD
    // Since we can't do true transparency, we'll use a border frame
    
    // Draw border frame
    for (int x = 15; x < KYWY_DISPLAY_WIDTH - 15; x++) {
      display.drawPixel(x, 35, BLACK);
      display.drawPixel(x, KYWY_DISPLAY_HEIGHT - 25, BLACK);
    }
    for (int y = 35; y < KYWY_DISPLAY_HEIGHT - 25; y++) {
      display.drawPixel(15, y, BLACK);
      display.drawPixel(KYWY_DISPLAY_WIDTH - 16, y, BLACK);
    }
    
    // Clear the inner area
    for (int x = 16; x < KYWY_DISPLAY_WIDTH - 16; x++) {
      for (int y = 36; y < KYWY_DISPLAY_HEIGHT - 26; y++) {
        display.drawPixel(x, y, WHITE);
      }
    }
    
    // Render pause menu
    if (pauseMenu) {
      pauseMenu->displayMenu();
    }
  }
  
  void handleInput(int signal, void* data) override {
    // Menu system handles its own input through MenuInputHandler
    // Additional pause-specific input can be handled here
    
    switch (signal) {
      case Events::BUTTON_LEFT_PRESSED:
        // ESC-like behavior - resume game
        if (onResume) onResume();
        break;
    }
  }
  
private:
  std::shared_ptr<Scene> createSettingsOverlay() {
    // Create a settings scene that overlays the pause menu
    class SettingsOverlayScene : public Scene {
    public:
      SettingsOverlayScene() : Scene("SettingsOverlay") {
        isOverlay = true;
        pauseUnderlying = true;
      }
      
      void render(Display::Display& display) override {
        // Draw settings overlay
        display.clear();
        
        Display::TextOptions textOpt;
        textOpt._color = BLACK;
        textOpt._font = Display::Font::intel_one_mono_8_pt;
        
        // Draw border
        for (int x = 10; x < KYWY_DISPLAY_WIDTH - 10; x++) {
          display.drawPixel(x, 25, BLACK);
          display.drawPixel(x, KYWY_DISPLAY_HEIGHT - 25, BLACK);
        }
        for (int y = 25; y < KYWY_DISPLAY_HEIGHT - 25; y++) {
          display.drawPixel(10, y, BLACK);
          display.drawPixel(KYWY_DISPLAY_WIDTH - 11, y, BLACK);
        }
        
        display.drawText(30, 45, "SETTINGS", textOpt);
        display.drawText(15, 65, "Sound: ON", textOpt);
        display.drawText(15, 80, "Vibration: OFF", textOpt);
        display.drawText(15, 95, "Difficulty: Medium", textOpt);
        display.drawText(15, 120, "Press any key to close", textOpt);
        
        display.update();
      }
      
      void handleInput(int signal, void* data) override {
        // Close settings overlay
        // Pop back to pause menu
      }
    };
    
    return std::make_shared<SettingsOverlayScene>();
  }
};

//==============================================================================
// Loading Scene - Universal loading screen
//==============================================================================

class LoadingScene : public Scene {
private:
  std::string loadingText;
  float progress = 0.0f;
  uint16_t timer = 0;
  uint16_t duration;
  std::function<void()> onComplete;
  bool showProgress;
  
public:
  LoadingScene(const std::string& text = "Loading...", 
               uint16_t durationMs = 2000,
               bool showProgressBar = true,
               std::function<void()> completeCallback = nullptr)
    : Scene("LoadingScene"), loadingText(text), duration(durationMs), 
      showProgress(showProgressBar), onComplete(completeCallback) {}
  
  void enter() override {
    Scene::enter();
    timer = 0;
    progress = 0.0f;
  }
  
  void update(float deltaTime) override {
    Scene::update(deltaTime);
    
    timer += static_cast<uint16_t>(deltaTime);
    progress = static_cast<float>(timer) / static_cast<float>(duration);
    
    if (progress >= 1.0f) {
      progress = 1.0f;
      if (onComplete) {
        onComplete();
      }
    }
  }
  
  void render(Display::Display& display) override {
    display.clear();
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    // Center the loading text
    int textWidth = loadingText.length() * 6; // Approximate character width
    int textX = (KYWY_DISPLAY_WIDTH - textWidth) / 2;
    display.drawText(textX, KYWY_DISPLAY_HEIGHT / 2 - 10, loadingText.c_str(), textOpt);
    
    if (showProgress) {
      // Draw progress bar
      int barWidth = 100;
      int barHeight = 8;
      int barX = (KYWY_DISPLAY_WIDTH - barWidth) / 2;
      int barY = KYWY_DISPLAY_HEIGHT / 2 + 10;
      
      // Draw progress bar border
      for (int x = barX; x < barX + barWidth; x++) {
        display.drawPixel(x, barY, BLACK);
        display.drawPixel(x, barY + barHeight - 1, BLACK);
      }
      for (int y = barY; y < barY + barHeight; y++) {
        display.drawPixel(barX, y, BLACK);
        display.drawPixel(barX + barWidth - 1, y, BLACK);
      }
      
      // Fill progress
      int fillWidth = static_cast<int>((barWidth - 2) * progress);
      for (int x = barX + 1; x < barX + 1 + fillWidth; x++) {
        for (int y = barY + 1; y < barY + barHeight - 1; y++) {
          display.drawPixel(x, y, BLACK);
        }
      }
      
      // Show percentage
      char percentText[8];
      snprintf(percentText, sizeof(percentText), "%d%%", static_cast<int>(progress * 100));
      display.drawText(barX + barWidth/2 - 10, barY + barHeight + 15, percentText, textOpt);
    }
    
    display.update();
  }
  
  void setProgress(float newProgress) {
    progress = std::min(std::max(newProgress, 0.0f), 1.0f);
  }
  
  void setLoadingText(const std::string& text) {
    loadingText = text;
  }
};

//==============================================================================
// Dialog Scene - Universal dialog/message box
//==============================================================================

class DialogScene : public Scene {
private:
  std::string title;
  std::string message;
  std::vector<std::string> buttons;
  int selectedButton = 0;
  std::function<void(int)> onButtonPressed;
  
public:
  DialogScene(const std::string& dialogTitle,
              const std::string& dialogMessage,
              const std::vector<std::string>& dialogButtons = {"OK"},
              std::function<void(int)> buttonCallback = nullptr)
    : Scene("DialogScene"), title(dialogTitle), message(dialogMessage), 
      buttons(dialogButtons), onButtonPressed(buttonCallback) {
    isOverlay = true;
    pauseUnderlying = true;
  }
  
  void render(Display::Display& display) override {
    // Draw dialog box background
    int boxWidth = KYWY_DISPLAY_WIDTH - 20;
    int boxHeight = 80;
    int boxX = 10;
    int boxY = (KYWY_DISPLAY_HEIGHT - boxHeight) / 2;
    
    // Clear dialog area
    for (int x = boxX; x < boxX + boxWidth; x++) {
      for (int y = boxY; y < boxY + boxHeight; y++) {
        display.drawPixel(x, y, WHITE);
      }
    }
    
    // Draw border
    for (int x = boxX; x < boxX + boxWidth; x++) {
      display.drawPixel(x, boxY, BLACK);
      display.drawPixel(x, boxY + boxHeight - 1, BLACK);
    }
    for (int y = boxY; y < boxY + boxHeight; y++) {
      display.drawPixel(boxX, y, BLACK);
      display.drawPixel(boxX + boxWidth - 1, y, BLACK);
    }
    
    Display::TextOptions textOpt;
    textOpt._color = BLACK;
    textOpt._font = Display::Font::intel_one_mono_8_pt;
    
    // Draw title
    display.drawText(boxX + 5, boxY + 15, title.c_str(), textOpt);
    
    // Draw message (simple word wrapping)
    display.drawText(boxX + 5, boxY + 30, message.c_str(), textOpt);
    
    // Draw buttons
    int buttonY = boxY + boxHeight - 20;
    int buttonSpacing = boxWidth / buttons.size();
    
    for (size_t i = 0; i < buttons.size(); i++) {
      int buttonX = boxX + static_cast<int>(i) * buttonSpacing + 5;
      
      // Highlight selected button
      if (static_cast<int>(i) == selectedButton) {
        display.drawText(buttonX - 2, buttonY, ">", textOpt);
      }
      
      display.drawText(buttonX + 5, buttonY, buttons[i].c_str(), textOpt);
    }
    
    display.update();
  }
  
  void handleInput(int signal, void* data) override {
    switch (signal) {
      case Events::D_PAD_LEFT_PRESSED:
        if (selectedButton > 0) {
          selectedButton--;
        }
        break;
        
      case Events::D_PAD_RIGHT_PRESSED:
        if (selectedButton < static_cast<int>(buttons.size()) - 1) {
          selectedButton++;
        }
        break;
        
      case Events::BUTTON_RIGHT_PRESSED:
        // Confirm button selection
        if (onButtonPressed) {
          onButtonPressed(selectedButton);
        }
        break;
        
      case Events::BUTTON_LEFT_PRESSED:
        // Cancel (typically button 0 or last button)
        if (onButtonPressed) {
          onButtonPressed(buttons.size() > 1 ? static_cast<int>(buttons.size()) - 1 : 0);
        }
        break;
    }
  }
};

//==============================================================================
// Utility Scene Factory
//==============================================================================

class UtilitySceneFactory {
public:
  static std::shared_ptr<PauseScene> createPauseScene(
    std::function<void()> onResume = nullptr,
    std::function<void()> onQuit = nullptr
  ) {
    return std::make_shared<PauseScene>(onResume, onQuit);
  }
  
  static std::shared_ptr<LoadingScene> createLoadingScene(
    const std::string& text = "Loading...",
    uint16_t duration = 2000,
    bool showProgress = true,
    std::function<void()> onComplete = nullptr
  ) {
    return std::make_shared<LoadingScene>(text, duration, showProgress, onComplete);
  }
  
  static std::shared_ptr<DialogScene> createDialogScene(
    const std::string& title,
    const std::string& message,
    const std::vector<std::string>& buttons = {"OK"},
    std::function<void(int)> onButton = nullptr
  ) {
    return std::make_shared<DialogScene>(title, message, buttons, onButton);
  }
  
  // Convenience methods for common dialogs
  static std::shared_ptr<DialogScene> createConfirmDialog(
    const std::string& message,
    std::function<void(bool)> onConfirm = nullptr
  ) {
    return std::make_shared<DialogScene>(
      "Confirm",
      message,
      std::vector<std::string>{"Yes", "No"},
      [onConfirm](int buttonIndex) {
        if (onConfirm) {
          onConfirm(buttonIndex == 0);
        }
      }
    );
  }
  
  static std::shared_ptr<DialogScene> createErrorDialog(
    const std::string& errorMessage,
    std::function<void()> onOK = nullptr
  ) {
    return std::make_shared<DialogScene>(
      "Error",
      errorMessage,
      std::vector<std::string>{"OK"},
      [onOK](int) {
        if (onOK) onOK();
      }
    );
  }
};

}  // namespace Kywy

#endif  // KYWY_LIB_UTILITY_SCENES
