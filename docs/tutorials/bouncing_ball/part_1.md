---
title: "Part 1: Writing To The Screen"
---

<!-- hide previous page button that links outside of tutorial -->
<style>
  .md-footer__link--prev:not([hidden]) { display: none }
</style>

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 1: Writing To The Screen

Welcome to the bouncing ball tutorial! We assume no knowledge of the Kywy engine or game development. This is a
"start-from-scratch" tutorial and a great place to start if you're a beginner both to Kywy and game development. If
you have some programming experience you may find this a bit slow.

We do assume a basic knowledge of programming in C++ with Arduino, and won't be explaining coding or Arduino basics.

As you're working through the tutorail, please stop on by the Discord to ask questions, share your progress, or show off
any customizations you make!

<a href="https://discord.gg/zAYym57Fy6"><img alt="Discord" src="https://img.shields.io/discord/1172988360063725629?style=for-the-badge&logo=discord" /></a>

## Starting Off

We'll begin with the simplest possible Kywy program, one that does nothing!

```c++
#include "Kywy.hpp"   // includes the Kywy engine

Kywy::Engine engine;  // creates an instance of the engine

void setup() {
  // put your setup code here, to run once:
  engine.start();     // starts the engine!

}

void loop() {
  // put your main code here, to run repeatedly:

}
```

This will be how all your Kywy projects start: importing and defining the engine and starting it up.

## Interacting With the Screen

Screen interactions are all done with `engine.display`. Drawing, clearing, updating, everything.

With the Kywy engine, "drawing to the screen" does not actually immediately display anything. Instead, the drawing is
done in memory. Later you must call `engine.display.update()` to actually send the pixels you've drawn to the screen for
display. This helps keep the engine performant by minimizing slow interactions like talking with the screen and
maximizing fast interactions like modifying values in memory.

Let's see this in action!

## Drawing Your First Circle

This tutorial is about a bouncing ball, so let's draw one. To do this we'll use the `engine.display.drawCircle` method:

::: doxy.kywy.class.method
    name: Display::Display
    method: void drawCircle(int16_t x, int16_t y, uint16_t diameter, Object2DOptions options=Object2DOptions())

We'll ignore the `Object2DOptions` for now. These help you customize exactly how the circle is drawn and we'll get into
more detail about them later.

All you need to to worry about is `x`, `y`, and `diameter`. These define where the circle is draw, and how big it will
be. Let's try it out with `0`, `0`, and `25` respectively.

We're going to put our code in the `setup` function so that it only runs once.

```c++
#include "Kywy.hpp"   // includes the Kywy engine

Kywy::Engine engine;  // creates an instance of the engine

void setup() {
  // put your setup code here, to run once:
  engine.start();     // starts the engine!

  engine.display.drawCircle(0, 0, 25);
}

void loop() {
  // put your main code here, to run repeatedly:

}
```

That's it! Now try uploading it to your Kywy.

What do you get? Nothing!

{{ kywy_screen(
  "blank_screen",
  [],
) }}

## Updating the Screen

That's because the draw methods only write to memory and not directly to the screen. We still need to call
`engine.display.update()` to actually show what we drew.

```c++
#include "Kywy.hpp"   // includes the Kywy engine

Kywy::Engine engine;  // creates an instance of the engine

void setup() {
  // put your setup code here, to run once:
  engine.start();     // starts the engine!

  engine.display.drawCircle(0, 0, 25);  // writes to memory
  engine.display.update();              // sends memory to the screen
}

void loop() {
  // put your main code here, to run repeatedly:

}
```

Uploading this should give you:

{{ kywy_screen(
  "first_circle",
  [
    ("circle", [ [12, 12], 12 ], { "outline": "black" }),
  ],
) }}

Your first circle!

Note how the X/Y coordinate `(0, 0)` corresponds to the top left of the screen. That is the origin for all draw calls.
Down on the screen increases Y and right on the screen increases X.
