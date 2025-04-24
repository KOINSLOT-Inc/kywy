---
title: "Part 2: Moving the Ball"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 2: Moving the Ball

A circle on a screen is boring, so lets make it fall down.

## Parameterizing Y

Remembering that increasing Y represents moving downwards on the screen, let's replace the hardcoded `0` that we used
for `y` before with a variable that we can modify.

There are a few changes we need to make to our script:

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

// this is our new variable, note that we define it outside of any
// function so that it will be available globally
int y = 0;

void setup() {
  engine.start();
}

void loop() {
  // we've moved our draw and update to the `loop` function so that it
  // will run over and over again

  y += 1; // each time it runs we'll increase y

  // and then draw the circle at the new location
  engine.display.drawCircle(0, y, 25); // note our variable `y` used here
  engine.display.update();

  // lastly, we'll add a 100 ms delay so that things don't happen too fast
  delay(100);
}
```

What do you think you'll see if you upload this? Was it this?

{% set frames = [] %}
{% for i in range(1, 20) %}
  {% set operations = [] %}
  {% for j in range(i) %}
    {% set _ = operations.append(("circle", [ [12, 12 + j], 12 ], { "outline": "black" })) %}
  {% endfor %}
  {% set _ = frames.append(operations) %}
{% endfor %}
{{ kywy_screen_gif("first_circle_moves_no_clear", frames, duration=100) }}

## Clearing the Screen

All the circles from previous loops stay on the screen! But why is that?

It's because the memory where we store what's on the screen still has all the previous draws in it. This makes it easy
to contruct images from mulitple draw calls, which is critical for building complex scenes later on.

To clear out this memory we have to make an explicit `engine.display.clear()` call which will reset the memory to a
blank white screen.

Note that `engine.display.clear()` works like a draw call where it doesn't immediately update the screen, just the
memory where the screen image is stored. We still have to `engine.display.update()` to apply it. However, you can
`engine.display.clear()` and then make additional draw calls on the blanked out screen memory first.

We want to put the `engine.display.clear()` call before we do our drawing so that we can draw to a nice blank canvas.

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

int y = 0;

void setup() {
  engine.start();
}

void loop() {
  y += 1;

  // reset the screen buffer memory to all white,
  // erasing previous circles
  engine.display.clear();               

  // draw our circle into the now blank memory
  engine.display.drawCircle(0, y, 25);

  engine.display.update();

  delay(100);
}
```

Now we should see our ball moving successfully!

{% set frames = [] %}
{% for i in range(1, 20) %}
  {% set _ = frames.append([("circle", [ [12, 12 + i], 12 ], { "outline": "black" })]) %}
{% endfor %}
{{ kywy_screen_gif("first_circle_moves", frames, duration=100) }}

## Centering the Ball

Remember that `Object2DOptions` argument that we ignored before? We're going to look at that now!

You'll find that these `Options` objects are common in `engine.display` methods and what you learn here will apply to
other types of `Options` too. These options let you specify, you guessed it, optional behaviors. Sometimes there's one
option, sometimes four, it just depends on what you're trying to draw to the screen.

In this case we're using `Object2DOptions` since we're drawing a circle, a 2D object, and there are two options to
modify: `color` and `origin`.

You specify these options with chained function calls on a new `Object2DOptions` object, e.g.:

```c++
Object2DOptions().color(BLACK).origin(Display::Origin::Object2D::TOP_LEFT);
Object2DOptions().color(WHITE);
Object2DOptions().origin(Display::Origin::Object2D::TOP_LEFT);
Object2DOptions().origin(Display::Origin::Object2D::TOP_LEFT).color(BLACK);
```

!!! info

    We use this scheme because it let's you specify whatever options you want in whatever order you want them. This is
    actually surprisingly difficult to do in older versions of C++!

Color is pretty self-explanatory. Since Kywy has a black and white screen you can specify `BLACK` or `WHITE` for the
color. `BLACK` is the default, which is why our circle appears black on a white screen above. Setting the color to
`WHITE` would just result in a blank white screen.

Origin describes _where_ the circle is drawn from. There are a couple of options for a 2D object:

{{ include_code("src/Display.hpp", "enum class Object2D") }}

These are very useful for getting a shape exactly where you want it without as much math! The default origin is
`TOP_LEFT`.

If we were to draw a circle with

```c++
engine.display.drawCircle(
  0, 0, 25,
  Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER),
);
```

then the center of the circle will be placed at `(0, 0)` instead and we'll see:

{{ kywy_screen(
  "circle_with_center_origin",
  [
    ("circle", [ [0, 0], 12 ], { "outline": "black" }),
  ],
) }}

With all that in mind, let's center the ball!

We want the center of the ball to be at the center of the screen. We know we can use `Display::Origin::Object2D::CENTER`
to get the center of the ball. What about the screen? For that we can use the `KYWY_DISPLAY_WIDTH` and
`KYWY_DISPLAY_HEIGHT` macros and just divide them in half to get the middle.

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

int y = 0;

void setup() {
  engine.start();
}

void loop() {
  y += 1;

  engine.display.clear();               

  engine.display.drawCircle(
    // specify the center of the screen
    KYWY_DISPLAY_WIDTH / 2, KYWY_DISPLAY_HEIGHT / 2 + y,

    25,

    // specify the center of the ball
    Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER)
  );
  engine.display.update();

  delay(100);
}
```

{% set frames = [] %}
{% for i in range(1, 20) %}
  {% set _ = frames.append([("circle", [ [144 / 2, 168 / 2 + i], 12 ], { "outline": "black" })]) %}
{% endfor %}
{{ kywy_screen_gif("centered_ball_moving", frames, duration=100) }}

And there we have it: a centered, moving ball!
