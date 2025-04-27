---
title: "Part 3: Using Button Inputs"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 3: Using Button Inputs

We've got a ball, it's on the screen, it's moving. Now let's make it dance to your input instead! In this part we'll
walk through the simplest way of using button inputs (meaning that later we'll discuss the more complex, but more
powerful way!).

## Engine Inputs

Just like the screen has `engine.display`, buttons have `engine.input`. Everything to do with button presses and
releases can be found in this section of the engine.

If you just need to check if a button is pressed or not, all you need to do read a value from `engine.input`. Want to
know if the left button is pressed? Look at `engine.input.buttonLeftPressed`. It's that easy!

Let's write just a few lines to work this into our previous program so that the ball can move up instead of down when we
press the left button:

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

int y = 0;

void setup() {
  engine.start();
}

void loop() {
  // check if the left button is pressed
  if (engine.input.buttonLeftPressed) {
    y -= 1;  // if it is, move the ball up instead of down
  } else {
    y += 1;  // otherwise do what we were doing before
  }

  engine.display.clear();

  engine.display.drawCircle(
    KYWY_DISPLAY_WIDTH / 2, KYWY_DISPLAY_HEIGHT / 2 + y, 25,
    Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
  engine.display.update();

  delay(25);  // speed things up so that they're more exciting
}
```

That's it! If we upload this we should get:

{% set frames = [] %}
{% for i in range(0, 40) %}
  {% set _ = frames.append([("circle", [ [144 / 2, 168 / 2 + i], 12 ], { "outline": "black" })]) %}
{% endfor %}
{% for i in range(0, 40) %}
  {% set _ = frames.append([
    ("circle", [ [144 / 2, 168 / 2 + 39 - i], 12 ], { "outline": "black" }),
    ("press_left_button", [], {}),
  ]) %}
{% endfor %}
{{ kywy_screen_gif("move_up_on_button_press", frames, duration=25) }}

## D-Pad Control

Lets modify our code a bit to give full control of the ball using the Kywy d-pad (it used to be a d-pad, so the code
still refers to it as such even if it's really a joystick).

We want the ball to move in whatever direction the joystick is pointed, including diagonally, and can do this by
checking in the state of the joystick directions and applying a movement accordingly.

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

int y = 0;
int x = 0; // add an x to control too

void setup() {
  engine.start();
}

void loop() {
  // check each direction and apply the movement if it's pressed
  if (engine.input.dPadLeftPressed) {
    x -= 1;
  } 
  if (engine.input.dPadRightPressed) {
    x += 1;
  } 
  if (engine.input.dPadUpPressed) {
    y -= 1;
  } 
  if (engine.input.dPadDownPressed) {
    y += 1;
  } 

  engine.display.clear();

  engine.display.drawCircle(
    // add x here too!
    KYWY_DISPLAY_WIDTH / 2 + x, KYWY_DISPLAY_HEIGHT / 2 + y, 25,

    Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
  engine.display.update();

  delay(25);
}
```

With these few lines you should now be able to move the ball wherever you want using just the joystick!

{% set frames = [] %}
{% for i in range(0, 40) %}
  {% set _ = frames.append([
    ("circle", [ [144 / 2, 168 / 2], 12 ], { "outline": "black" }),
  ]) %}
{% endfor %}
{% for i in range(0, 40) %}
  {% set _ = frames.append([
    ("circle", [ [144 / 2, 168 / 2 + i], 12 ], { "outline": "black" }),
    ("d_pad_down", [], {}),
  ]) %}
{% endfor %}
{% for i in range(0, 40) %}
  {% set _ = frames.append([
    ("circle", [ [144 / 2 - i, 168 / 2 + 39 - i], 12 ], { "outline": "black" }),
    ("d_pad_up_left", [], {}),
  ]) %}
{% endfor %}
{% for i in range(0, 40) %}
  {% set _ = frames.append([
    ("circle", [ [144 / 2 - 39 + i, 168 / 2], 12 ], { "outline": "black" }),
    ("d_pad_right", [], {}),
  ]) %}
{% endfor %}
{{ kywy_screen_gif("ball_with_dpad_controls", frames, duration=25) }}
