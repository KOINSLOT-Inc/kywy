---
title: "Part 4: Simple Nested If Statements"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 4: Simple Nested If Statements

Nested if statements let you create different paths in your game. Let's look at a simple 2-level nested structure where your first choice determines what happens in your second choice.

## What is a 2-Level Nested If?

A 2-level nested if has:
- **Level 1**: Your first choice (which path to take)
- **Level 2**: Different options based on your first choice

## Simple 2-Level Example

Here's how to create different screens and choices based on your first decision:

```cpp
void loop() {
  // Show start screen
  engine.display.clear();
  engine.display.drawBitmap(0, 0, KYWY_DISPLAY_WIDTH, KYWY_DISPLAY_HEIGHT, startScreen);
  drawInstructionText();
  engine.display.update();

  ButtonEvent choice = waitForInput();

  // LEVEL 1: First choice determines the path
  if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
    // PATH A: Forest path
    engine.display.clear();
    drawColoredScreen(0x07E0); // Green for forest
    drawBottomText("You enter a peaceful forest.\nLeft: Follow the stream\nRight: Climb the hill");
    engine.display.update();

    // LEVEL 2: Second choice depends on being in the forest
    choice = waitForInput();

    if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
      // Forest + Stream = Find treasure
      engine.display.clear();
      drawColoredScreen(0x07FF); // Cyan
      score += 20;
      drawBottomText("You follow the stream and find treasure!\n(+20 points)");
    } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
      // Forest + Hill = Get lost
      engine.display.clear();
      drawColoredScreen(0xF800); // Red
      health -= 10;
      drawBottomText("You climb the hill but get lost!\n(-10 health)");
    }

  } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
    // PATH B: Mountain path
    engine.display.clear();
    drawColoredScreen(0x2945); // Dark for mountain
    drawBottomText("You climb a steep mountain.\nLeft: Look for a cave\nRight: Cross the bridge");
    engine.display.update();

    // LEVEL 2: Second choice depends on being on the mountain
    choice = waitForInput();

    if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
      // Mountain + Cave = Find shelter
      engine.display.clear();
      drawColoredScreen(0x001F); // Blue
      health += 15;
      drawBottomText("You find a cave with shelter!\n(+15 health)");
    } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
      // Mountain + Bridge = Dangerous crossing
      engine.display.clear();
      drawColoredScreen(0xF800); // Red
      health -= 20;
      score += 10;
      drawBottomText("The bridge is unstable!\n(+10 points, -20 health)");
    }
  }

  // Loop back to start
}
```

## How It Works

**Level 1 Choice:**
- **Left**: Takes you to the forest path (green screen)
- **Right**: Takes you to the mountain path (dark screen)

**Level 2 Choices:**
- **Forest Path**: Stream (treasure) or Hill (get lost)
- **Mountain Path**: Cave (shelter) or Bridge (dangerous)

Each path has its own unique screen color and consequences!

## Your Challenge

1. **Add the nested structure** to your existing game
2. **Test both paths**:
   - Forest → Stream: Should give +20 points
   - Forest → Hill: Should give -10 health
   - Mountain → Cave: Should give +15 health
   - Mountain → Bridge: Should give +10 points, -20 health

**Questions to consider:**
- Which path gives the best results?
- Which path gives the worst results?
- How does the first choice affect your second choice options?

## Why Use Nested Ifs?

**Pros:**
- Different screens for different paths
- More interesting story branches
- Each choice feels more meaningful

**Cons:**
- Code gets a bit more complex
- Harder to see all possible paths

This is completely optional - your simple sequential choices work great too! In the next part, we'll learn how to add custom sprites to make your game look amazing.
        score += 40;
        health -= 30;
        drawBottomText("With sword and strength, you charge the dragon!\n(+40 points, -30 health)");
      } else {
        // Wounded with sword - defensive
        drawColoredScreen(0xFFE0); // Yellow
        score += 25;
        health -= 10;
        drawBottomText("Your sword helps, but you're careful...\n(+25 points, -10 health)");
      }
    } else {
      // No sword path with health check
      if (health > 40) {
        // Healthy without sword - risky
        drawColoredScreen(0xF800); // Red
        score += 15;
        health -= 25;
        drawBottomText("No weapon but you're strong! Barely survive...\n(+15 points, -25 health)");
      } else {
        // Wounded without sword - desperate
        drawColoredScreen(0x2945); // Dark
        score += 5;
        health -= 15;
        drawBottomText("Unprepared and hurt... you struggle on.\n(+5 points, -15 health)");
      }
    }
  } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
    // Second right choice with nested logic
    engine.display.clear();
    if (metWizard) {
      // Wizard path with score check
      if (score > 30) {
        // High score with wizard - magical
        drawColoredScreen(0x07FF); // Cyan
        health += 20;
        score += 20;
        drawBottomText("The wizard rewards your success!\n(+20 health, +20 points)");
      } else {
        // Low score with wizard - helpful
        drawColoredScreen(0x001F); // Blue
        health += 15;
        score += 10;
        drawBottomText("The wizard aids your journey!\n(+15 health, +10 points)");
      }
    } else {
      // No wizard path with health check
      if (health > 50) {
        // Healthy exploration
        drawColoredScreen(0x07E0); // Green
        score += 25;
        drawBottomText("You discover a hidden path!\n(+25 points)");
      } else {
        // Wounded exploration - risky
        drawColoredScreen(0xF800); // Red
        score += 10;
        health -= 10;
        drawBottomText("The path is treacherous...\n(+10 points, -10 health)");
      }
    }
  }

  // THIRD CHOICE - Complex nested conditions
  choice = waitForInput();

  if (choice == Kywy::Events::KywyEvents::D_PAD_LEFT_PRESSED) {
    // Third left choice - multiple nested conditions
    engine.display.clear();
    if (hasSword && metWizard) {
      // Best case scenario
      if (health > 70 && score > 50) {
        drawColoredScreen(0x07E0);
        score += 50;
        drawBottomText("Perfect combination! Legendary victory!\n(+50 points) EPIC WIN!");
      } else if (health > 40) {
        drawColoredScreen(0x07FF);
        score += 35;
        health += 10;
        drawBottomText("Sword and wizard magic prevail!\n(+35 points, +10 health)");
      } else {
        drawColoredScreen(0xFFE0);
        score += 20;
        drawBottomText("Helpful allies, but you're wounded...\n(+20 points)");
      }
    } else if (hasSword || metWizard) {
      // Partial advantages
      if (health > 60) {
        drawColoredScreen(0x07E0);
        score += 30;
        drawBottomText("One advantage helps you succeed!\n(+30 points)");
      } else {
        drawColoredScreen(0xFFE0);
        score += 15;
        health -= 5;
        drawBottomText("Some help, but it's challenging...\n(+15 points, -5 health)");
      }
    } else {
      // No advantages - tough path
      if (health > 30) {
        drawColoredScreen(0xF800);
        score += 10;
        health -= 20;
        drawBottomText("No help and it's dangerous...\n(+10 points, -20 health)");
      } else {
        drawColoredScreen(0x2945);
        health -= 15;
        if (health <= 0) {
          drawBottomText("Completely unprepared... you fall.\nGAME OVER!");
        } else {
          drawBottomText("Barely survive through determination!\n(-15 health)");
        }
      }
    }
  } else if (choice == Kywy::Events::KywyEvents::D_PAD_RIGHT_PRESSED) {
    // Third right choice - alternative path
    engine.display.clear();
    if (metWizard && score > 40) {
      // Wizard rewards high achievers
      drawColoredScreen(0x001F);
      health += 15;
      score += 25;
      drawBottomText("The wizard is impressed by your achievements!\n(+15 health, +25 points)");
    } else if (hasSword && health > 50) {
      // Sword provides alternative victory
      drawColoredScreen(0x07E0);
      score += 30;
      drawBottomText("Your sword carves a path to victory!\n(+30 points)");
    } else {
      // Default path
      drawColoredScreen(0x2945);
      score += 15;
      drawBottomText("You find an alternative route!\n(+15 points)");
    }
  }

  // FOURTH CHOICE - Show final results with nested win/lose
  choice = waitForInput();

  engine.display.clear();
  if (health <= 0) {
    // LOSE CONDITION
    drawColoredScreen(0xF800); // Red for defeat
    char loseText[64];
    sprintf(loseText, "GAME OVER!\nFinal Score: %d\nHealth: %d", score, health);
    drawBottomText(loseText);
  } else {
    // WIN CONDITIONS - Nested based on score and health
    if (score >= 80 && health > 50) {
      drawColoredScreen(0x07E0); // Perfect victory
      char winText[64];
      sprintf(winText, "LEGENDARY HERO!\nScore: %d Health: %d", score, health);
      drawBottomText(winText);
    } else if (score >= 60) {
      drawColoredScreen(0x07FF); // Good victory
      char winText[64];
      sprintf(winText, "VICTORIOUS!\nScore: %d Health: %d", score, health);
      drawBottomText(winText);
    } else if (health > 30) {
      drawColoredScreen(0xFFE0); // Partial victory
      char winText[64];
      sprintf(winText, "SURVIVED!\nScore: %d Health: %d", score, health);
      drawBottomText(winText);
    } else {
      drawColoredScreen(0xF800); // Pyrrhic victory
      char winText[64];
      sprintf(winText, "VICTORY... at great cost.\nScore: %d Health: %d", score, health);
      drawBottomText(winText);
    }
  }

  // Loop back to start for new adventure
}
```

## Pros of Nested If Statements

### ✅ More Dynamic Gameplay
Choices change based on player state, creating replayability:
- Healthy players get aggressive options
- Wounded players get defensive options
- High-score players get riskier rewards

### ✅ Realistic Consequences
Decisions have lasting impact:
- Previous choices affect future options
- Health/score influence available paths
- Creates meaningful risk/reward systems

### ✅ Complex Storytelling
Enables sophisticated narratives:
- Character development through choices
- Branching storylines
- Multiple paths to the same ending

### ✅ Player Agency
Players feel their choices matter:
- Different playthroughs feel unique
- Strategic decision making
- Personal investment in outcomes

## Cons of Nested If Statements

### ❌ Code Complexity
Harder to read and maintain:
- Deep nesting creates "arrow code"
- Logic becomes harder to follow
- Debugging is more difficult

### ❌ Limited Scalability
Gets unwieldy with many conditions:
- Too many nested levels confuse logic
- Hard to add new features
- Code becomes brittle

### ❌ Testing Challenges
More combinations to test:
- Every health/score combination needs testing
- Edge cases multiply
- Regression testing becomes complex

## Best Practices for Nested Ifs

### 1. Limit Nesting Depth
Keep it to 2-3 levels maximum:
```cpp
// Good: Shallow nesting
if (mainCondition) {
  if (subCondition) {
    // Action
  }
}

// Bad: Deep nesting
if (condition1) {
  if (condition2) {
    if (condition3) {
      if (condition4) {
        // Too deep!
      }
    }
  }
}
```

### 2. Use Early Returns
Exit early to reduce nesting:
```cpp
// Instead of deep nesting
if (health > 0) {
  if (score > 10) {
    if (hasSword) {
      // Deep nesting
    }
  }
}

// Use early returns
if (health <= 0) return; // Dead - exit early
if (score <= 10) return; // Not enough score - exit early
if (!hasSword) return;   // No sword - exit early
// Now we know player is alive, has score, and has sword
// Continue with main logic
```

### 3. Extract Complex Logic
Move nested logic to helper functions:
```cpp
void handleHealthyPlayerChoices() {
  // All healthy player logic here
}

void handleWoundedPlayerChoices() {
  // All wounded player logic here
}

// Main loop becomes cleaner
if (health > 50) {
  handleHealthyPlayerChoices();
} else {
  handleWoundedPlayerChoices();
}
```

## Your Challenge: Implement Nested Logic

1. **Add health-based choice variations** to your existing game
2. **Create score-dependent rewards** for risky choices
3. **Implement different win conditions** based on final health/score
4. **Test different health levels** to see how choices change

**Questions to explore:**
- How does nested logic change player strategy?
- When does nesting become too complex?
- How can you balance dynamic choices with code maintainability?

In the next part, we'll learn how to add your own custom sprites using the online HPP generator tool!
