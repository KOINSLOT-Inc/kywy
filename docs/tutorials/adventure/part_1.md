---
title: "Part 1: Introduction to Choose Your Own Adventure"
---

<!-- hide previous page button that links outside of tutorial -->
<style>
  .md-footer__link--prev:not([hidden]) { display: none }
</style>

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 1: Introduction to Choose Your Own Adventure

Welcome to the Choose Your Own Adventure tutorial! This tutorial will teach you how to create interactive stories and games using the Kywy platform. We'll build upon the existing Adventure game example and learn how to extend it with new features.

## What You'll Learn

- How to create interactive stories with multiple choices
- Using helper functions to organize your code
- Handling user input with d-pad controls
- Adding story text and visual feedback
- Extending games with new screens and choices

## What is the Adventure Game?

The Adventure game is a simple choose-your-own-adventure style game where players make choices that determine their story path. Currently, it has:

- A start screen with choice options
- Two different story paths (left and right)
- Simple text feedback for each choice
- A loop that returns to the beginning

## Getting Started

First, let's look at the basic structure of the Adventure game. Open `examples/games/Adventure/Adventure.ino` in your code editor.

```cpp
// Choose your own adventure game
//
// This example shows:
//   -How to use if/else statements to make decisions
//   -How to use d-pad to navigate between screens
//   -How to draw pictures and text on screen
//   -How to add story text to game screens
//   -How to create and use helper functions for code reuse

#include "Kywy.hpp"    // Main kywy library for controlling the KYWY
#include "screens.hpp" // This file includes all of our images for the game

Kywy::Engine engine;  // This starts the kywy firmware and engine
```

## Basic Game Structure

The game follows a simple pattern:

1. **Setup**: Initialize the engine and prepare the game
2. **Main Loop**: Show screens, wait for input, respond to choices
3. **Helper Functions**: Reusable code for common tasks

## Your First Task

Try running the Adventure game on your Kywy device. Use the d-pad left and right to make choices and see how the story changes.

In the next part, we'll explore the code structure and learn how to modify the existing choices!
