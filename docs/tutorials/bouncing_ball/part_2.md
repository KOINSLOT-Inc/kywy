---
title: "Part 2: Moving the Ball"
---
<!-- hide next page button that links outside of tutorial -->
<style>
  .md-footer__link--next:not([hidden]) { display: none }
</style>

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

  // lastly, we'll add a 250 ms delay so that things don't happen too fast
  delay(250);
}
```

What do you think you'll see if you upload this? Was it this?

{% set frames = [] %}
{% for i in range(1, 20) %}
  {% set operations = [] %}
  {% for j in range(i) %}
    {% set _ = operations.append(("draw_circle", [ [12, 12 + j], 12 ], { "outline": "black" })) %}
  {% endfor %}
  {% set _ = frames.append(operations) %}
{% endfor %}
{{ kywy_screen_gif("first_circle_moves_no_clear", frames, duration=250) }}

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

  engine.display.drawCircle(0, y, 25);  // draw our circle into the now blank memory
  engine.display.update();

  delay(250);
}
```

Now we should see our ball moving successfully!

{% set frames = [] %}
{% for i in range(1, 20) %}
  {% set _ = frames.append([("draw_circle", [ [12, 12 + i], 12 ], { "outline": "black" })]) %}
{% endfor %}
{{ kywy_screen_gif("first_circle_moves", frames, duration=250) }}
