---
title: "Cookie Clicker Tutorial"
hide:
  - footer
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Cookie Clicker Tutorial

_audience: beginner (middle school age and up)_

Learn programming fundamentals by building a fun Cookie Clicker game! This tutorial teaches you how to use variables, handle input, and create simple animations.

## What You'll Learn

- **Variables**: How to store and change information (like your cookie count)
- **Input Handling**: How to detect when buttons are pressed
- **Drawing**: How to put pictures and text on the screen
- **Animation**: How to make things move and change over time
- **Game Loops**: How games work behind the scenes

## What You'll Build

A simple Cookie Clicker game where:

- You press the RIGHT button to click a cookie
- Each click increases your cookie count
- The score is displayed at the top of the screen
- Cool animation circles appear when you click
- Instructions show at the bottom

## The Complete Code

Here's the full Cookie Clicker game code with detailed explanations:

```cpp title="examples/games/Clicker/Clicker.ino"
--8<-- "examples/games/Clicker/Clicker.ino"
```

## Understanding the Code

Let's break down each part of the code to understand how it works:

### 1. Setting Up Variables

```cpp
// Game variables - these store our game information
int cookieCount = 0;             // How many cookies we've clicked
bool showAnimation = false;      // Should we show the click animation?
int animationTime = 0;           // How long has the animation been running?
bool wasButtonPressed = false;   // Was the button pressed last time we checked?
```

**Variables** are like boxes that store information. Think of them as:

- `cookieCount` - A counter that remembers how many cookies you've clicked
- `showAnimation` - A yes/no answer about whether to show the circle animation
- `animationTime` - A timer that counts how long the animation has been running
- `wasButtonPressed` - Remembers if the button was pressed last time we checked

### 2. The Setup Function

```cpp
void setup() {
  engine.start();  // This starts up the Kywy engine
}
```

The `setup()` function runs **once** when your game starts. It's like turning on your game console - you only need to do it once.

### 3. The Game Loop

```cpp
void loop() {
  // This runs over and over again, many times per second
}
```

The `loop()` function runs **forever**, over and over again. This is called a "game loop" and it's how all games work. Each time through the loop, the game:

1. Checks for button presses
2. Updates the animation
3. Clears the screen
4. Draws everything new
5. Repeats!

### 4. Checking for Button Presses

```cpp
if (engine.input.buttonRightPressed && !wasButtonPressed) {
    cookieCount++;        // Add one more cookie
    showAnimation = true; // Start the click animation
    animationTime = 0;    // Reset animation timer
}
```

This code checks if the RIGHT button was **just pressed** (not held down). The `&&` means "AND" - both conditions must be true:

- Button is pressed now (`engine.input.buttonRightPressed`)
- Button was NOT pressed before (`!wasButtonPressed`)

This prevents the game from counting multiple clicks when you hold the button down.

### 5. Remembering Button State

```cpp
wasButtonPressed = engine.input.buttonRightPressed;
```

After we check the button, we remember its current state for next time. This is how we detect when it goes from "not pressed" to "pressed".

### 6. Animation Logic

```cpp
if (showAnimation) {
    animationTime++;
    if (animationTime > 30) {  // Stop animation after 30 loops
      showAnimation = false;
    }
}
```

This controls the circle animation:

- If animation is running, increase the timer
- After 30 loops (about half a second), stop the animation

### 7. Drawing Everything

```cpp
// Clear the screen
engine.display.clear();

// Draw the score
String scoreText = "Cookies: " + String(cookieCount);
engine.display.drawText(cookieX, 20, scoreText.c_str(),
  Display::TextOptions().origin(Display::Origin::Text::CENTER));

// Draw the cookie picture
engine.display.drawBitmap(
    cookieX - COOKIE_WIDTH / 2,   // Left edge of cookie
    cookieY - COOKIE_HEIGHT / 2,  // Top edge of cookie
    COOKIE_WIDTH, COOKIE_HEIGHT,
    cookie_data);

// Draw animation circles
if (showAnimation) {
    int circleSize = 70 + (animationTime * 2);
    engine.display.drawCircle(/*...*/);
}

// Show everything on screen
engine.display.update();
```

Drawing happens in steps:

1. **Clear** - Erase everything from last time
2. **Draw text** - Show the cookie count at the top
3. **Draw bitmap** - Show the cookie picture using the image data from `cookie.hpp`
4. **Draw animation** - If clicking, show expanding circles
5. **Update** - Actually display everything on the screen

#### Understanding Bitmap Drawing

The `drawBitmap()` function takes several parameters:
- **X, Y position**: Where to place the image (we center it by subtracting half the width/height)
- **Width, Height**: Size of the image (defined in `cookie.hpp`)
- **Image data**: The actual pixel information (`cookie_data` array)

The image data comes from the `cookie.hpp` file, which was created using the [Kywy Drawing Editor](https://github.com/KOINSLOT-Inc/kywy-web-tools). Each number in the array represents the color of one pixel!

## Try It Yourself!

### Step 1: Upload the Code

1. Open the Kywy development environment
2. Navigate to `examples/games/Clicker/`
3. Upload the code to your Kywy device
4. Press the RIGHT button to click cookies!

### Step 2: Create Your Own Images

Want to replace the cookie with your own drawing? You can use the **Kywy Drawing Editor** to create custom images!

#### Using the Kywy Drawing Editor

1. **Visit the Editor**: Go to [https://github.com/KOINSLOT-Inc/kywy-web-tools](https://github.com/KOINSLOT-Inc/kywy-web-tools)
2. **Draw Your Image**: Use the web-based drawing tools to create your custom image
3. **Generate Code**: The editor automatically converts your drawing into a `.hpp` file format
4. **Replace the File**: Use the generated code to replace `cookie.hpp` with your custom image

#### Understanding Image Files

The `cookie.hpp` file contains your image data in a special format:

```cpp
// This is how images are stored as code
uint8_t cookie_data[3024] = {
  0xFF, 0xFF, 0xFF, 0xFF, // Each number represents pixel colors
  0xFF, 0xFF, 0xFF, 0xFF, // 0xFF = white, 0x00 = black
  // ... thousands more numbers for all the pixels
};

// These tell us the image size
#define COOKIE_WIDTH 144   // 144 pixels wide
#define COOKIE_HEIGHT 168  // 168 pixels tall
```

**Why This Format?**
- **Memory Efficient**: Images are stored directly in your device's memory
- **Fast Drawing**: No need to load files - the image data is part of your program
- **Custom Graphics**: You can create any image you want for your games!

#### Try These Custom Images

Replace your cookie with:
- **Your Pet**: Draw your dog, cat, or favorite animal
- **Your Face**: Create a simple self-portrait
- **Food Items**: Pizza, hamburger, ice cream cone
- **Game Characters**: Simple sprites for your own games

### Step 3: Experiment and Learn

Try making these changes to see how the code works:

#### Change the Animation Speed
```cpp
// Make animation faster (change 30 to 15)
if (animationTime > 15) {
    showAnimation = false;
}
```

#### Change How Many Cookies Per Click
```cpp
// Get 5 cookies per click instead of 1
cookieCount = cookieCount + 5;  // or cookieCount += 5;
```

#### Add Different Text
```cpp
// Change the score display
String scoreText = "Total Cookies: " + String(cookieCount) + " yum!";
```

#### Make Bigger Circles
```cpp
// Make the animation circles bigger
int circleSize = 100 + (animationTime * 3);  // Start at 100, grow by 3 each time
```

## Programming Concepts You've Learned

### Variables
You used different types of variables:
- `int` for numbers (like `cookieCount`)
- `bool` for true/false values (like `showAnimation`)

### Conditions (If Statements)
You used `if` statements to make decisions:
```cpp
if (something is true) {
    do this;
}
```

### Logical Operators
- `&&` means "AND" (both conditions must be true)
- `!` means "NOT" (opposite of true/false)

### Loops
The `loop()` function creates a game loop that runs forever.

### Functions
You used functions like `engine.display.clear()` to do specific tasks.

## What's Next?

Now that you understand the basics, you could try:

- **Custom Graphics**: Use the [Kywy Drawing Editor](https://github.com/KOINSLOT-Inc/kywy-web-tools) to create your own images
- **Sound Effects**: Adding sound effects when clicking (if your device supports it)
- **Different Cookie Types**: Creating different types of cookies worth different points
- **Upgrades System**: Adding upgrades that increase points per click
- **High Score System**: Saving and displaying the best scores
- **More Animations**: Adding particle effects, color changes, or moving elements
- **Multiple Levels**: Different backgrounds or challenges as you progress

### Learn More About Graphics

Want to understand how computer graphics work? Check out our **[Image Format Tutorial](../graphics_basics/index.md)** _(coming soon!)_ to learn:

- How pixels and colors work in computers
- Why we use hexadecimal numbers for colors
- How to optimize images for small devices
- Advanced drawing techniques and animations

## Common Questions

**Q: Why do we need `wasButtonPressed`?**
A: Without it, holding the button would count as hundreds of clicks per second! We only want to count when the button is first pressed.

**Q: What does `engine.display.update()` do?**
A: It's like pressing "save" - it takes everything you've drawn and actually shows it on the screen.

**Q: Why do we clear the screen each time?**
A: If we didn't clear it, old drawings would stay and create a mess. Clearing gives us a fresh start each frame.

**Q: How fast does the loop run?**
A: Very fast! Many times per second. That's why the animation looks smooth.

**Q: What's inside the `cookie.hpp` file?**
A: It's your cookie image converted into numbers! Each pixel becomes a number that tells the computer what color to display. You can create your own using the [Kywy Drawing Editor](https://github.com/KOINSLOT-Inc/kywy-web-tools).

**Q: Can I use my own images?**
A: Yes! Draw anything you want in the Kywy Drawing Editor, and it will generate the `.hpp` file for you. Replace `cookie.hpp` with your custom image file.

**Q: Why are images stored as code instead of image files?**
A: Storing images as code makes them load faster and saves memory on your device. It's a common technique in embedded programming!

Congratulations! You've built your first game and learned fundamental programming concepts. Keep experimenting and have fun! 🍪🎮
