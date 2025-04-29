---
title: "Part 4: Simulating Physics"
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 4: Simulating Physics

We're actually going to take a quick detour here, remove our button input code, and add in some more realistic physics
to our ball.

Don't worry though; we'll add back in controls in the next part, but in a totally different way!

## Goal and Strategy

We have a ball, but right now it's really just a circle moving with the joystick. What we want is a ball that falls with
gravity and bounces off the walls, floor, and ceiling.

So far we've just been controlling the ball's position, but to get our physics going we're instead going to control it's
velocity instead, and write some simple rules that determine where the ball goes based on that.

Our code needs to do four things:

1. keep track of x and y velocity, and position
1. constantly increase the y velocity to simulate gravity
1. recognize a collision when a ball hits a wall, the ceiling, or the floor
1. when a collision is detected, flip either the x or y velocity to simulate a bounce

## Position and Velocity

We'll start with keeping track of our x and y velocities and positions:

```c++
// start x and y at the center of the screen
float x = KYWY_DISPLAY_WIDTH / 2;
float y = KYWY_DISPLAY_HEIGHT / 2;

// start with a sideways velocity so interesting things happen
float xVelocity = 5;
float yVelocity = 0;
```

Now, why `floats` instead of the `ints` we had been using?

When simulating physics we often get fractions in our calculations. If we stick with `ints`, which are always rounded to
the nearest whole number, we can get unexpected results. E.g. a ball with a very small velocity might never move
because, after moving 0.25 pixels, the final pixel location is rounded right back to where it started:

```
(int)new_x = int(old_x + 0.25) == old_x
```

Calculating everything as a `float` and only converting to `ints` at the very end when we write to the screen will give
us an accurate simulation of physics.

We'll also need to change the position each frame according to the velocity. In our loop we can put:

```c++
x = x + xVelocity;
y = y + yVelocity;
```

## Gravity

Gravity is actually surprisingly easy, it's just a steady increase of the downwards velocity. We can achieve this with a
single line in our `loop`:

```c++
yVelocity += 1; // simulate gravity
```

## Collisions

The most difficult part of this simulation is knowing when the ball has collided with one of the sides of the screen.
For this we'll need a little math.

The first thing we need to remember is that the ball has a diameter of 25 pixels, and we're drawing the ball from it's center point.

From this, we can say that we'll know the ball is touching a wall if it's center is `25 / 2 ~ 12` pixels from a side. We
have 4 walls, so we'll need to check the distance to each of them:

```c++
if (x < 12) {
  // we've collided with the left wall
}

if (x > KYWY_DISPLAY_WIDTH - 12) {
  // we've collided with the right wall
}

if (y < 12) {
  // we've collided with the top wall
}

if (y > KYWY_DISPLAY_HEIGHT - 12) {
  // we've collided with the bottom wall
}
```


## Bounces

Now that we know when the ball has hit a wall, we just need to reverse either it's x or y velocity when that happens to
simulate a bounce. The side walls will change the x velocity, and the top and bottom walls the y velocity.

Note we use `abs` to get the absolute value (always positive) here.


```c++
if (x < 12) {
  // make sure the velocity is positive, to the right
  xVelocity = abs(xVelocity);
}

if (x > KYWY_DISPLAY_WIDTH - 12) {
  // make sure the velocity is negative, to the left
  xVelocity = -1 * abs(xVelocity);
}

if (y < 12) {
  // make sure the velocity is positive, downwards
  yVelocity = abs(yVelocity);
}

if (y > KYWY_DISPLAY_HEIGHT - 12) {
  // make sure the velocity is negative, upwards
  yVelocity = -1 * abs(yVelocity);
}
```

The last thing we'll do is make sure the ball doesn't move through our walls by, when a collision is detected, making
sure the ball's position is always inside the boundary of the wall.


```c++
if (x < 12) {
  xVelocity = abs(xVelocity);
  // x should never be less than 12
  x = 12;
}

if (x > KYWY_DISPLAY_WIDTH - 12) {
  xVelocity = -1 * abs(xVelocity);

  // x should never be more than 12 from the right edge
  x = KYWY_DISPLAY_WIDTH - 12;
}

if (y < 12) {
  yVelocity = abs(yVelocity);
  // y should never be less than 12
  y = 12;
}

if (y > KYWY_DISPLAY_HEIGHT - 12) {
  yVelocity = -1 * abs(yVelocity);
  // y should never be more than 12 from the top edge
  y = KYWY_DISPLAY_HEIGHT - 12;
}
```

## Putting It All Together

Above is everything we need to make our ball bounce around the screen according to physics! We have an x and y velocity
and position, gravity, collision detection, and bounces on collisions.

Putting this all together in one script looks like:

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

// x and y position and velocity
float x = KYWY_DISPLAY_WIDTH / 2;
float y = KYWY_DISPLAY_HEIGHT / 2;
float xVelocity = 5;
float yVelocity = 0;

void setup() {
  engine.start();
}

void loop() {
  // x and y position and velocity changes
  x = x + xVelocity;
  y = y + yVelocity;

  // collisions and bounces
  if (x < 12) {
    xVelocity = abs(xVelocity);
    x = 12;
  }

  if (x > KYWY_DISPLAY_WIDTH - 12) {
    xVelocity = -1 * abs(xVelocity);
    x = KYWY_DISPLAY_WIDTH - 12;
  }

  if (y < 12) {
    yVelocity = abs(yVelocity);
    y = 12;
  }

  if (y > KYWY_DISPLAY_HEIGHT - 12) {
    yVelocity = -1 * abs(yVelocity);
    y = KYWY_DISPLAY_HEIGHT - 12;
  }

  // gravity
  yVelocity += 1;

  // and finally, drawing to the screen
  engine.display.clear();
  engine.display.drawCircle(
    (int)x, (int)y,  // note we cast the floats to ints
    25,
    Display::Object2DOptions().origin(Display::Origin::Object2D::CENTER));
  engine.display.update();

  delay(25);
}
```

With that, we should now be able to see the ball bouncing around the screen!

{% set coordinates = [
  (12, 90), (17, 90), (22, 91), (27, 93), (32, 96),
  (37, 100), (42, 105), (47, 111), (52, 118), (57, 126),
  (62, 135), (67, 145), (72, 156), (77, 145), (82, 135),
  (87, 126), (92, 118), (97, 111), (102, 105), (107, 100),
  (112, 96), (117, 93), (122, 91), (127, 90), (132, 90),
] %}
{% set frames = [] %}
{% for coordinate in coordinates %}
  {% set _ = frames.append([
    ("circle", [ [coordinate[0], coordinate[1] ], 12 ], { "outline": "black" }),
  ]) %}
{% endfor %}
{% for coordinate in coordinates[1:-1]|reverse %}
  {% set _ = frames.append([
    ("circle", [ [coordinate[0], coordinate[1] ], 12 ], { "outline": "black" }),
  ]) %}
{% endfor %}
{{ kywy_screen_gif("ball_with_physics", frames, duration=25) }}

!!! info

    Want to see some weird physics? If you change the order of some of these operations you may see the ball clipping
    through the edge of the screen or getting faster and faster on every bounce. Play around with the ordering and see
    if you can figure out why!
