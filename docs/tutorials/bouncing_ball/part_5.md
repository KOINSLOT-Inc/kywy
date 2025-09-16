---
title: "Part 5: Handling Events"
---

<!-- hide next page button that links outside of tutorial -->
<style>
  .md-footer__link--next:not([hidden]) { display: none }
</style>

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Part 5: Handling Events

## Actors

Now we're going to get into a really powerful part of the Kywy engine, and how it's meant to be used: with Actors and
Events.

If you want to build a game or program that has some meat to it things can get really complicated really fast,
especially if you're trying to run everything from a single `loop` function.

The Kywy engine offers a solution to this in the form of Actors and Events.

Actors are buckets of code that run on their own, recieve Events, respond (or handle) those Events, and even send out
(or publish) Events of their own.

The real kicker is that you can have multiple actors, all running simultaneously. It's like having multiple `loop`
functions that can each have a bit of the responsibility.

Events are the glue between these separately running bits of code. In other words: Actors talk to each other through
Events.

## The Simplest Actor

Let's look at the most basic Actor we can create.

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

// an actor is a class that inherits from Actor::Actor
class NothingDoer : public Actor::Actor {
public:
  // an actor must have a `handle` function to recieve messages
  void handle(::Actor::Message *message) {}
} nothingDoer; // create an instance of a NothingDoer called nothingDoer

void setup() {
  engine.start();
  nothingDoer.start(); // an actor is started just like the engine
}

void loop() {
  delay(1000); // don't churn the CPU running an empty loop
}
```

This actor does, you may be surprised to learn, nothing. But it does have all the critical parts of an Actor:

* A class definition that inherits from `Actor::Actor`
* a `handle` function that takes in an `::Actor::Message *`
* a call to `start()`

## Responding to an Event

Let's make our `NothingDoer` actually do something (I guess we'll have to rename it as well).

We'll start with responding to our first event: `Kywy::Events::TICK`. The engine will automatically create these `TICK`
events at a regular interval, and our actors can respond in kind.

To handle this event, we'll need to update our `handle` function. For now, we'll simply make it print out "Tock"
whenever it get's a `TICK`.

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

class Tocker : public Actor::Actor {
public:
  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::TICK:
        Serial.println("Tock");
        break;
    }
  }
} tocker;

void setup() {
  engine.start();
  tocker.start();
}

void loop() {
  delay(1000);
}
```

A few notes:

* We see what type of message we're dealing with by checking `message->signal`
* We use a `switch` statement here because we very often (and will below) expand the `handle` function to respond to
  multiple types of events.

If we run this and check our serial monitor we should see...nothing.

That's because we've missed one crucial part: subscription.

By default, actors don't actually listen for any events. They just plug their ears and sit there. We have to tell an
actor who it should listen to by calling the `subscribe` method on another actor.

In other words, one actor "subscribes" to another actors events.

Here's where we reveal that the Kywy engine is actually an actor--well, several actors--itself!

One of those actors you've already seen: `engine.input`. We'll use that actor later, but you can probably guess that it
publishes events related to button presses.

The actor we need here is `engine.clock`, which periodically fires the `TICK` event and drives forward our program.

So lets add this subscription:

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

class Tocker : public Actor::Actor {
public:
  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::TICK:
        Serial.println("Tock");
        break;
    }
  }
} tocker;

void setup() {
  engine.start();

  // this is all we need to add, it tells our tocker
  // to listed for events from the engine clock
  tocker.subscribe(&engine.clock); 
  tocker.start();
}

void loop() {
  delay(1000);
}
```

Now our serial monitor should be endlessly printing

```
Tock
Tock
Tock
Tock
Tock
Tock
Tock
...
```

!!! info
    
    You can also unsubscribe from other actors. For example, if we wanted our tocker to stop tocking at some point we
    could call `tocker.unsubscribe(&engine.clock)` and it would no longer recieve those events.

## Bouncing Ball Actor

Now let's apply this to our bouncing ball. Instead of running our physics simulation in `loop`, we're going to move it
to a new actor that will run the simulation every `TICK`.

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

class Ball : public Actor::Actor {
public:
  // make our global variables class variables instead
  float x = KYWY_DISPLAY_WIDTH / 2;
  float y = KYWY_DISPLAY_HEIGHT / 2;
  float xVelocity = 5;
  float yVelocity = 0;

  void handle(::Actor::Message *message) {
    switch (message->signal) {
      // copy in our physics code, EXCEPT THE `delay`
      // that's handled by the engine.clock now.
      case Kywy::Events::TICK:
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

        // draw to the screen
        engine.display.clear();
        engine.display.drawCircle(
          (int)x, (int)y,
          25,
          Display::Object2DOptions()
            .origin(Display::Origin::Object2D::CENTER));
        engine.display.update();
        break;
    }
  }
} ball;

void setup() {
  engine.start();

  // subscribe our ball to the clock and start it!
  ball.subscribe(&engine.clock); 
  ball.start();
}

// remove all code from `loop` and just put in
// a delay so that it doesn't run often
void loop() { 
  delay(1000);
}
```

We should now see... exactly what we did before!

<p align="center">
    <img src="../part_4/img/ball_with_physics.gif"/>
</p>

## Handling Inputs

So why did we go to all this trouble?

Consider a new behavior we might want for our program: giving the ball a "kick" whenever we press up on the joystick.

How would you accomplish this with our previous method? Well we'd have to:
* keep track of whether the button was pressed or not
* whenever it changes, check if it's changing to pressed or to not-pressed
* only apply a "kick" when it's changing to pressed

Not too bad, but still a bit clunky.

With actors we can just listen for more events. Luckily for us, the Kywy engine has just the one we need:
`Kywy::Events::D_PAD_UP_PRESSED`. This event comes from the `engine.input` actor so we'll also have to remember to
subscribe our `ball` to that.

So lets add a `-15` (upwards) kick to our `yVelocity` whenever we get a `D_PAD_UP_PRESSED` event.

We should only need 4 lines of code for this. Our new event handler:

```c++
      case Kywy::Events::D_PAD_UP_PRESSED:
        yVelocity -= 15;
        break;
```

and our new subscription:

```c++
  ball.subscribe(&engine.input);
```

In total we have:

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

class Ball : public Actor::Actor {
public:
  float x = KYWY_DISPLAY_WIDTH / 2;
  float y = KYWY_DISPLAY_HEIGHT / 2;
  float xVelocity = 5;
  float yVelocity = 0;

  void handle(::Actor::Message *message) {
    switch (message->signal) {
      case Kywy::Events::TICK:
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

        // draw to the screen
        engine.display.clear();
        engine.display.drawCircle(
          (int)x, (int)y, 25,
          Display::Object2DOptions()
            .origin(Display::Origin::Object2D::CENTER));
        engine.display.update();
        break;

      // our new event handler
      case Kywy::Events::D_PAD_UP_PRESSED:
        yVelocity -= 15;
        break;
    }
  }
} ball;

void setup() {
  engine.start();

  // don't forget to subscribe to input events
  ball.subscribe(&engine.input);
  ball.subscribe(&engine.clock); 
  ball.start();
}

void loop() { 
  delay(1000);
}
```

And there we have it! A Kywy style ball bouncing around the screen and responding to button presses!

{% set coordinates = [
  (97, 111), (102, 118), (107, 126),  (112, 135),
  (117, 145), (122, 156), (127, 156), (132, 145),
  (132, 135), (127, 126), (122, 118), (117, 111),
  (112, 105), (107, 100), (102, 96), (97, 93), (92, 91),
  (87, 90), (82, 90), (77, 91), (72, 93), (67, 96),
  (62, 100), (57, 105), (52, 111), (47, 118), (42, 126),
  (37, 135), (32, 145), (27, 156), (22, 156), (17, 145),
  (12, 135), (12, 126), (17, 118), (22, 111), (27, 105),
  (32, 100), (37, 96), (42, 93), (47, 91), (52, 90),
  (57, 90), (62, 91), (67, 93), (72, 96),
  (77, 100, "d_pad_up"), (82, 90, "d_pad_up"),
  (87, 81, "d_pad_up"), (92, 73, "d_pad_up"),
  (97, 66, "d_pad_up"), (102, 60, "d_pad_up"),
  (107, 55, "d_pad_up"), (112, 51, "d_pad_up"),
  (117, 48, "d_pad_up"), (122, 46, "d_pad_up"),
  (127, 45, "d_pad_up"), (132, 45), (132, 46), (127, 48),
  (122, 51), (117, 55), (112, 60), (107, 66), (102, 73),
  (97, 81), (92, 90), (87, 100), (82, 111), (77, 123),
  (72, 136), (67, 150), (62, 156), (57, 142), (52, 129),
  (47, 117), (42, 106), (37, 96), (32, 87), (27, 79), (22, 72),
  (17, 66), (12, 61), (12, 57), (17, 54), (22, 52), (27, 51),
  (32, 51), (37, 52), (42, 54), (47, 57), (52, 61), (57, 66),
  (62, 72), (67, 79), (72, 87), (77, 96), (82, 106), (87, 117),
] %}
{% set frames = [] %}
{% for coordinate in coordinates %}
  {% set operations = [
    ("circle", [ [coordinate[0], coordinate[1] ], 12 ], { "outline": "black" }),
  ] %}
  {% if coordinate|length == 3 %}
    {% set _ = operations.append(("d_pad_up", [], {})) %}
  {% endif %}
  {% set _ = frames.append(operations) %}
{% endfor %}
{{ kywy_screen_gif("ball_with_kick", frames, duration=25) }}

!!! info

    The full list of events that you can recieve from the Kywy engine is
    [here](../../api_reference.md).

    You can also create your own events to publish and handle! For more advanced Actor techniques check out our [Actors
    Guide](../../guides/actors.md).
