---
title: "Cookie Clicker"
hide:
  - footer
tags:
  - beginner
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Cookie Clicker

_audience: beginner_

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
- Animated circles appear when you click

## The Complete Code

Here's the full Cookie Clicker game code with detailed explanations:

```cpp title="examples/games/Clicker/Clicker.ino"
--8<-- "examples/games/Clicker/Clicker.ino"
```

## Understanding the Code

Here we will break down each part of the code to understand it:

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

The `setup()` function runs **once** when your game starts. It's like turning on your game console - you only need to do it once. Here we are telling it to start the game engine. The engine contains important code for running the hardware like the display, buttons, and usb port. We will learn more about what the engine does later.

### 3. The Game Loop

```cpp
void loop() {
  // This runs over and over again, many times per second
}
```

The `loop()` function runs **forever**, over and over again. This is called a "game loop" and it's how pretty much all games work. Each time through the loop, the game:

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

The `cookie.hpp` file contains your image data in a special format. Let's break it down step by step:

```cpp
// This is how images are stored as code
uint8_t cookie_data[3024] = {
  0xFF, 0xFF, 0xFF, 0xFF, // Each number represents packed pixel data
  0xFF, 0xFF, 0xFF, 0xFF, // 0xFF = black pixels, 0x00 = white pixels
  // ... thousands more numbers for all the pixels
};

// These tell us the image size
#define COOKIE_WIDTH 144   // 144 pixels wide
#define COOKIE_HEIGHT 168  // 168 pixels tall
```

**What do these numbers mean?**

Think of your image like a grid of tiny colored squares (pixels). Each pixel is stored as a single binary number (1s and 0s), but to make them easier for humans to read and write, these binary values are **packed** into bytes and displayed as hexadecimal numbers.

**Understanding Data Types:**
- `uint8_t` means "unsigned 8-bit integer" - a number that can hold values from 0 to 255
- This is perfect for pixel data because each pixel needs a number between 0 (black) and 255 (white)
- Other data types you might see: `int` (larger numbers), `float` (decimal numbers), `bool` (true/false)

**How Pixel Packing Works:**
Each individual pixel is stored as a single bit (0 = white, 1 = black). Since there are 8 bits in each byte, **each byte stores exactly 8 pixels**. When we store lots of pixels together, they get **packed** into bytes. Think of it like packing clothes into a suitcase - 8 pixels fit into each storage unit (byte).

For example:
- `0xFF` = binary `11111111` = 8 black pixels in a row
- `0x00` = binary `00000000` = 8 white pixels in a row  
- `0x80` = binary `10000000` = 1 black pixel followed by 7 white pixels

**Understanding Hexadecimal Numbers:**
- `0xFF` is a special way to write numbers called "hexadecimal" (or "hex" for short)
- Regular numbers (decimal) use digits 0-9 (like 255)
- Hex numbers use digits 0-9 and letters A-F (like 0xFF)
- `0xFF` in hex equals 255 in decimal - this represents black pixels
- `0x00` in hex equals 0 in decimal - this represents white pixels

**Why 3024 Numbers for 24,192 Pixels?**
The cookie image is 144 pixels wide × 168 pixels tall = 24,192 pixels total. But pixels are **packed efficiently** into bytes. Multiple pixel values can be stored in each byte, so we end up with 3,024 bytes in the array instead of needing 24,192 separate numbers.

**Real Example:**
```cpp
0xFF, 0xFF, 0xFF, 0xFF,  // These 4 bytes contain 32 black pixels (8 pixels per byte)
0x00, 0x00, 0xFF, 0xFF,  // These 4 bytes: 16 white pixels, then 16 black pixels
0x80, 0x80, 0x80, 0x80,  // These 4 bytes contain 32 pixels in a black/white pattern
```

**Why This Format?**
- **Memory Efficient**: Images are stored directly in your device's memory
- **Fast Drawing**: No need to load files - the image data is part of your program
- **Custom Graphics**: You can create any image you want for your games!
- **Learning**: You can see exactly how computers store pictures!

#### Try These Custom Images

Replace your cookie with:
- **Your Pet**: Draw your dog, cat, or favorite animal
- **Your Face**: Create a simple self-portrait
- **Food Items**: Pizza, hamburger, ice cream cone
- **Game Characters**: Simple sprites for your own games

#### Mini-Activity: Understanding Hex Numbers and Data Types

Want to understand those `0xFF` numbers better? Try this quick exercise:

**Convert these hex numbers to regular numbers:**
- `0x00` = ? (Answer: 0 - represents 8 white pixels in a row)
- `0xFF` = ? (Answer: 255 - represents 8 black pixels in a row)
- `0x80` = ? (Answer: 128 - represents 1 black pixel + 7 white pixels)

**Understanding the Pattern:**
Each hex number represents a pattern of 8 pixels (since 8 bits = 1 byte). The format is inverted: 0 = white pixel, 1 = black pixel!

**Understanding Binary to Hex:**
- Binary `11111111` = Hex `0xFF` = 8 black pixels in this byte
- Binary `00000000` = Hex `0x00` = 8 white pixels in this byte
- Binary `10000000` = Hex `0x80` = 1 black pixel + 7 white pixels in this byte

**Hex Counting Practice:**
- Regular: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16...
- Hex: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F, 10...

Remember: A=10, B=11, C=12, D=13, E=14, F=15

**Fun Facts**: 
- Programmers love hex because it perfectly matches how computers store data in bytes!
- Each hex digit represents exactly 4 binary digits (bits)
- `uint8_t` means "unsigned 8-bit integer" - perfect for storing one byte of data

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

## Design Challenges (No Code Provided!)

Now that you understand how the code works, try these creative challenges on your own. **Think through the logic first, then figure out what code to change:**

### Mathematical Challenges

**Cookie Counter Modifications:**
- Can you make the cookie count go up by 2 each time instead of 1?
- What about counting by 5s or 10s?
- How would you make it count backwards from 100?
- Can you make it alternate between adding 1 and adding 3 cookies?

**Animation Speed Experiments:**
- How would you make the animation last twice as long?
- What if you wanted it to run super fast (only 10 loops)?
- Can you make the animation speed up over time?
- How about making it slow down as it progresses?

### Visual Design Challenges

**Shape Modifications:**
- Instead of circles, how could you draw squares around the cookie?
- What about drawing multiple circles of different sizes?
- Can you make the animation shape change color as it grows?
- How would you draw a triangle or diamond shape?

**Animation Patterns:**
- Can you make the circle shrink instead of grow?
- What about making it grow, then shrink back down?
- How would you create a pulsing effect that repeats?
- Can you make multiple circles appear at different times?

**Screen Layout Changes:**
- How would you move the cookie to a different part of the screen?
- What if you wanted to show the score in the bottom corner instead?
- Can you add a second line of text showing something else?
- How would you center everything differently?

### Game Logic Puzzles

**Button Behavior:**
- What if you wanted to use the LEFT button instead of RIGHT?
- How could you make it work with BOTH buttons?
- What about requiring the player to press the button twice quickly?
- Can you make different buttons give different amounts of cookies?

**Scoring Systems:**
- How would you create a "high score" that remembers the best result?
- What about adding a timer that counts down?
- Can you create different "levels" with different point values?
- How would you add a "multiplier" that increases over time?

**Game States:**
- How could you add a "game over" condition?
- What about a "pause" feature when a certain button is pressed?
- Can you create a "reset" button that starts everything over?
- How would you add a "congratulations" message at certain milestones?

### Creative Extensions

**Storytelling Elements:**
- How would you change the text to tell a story as you play?
- What about having different messages based on your score?
- Can you create "achievements" that unlock new text?
- How would you add personality to your cookie character?

**Sound Design (if your device supports it):**
- When would you want to play different sounds?
- How could sound effects enhance the clicking experience?
- What about background music that changes with your score?
- Can you imagine sound patterns that match your animations?

### Reflection Questions

After trying some of these challenges, think about:

- **Problem Solving**: Which challenges were hardest to figure out? Why?
- **Creativity**: What unique ideas did you come up with that weren't listed here?
- **Planning**: How did you break down complex challenges into smaller steps?
- **Testing**: How did you know if your changes worked the way you intended?
- **Debugging**: When something didn't work, how did you figure out what went wrong?

**Remember:** The best way to learn programming is to experiment! Don't be afraid to try something and see what happens. If it breaks, you can always go back to the working version.

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
A: It's your cookie image converted into thousands of numbers! Each tiny square (pixel) in your image becomes a number that tells the computer whether to display black or white. For example, `0xFF` means "black pixels" and `0x00` means "white pixels". Think of it like a paint-by-numbers kit, but instead of colors, we use numbers!

**Q: What does `0xFF` actually mean?**
A: `0xFF` is a hexadecimal number (base-16) that equals 255 in regular numbers (base-10). Here's the simple explanation:
- Regular numbers: 0, 1, 2, 3... 9, 10, 11, 12...
- Hex numbers: 0, 1, 2, 3... 9, A, B, C, D, E, F, 10, 11...
- In hex, F = 15, so FF = 15×16 + 15 = 255
- `0xFF` = black pixels (all pixels in this byte are black)
- `0x00` = white pixels (all pixels in this byte are white)
- `0x80` = pattern of black and white pixels (creates visual texture)

**Q: Can I use my own images?**
A: Yes! Draw anything you want in the Kywy Drawing Editor, and it will automatically convert your drawing into these special numbers. Replace `cookie.hpp` with your custom image file, and your game will show your artwork instead!

**Q: Why are images stored as code instead of image files?**
A: Storing images as numbers in code makes them load super fast and saves memory on your device. It's like having the image "baked into" your program instead of having to read it from a separate file. This is a common technique in embedded programming (programming small devices like your Kywy)!

Congratulations! You've built your first game and learned fundamental programming concepts. Keep experimenting and have fun! 🍪🎮
