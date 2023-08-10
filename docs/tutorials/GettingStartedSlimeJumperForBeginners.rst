.. SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.

   SPDX-License-Identifier: GPL-3.0-or-later

Beginner Tutorial: Building a Slime Jumper Game
===============================================

Introduction
------------

Welcome to a Kywy getting started tutorial! This particular guide is crafted with beginners in mind, focusing on those
new to programming, game development, and the Kywy engine. Whether you're taking your first steps into coding or
curious about exploring the world of game creation, this tutorial offers a friendly introduction.

Kywy is an Espressif component designed to provide an educational experience in game development. It offers a hands-on
approach to learning programming concepts and game mechanics, making it a valuable tool for those looking to explore and
understand the fundamentals.

What You Will Learn
~~~~~~~~~~~~~~~~~~~

This tutorial will guide you through the foundational aspects of using the Kywy engine to create a simple game. Here's
what you can expect to explore:

1. **Setting Up the Project**: Learn how to include the Kywy engine in your project and initialize it to begin crafting
   your game.

2. **Creating Sprites**: Delve into defining and managing graphical objects known as sprites, essential in many games.

3. **Handling User Input**: Discover how to capture and respond to user interactions within your game.

4. **Managing Game Actors**: Explore basic concepts of creating and controlling game components like characters and
   platforms.

5. **Running the Game**: Assemble the pieces to create a basic game loop, bringing your creation to life.

Who This Tutorial Is For
~~~~~~~~~~~~~~~~~~~~~~~~

- **Absolute Beginners**: If you're new to programming and game development, this tutorial offers a gentle introduction.

- **Aspiring Game Enthusiasts**: If you're interested in games and want to try creating your own, this tutorial provides
  a starting point.

- **Users New to Kywy**: If you have some coding experience but are new to Kywy, this tutorial helps you explore its
  features.

While this tutorial introduces some fundamental concepts of game development and the Kywy engine, it's important to note
that it serves as a starting point. It's one of several tutorials that you can explore to gradually build your skills and
understanding.

Ready to begin? Let's dive in!

Setting Up the Project
----------------------

Creating a game with the Kywy engine involves setting up the project environment and including the necessary components.
Here's a step-by-step guide to help you get started:

1. **Include Kywy Header**

   The first step in using the Kywy engine is to include the Kywy header file in your project. This header file
   contains all the necessary declarations and functions to work with the engine.

   .. code-block:: cpp

      #include <Kywy.hpp>

   Explanation:

   - The ``#include`` directive tells the compiler to include the contents of the specified file, in this case,
     "Kywy.hpp."
   - By including this header file, you gain access to all the classes, functions, and variables defined in the Kywy
     engine, allowing you to utilize its features in your game.

2. **Initialize the Engine**

   Once you've included the Kywy header, you'll need to create an instance of the Kywy engine. This instance will act
   as the central hub for managing various aspects of your game, such as rendering, input handling, and more.

   .. code-block:: cpp

      Kywy::Engine engine;

   Explanation:

   - ``Kywy::Engine`` is a class provided by the Kywy library that encapsulates the core functionality of the engine.
   - By creating an instance of this class (named ``engine`` in this example), you initialize the engine and prepare it
     for use in your game.
   - This instance will be used throughout your code to interact with various engine features, such as drawing graphics,
     handling user input, and managing game actors.

Creating Sprites
----------------

In the world of game development, a sprite is a two-dimensional image or animation that represents an object within the
game. Sprites are used to depict characters, enemies, obstacles, and more. They are the visual building blocks that give
life to the game's environment.

A sprite sheet is a collection of multiple sprites organized into a single image. It's a way to group related sprites
together, such as different frames of an animation. By using a sprite sheet, you can efficiently manage and animate
multiple sprites, creating more dynamic and engaging visuals.

Define Sprite Data
~~~~~~~~~~~~~~~~~~

Kywy uses binary data to represent sprite sheets. This data is an array of bytes that encodes the pixel information for
each sprite in the sheet.

For this Slime Jumper game we are going to use the following sprite sheet. It has three frames that represent the slime
in different stages of a jump cycle.

----

.. image:: ../images/slime_sprite_sheet.png
   :alt: Slime Sprite Sheet
   :align: center

----

.. note::
   This tutorial won't cover how to create the binary data for the sprites. There will be another tutorial dedicated to
   creating sprite data.

We can represent this sprite sheet using binary data that the Kywy engine can understand:

.. code-block:: cpp

   // binary data for slime frames, 3x1 sprite sheet encoded with 1 bit per pixel (monochrome)
   const static uint8_t spriteSheetData[] = {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
       0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00,
       0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00,
       0x00, 0x01, 0x80, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x0c, 0x30,
       0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x02,
       0x40, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00,
       0x0c, 0x30, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0x00, 0x00, 0xe0, 0x00, 0x0b, 0xd0, 0x00, 0x00, 0x05, 0xa0, 0x00,
       0x0c, 0x02, 0x40, 0x30, 0x00, 0x1a, 0x58, 0x00, 0x00, 0x0d, 0xb0, 0x00, 0x18, 0x0e, 0x70, 0x18, 0x00, 0x36, 0x6c,
       0x00, 0x00, 0x08, 0x10, 0x00, 0x10, 0x38, 0x1c, 0x08, 0x00, 0xe4, 0x27, 0x00, 0x00, 0x18, 0x18, 0x00, 0x10, 0x00,
       0x00, 0x08, 0x01, 0x80, 0x01, 0x80, 0x00, 0x30, 0x0c, 0x00, 0x30, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x00, 0x80, 0x00,
       0x20, 0x04, 0x00, 0x20, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0xc0, 0x00, 0x60, 0x06, 0x00, 0x20, 0x00, 0x00, 0x04,
       0x06, 0x00, 0x00, 0x60, 0x00, 0xc0, 0x03, 0x00, 0x20, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x20, 0x00, 0x80, 0x01,
       0x00, 0x20, 0x00, 0x00, 0x04, 0x0c, 0x00, 0x00, 0x30, 0x00, 0x80, 0x01, 0x00, 0x20, 0x00, 0x00, 0x04, 0x08, 0x00,
       0x00, 0x10, 0x01, 0x80, 0x01, 0x80, 0x20, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x80, 0x30,
       0x00, 0x00, 0x0c, 0x08, 0x00, 0x00, 0x10, 0x00, 0x80, 0x01, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xe0,
       0x00, 0x7f, 0xfe, 0x00};

Create a Sprite Sheet
~~~~~~~~~~~~~~~~~~~~~

In Kywy, the ``SpriteSheet`` class is used to create a sprite sheet from the binary data. You'll need to specify the
width and height of each frame, and the number of frames. Kywy currently supports only equal-sized frames in a sprite
sheet, reading them left to right and top to bottom.

You can create a ``SpriteSheet`` with the constructor:

.. cpp:alias:: SpriteSheet::SpriteSheet

The Slime Jumper sprite sheet is 96 pixels wide by 32 pixels tall and contains 3 frames so we will write:

.. code-block:: cpp

   SpriteSheet mySpriteSheet = ::SpriteSheet(spriteSheetData, 96, 32, 3);

Next we'll tell the sprite sheet about how the frames are laid out with the ``addFrames`` method:

.. cpp:alias:: SpriteSheet::addFrames

This method starts at the ``x``/``y`` position given and defines ``numFrames`` with height ``frameHeight`` and width
``frameWidth``. Since we are calling this method it should live in the main loop function. We have 3 frames that start
at position ``(0, 0)`` and are 32 pixels squares.

.. code-block:: cpp

  // load sprite frames
  slimeSheet.addFrames(0, 0, 32, 32, 3);


Create a Sprite
~~~~~~~~~~~~~~~

The ``Sprite`` class in Kywy allows you to define and manage individual sprites. You can create a sprite by specifying
the frames, width, and height.

.. cpp:alias:: Sprite::Sprite

We have all of this data from previous steps so we can define the sprite as:

.. code-block:: cpp

   Sprite slime(slimeSheet.frames, 3, 32, 32);

.. tip::
   The ``Sprite`` and ``SpriteSheet`` classes in Kywy provide various methods to manipulate and animate sprites,
   offering flexibility in designing your game's visuals. For example, you can use ``mySprite.setFrame(frameIndex)`` to
   set a specific frame, or ``mySpriteSheet.getFrame(frameIndex)`` to retrieve a frame from the sprite sheet.

The code up till now should look like:

.. code-block:: cpp

   #include <Kywy.hpp>

   Kywy::Engine engine;

   // binary data for slime frames, 3x1 sprite sheet encoded with 1 bit per pixel (monochrome)
   const static uint8_t spriteSheetData[] = {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
       0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00,
       0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00,
       0x00, 0x01, 0x80, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x0c, 0x30,
       0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x02,
       0x40, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00,
       0x0c, 0x30, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0x00, 0x00, 0xe0, 0x00, 0x0b, 0xd0, 0x00, 0x00, 0x05, 0xa0, 0x00,
       0x0c, 0x02, 0x40, 0x30, 0x00, 0x1a, 0x58, 0x00, 0x00, 0x0d, 0xb0, 0x00, 0x18, 0x0e, 0x70, 0x18, 0x00, 0x36, 0x6c,
       0x00, 0x00, 0x08, 0x10, 0x00, 0x10, 0x38, 0x1c, 0x08, 0x00, 0xe4, 0x27, 0x00, 0x00, 0x18, 0x18, 0x00, 0x10, 0x00,
       0x00, 0x08, 0x01, 0x80, 0x01, 0x80, 0x00, 0x30, 0x0c, 0x00, 0x30, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x00, 0x80, 0x00,
       0x20, 0x04, 0x00, 0x20, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0xc0, 0x00, 0x60, 0x06, 0x00, 0x20, 0x00, 0x00, 0x04,
       0x06, 0x00, 0x00, 0x60, 0x00, 0xc0, 0x03, 0x00, 0x20, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x20, 0x00, 0x80, 0x01,
       0x00, 0x20, 0x00, 0x00, 0x04, 0x0c, 0x00, 0x00, 0x30, 0x00, 0x80, 0x01, 0x00, 0x20, 0x00, 0x00, 0x04, 0x08, 0x00,
       0x00, 0x10, 0x01, 0x80, 0x01, 0x80, 0x20, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x80, 0x30,
       0x00, 0x00, 0x0c, 0x08, 0x00, 0x00, 0x10, 0x00, 0x80, 0x01, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xe0,
       0x00, 0x7f, 0xfe, 0x00};


   // define slime sprite
   SpriteSheet slimeSheet = ::SpriteSheet(spriteSheetData, 96, 32, 3);

   extern "C" void app_main(void) {
     // load sprite frames
     slimeSheet.addFrames(0, 0, 32, 32, 3);
     Sprite slime(slimeSheet.frames, 3, 32, 32);
   }

This code block represents all the code covered in this tutorial up to this point. It's a valid C++ code snippet that
you can copy and paste into a file to run.

----

This section has introduced you to the concept of sprites and sprite sheets, essential elements in game development. You
have also learned how to use Kywy's ``Sprite`` and ``SpriteSheet`` classes to define and manage these graphical objects.
These foundational skills will enable you to create visually engaging games as you continue to explore Kywy and game
development.

Setting Up the Game Manager
---------------------------

In the context of game development, an actor represents an individual entity or object within the game. It could be a
character, an enemy, a platform, or even a system component like the game manager. Actors interact with each other and
respond to events, forming the dynamic behavior of the game.

Kywy's Actor Implementation
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Kywy's implementation of actors provides a flexible and robust framework for managing game entities. Here's a brief
overview of the key components:

- **Actor Class**: Defines the basic structure and behavior of an actor. It includes methods for initialization,
  handling messages, starting and stopping the actor, and managing subscriptions to other actors.

- **Message Handling**: Actors communicate through messages. The ``handle`` method processes incoming messages, allowing
  actors to respond to specific directives.

- **Subscriptions**: Actors can subscribe to each other, enabling them to receive and respond to messages from other
  actors in the system.

- **Starting and Stopping**: The ``start`` and ``stop`` methods manage the actor's lifecycle, allowing it to begin
  processing messages and to clean up when no longer needed.

The Need for a Game Manager Actor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As a game grows in complexity, managing its lifecycle, transitions between screens, and overall flow can become
challenging. Abstracting these responsibilities into a dedicated actor can simplify the process, leading to the creation
of a game manager actor. This actor serves as a central hub, coordinating various aspects of the game and ensuring a
smooth player experience.

Define the Game Manager
~~~~~~~~~~~~~~~~~~~~~~~

We'll begin by defining the game manager actor, inheriting from Kywy's Actor class. This will allow us to customize its
behavior to suit our game's needs.

.. code-block:: cpp

   class GameManager : public Actor::Actor {
     const char *getName() { return "gameManager"; };
      // ...
   } gameManager;

Show the Start Screen
~~~~~~~~~~~~~~~~~~~~~

The start screen is the first thing the player sees when the game is launched. It's a welcoming screen that displays the
game title and provides instructions on how to begin the game. In our case, we'll show the game title "SLIME JUMPER" and
instruct the player to press the left button to start.

Here's how we can implement the start screen in the game manager:

1. **Clear the Display**: We'll start by clearing the display to ensure a clean slate for drawing the start screen.

2. **Draw the Game Title**: We'll draw the game title "SLIME JUMPER" at the center of the screen using the
   ``drawText`` method.

3. **Draw the Instructions**: Below the title, we'll draw the instructions to press the left button to start. We'll use
   the ``drawText`` method again, specifying the position and font.

4. **Update the Display**: Finally, we'll call the ``update`` method to refresh the display and show the drawn elements.

Here's the code snippet for a `showStartScreen` method that accomplishes the points above:

.. code-block:: cpp

   void showStartScreen() {
     engine.display.clear();

     // draw title card
     char slimeJumper[] = "SLIME JUMPER";
     char pressLeftButton[] = "Press left button";
     char toStart[] = "to start.";
     engine.display.drawText(Display::Origin::Text::BASELINE_CENTER, 64, 54, Display::Font::intel_one_mono_12_pt,
                             slimeJumper, 0xf);
     engine.display.drawText(Display::Origin::Text::BASELINE_CENTER, 64, 82, Display::Font::intel_one_mono_8_pt,
                             pressLeftButton, 0xf);
     engine.display.drawText(Display::Origin::Text::BASELINE_CENTER, 64, 94, Display::Font::intel_one_mono_8_pt,
                             toStart, 0xf);
     engine.display.update();

     subscribe(&engine.input); // listen for user input
   }

This code snippet draws the game title "SLIME JUMPER" and the instructions to press the left button to start the game.

The ``drawText`` method:

.. cpp:alias:: Display::Display::drawText

is used three times above so lets discuss each of the inputs since it'll be useful for any other games you want to make.

The first argument, ``origin``, determines what part of the text is used as a reference point for placement. The
possible origins are defined in the enum class ``Display::Origin::Text``:

.. cpp:alias:: Display::Origin::Text

In the code above we use ``BASELINE_CENTER``. The text baseline is the horizontal line upon which most letters and
characters in a typeface sit. Letters like ``g`` and ``q`` will have their tails below the baseline. You can think of
the baseline like the lines on ruled paper you might have used in school. So when we use the origin ``BASELINE_CENTER``
we are telling the Kywy engine that we want to position the text relative to the center of this baseline. The next two
arguments, ``x`` and ``y``, specify where the center of the baseline should be.

After that we specify the font to use, which is chosen from the ``Display::Font`` namespace:

.. cpp:alias:: Display::Font

Kywy comes preloaded with multiple fonts that you can use.

The next argument, ``text``, is a pointer to the text that we want to render on the screen. After that we specify the
4-bit (0-16) greyscale color value that the text should be rendered with.

We won't deal with the ``flags`` argument here.

Putting these all together gives us a flexible way to write text anywhere on the screen.

Wire Up to User Inputs
~~~~~~~~~~~~~~~~~~~~~~

The game manager will also need to respond to user input. In Kywy, inputs are managed by another actor. This actor
creates events like button presses and sends them to subscribed actors. The game manager can subscribe to the engine's
input events to listen for specific user actions, such as pressing the left button to start the game. By wiring the game
manager to user inputs, we can create an interactive start screen that responds to player actions.

Defining the Game Events
************************

We'll start by defining an enum to represent the different game events, including the start screen:

.. code-block:: cpp

   enum Events : uint16_t {
     START_SCREEN = Kywy::Events::USER_EVENTS,
   };

Note that the `= Kywy::Events::USER_EVENTS` causes the event numbering to start after the numbering of built in Kywy
events. This allows us to mix Kywy events with custom events and not have to handle them separately. E.g. if we had a
Kywy event with enum number `1` and a custom event with the same number then we would have to have some way to
distinguish between them. Instead, we use `= Kywy::Events::USER_EVENTS` to start the custom event numbering after the
end of the Kywy event numbering to ensure there's no overlap.

Implementing the Handle Method
******************************

The `handle` method is where we'll process incoming messages and events.

1. **Handle the START_SCREEN State**: We'll add a case for the `START_SCREEN` state in the `handle` method. Inside this
   case, we'll call the `showStartScreen` method to display the start screen.

2. **Respond to User Input**: We'll listen for specific user input events, such as pressing the left button, and respond
   accordingly. For example, we can transition to the gameplay state when the left button is pressed.

Here's the code snippet for the `handle` method:

.. code-block:: cpp

   void handle(::Actor::Message *message) {
     switch (message->signal) {
       case START_SCREEN: {
         showStartScreen();
         subscribe(&engine.input); // listen for user input
         break;
       }
       case Kywy::Events::D_PAD_LEFT_PRESSED: { // user event to start a new game
         unsubscribe(&engine.input);            // no longer need to listen for user input

         // start the game!

         break;
       }
     }
   }

Note that we add the ``subscribe(&engine.input)`` line so that the game manager will receive user input events. We don't
always want the game manager to recieve these events though. E.g. later when the game is running we don't want to start
a new game every time the left button is pressed so we ``unsubscribe(&engine.input)`` after handling the
``Kywy::Events::D_PAD_LEFT_PRESSED`` event.

Kywy engine events are found in the ``Kywy::Events`` namespace:

.. cpp:alias:: Kywy::Events

Lastly, lets start the game manager actor along with the Kywy engine in the main loop:

.. code-block:: cpp

   engine.start();
   gameManager.start();

Here is the code we've written up to this point:

.. code-block:: cpp

   #include <Kywy.hpp>

   Kywy::Engine engine;

   // binary data for slime frames, 3x1 sprite sheet encoded with 1 bit per pixel (monochrome)
   const static uint8_t spriteSheetData[] = {
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80,
       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
       0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00,
       0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x03, 0xc0, 0x00,
       0x00, 0x01, 0x80, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x0c, 0x30,
       0x00, 0x00, 0x02, 0x40, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x02,
       0x40, 0x00, 0x00, 0x70, 0x0e, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x00,
       0x0c, 0x30, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0x00, 0x00, 0xe0, 0x00, 0x0b, 0xd0, 0x00, 0x00, 0x05, 0xa0, 0x00,
       0x0c, 0x02, 0x40, 0x30, 0x00, 0x1a, 0x58, 0x00, 0x00, 0x0d, 0xb0, 0x00, 0x18, 0x0e, 0x70, 0x18, 0x00, 0x36, 0x6c,
       0x00, 0x00, 0x08, 0x10, 0x00, 0x10, 0x38, 0x1c, 0x08, 0x00, 0xe4, 0x27, 0x00, 0x00, 0x18, 0x18, 0x00, 0x10, 0x00,
       0x00, 0x08, 0x01, 0x80, 0x01, 0x80, 0x00, 0x30, 0x0c, 0x00, 0x30, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x00, 0x80, 0x00,
       0x20, 0x04, 0x00, 0x20, 0x00, 0x00, 0x04, 0x03, 0x00, 0x00, 0xc0, 0x00, 0x60, 0x06, 0x00, 0x20, 0x00, 0x00, 0x04,
       0x06, 0x00, 0x00, 0x60, 0x00, 0xc0, 0x03, 0x00, 0x20, 0x00, 0x00, 0x04, 0x04, 0x00, 0x00, 0x20, 0x00, 0x80, 0x01,
       0x00, 0x20, 0x00, 0x00, 0x04, 0x0c, 0x00, 0x00, 0x30, 0x00, 0x80, 0x01, 0x00, 0x20, 0x00, 0x00, 0x04, 0x08, 0x00,
       0x00, 0x10, 0x01, 0x80, 0x01, 0x80, 0x20, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x80, 0x30,
       0x00, 0x00, 0x0c, 0x08, 0x00, 0x00, 0x10, 0x00, 0x80, 0x01, 0x00, 0x1f, 0xff, 0xff, 0xf8, 0x07, 0xff, 0xff, 0xe0,
       0x00, 0x7f, 0xfe, 0x00};


   // define slime sprite
   SpriteSheet slimeSheet = ::SpriteSheet(spriteSheetData, 96, 32, 3);

   enum Events : uint16_t {
     START_SCREEN = Kywy::Events::USER_EVENTS,
   };

   class GameManager : public Actor::Actor {
     const char *getName() { return "gameManager"; };

     void showStartScreen() {
       engine.display.clear();

       // draw title card
       char slimeJumper[] = "SLIME JUMPER";
       char pressLeftButton[] = "Press left button";
       char toStart[] = "to start.";
       engine.display.drawText(Display::Origin::Text::BASELINE_CENTER, 64, 54, Display::Font::intel_one_mono_12_pt,
                               slimeJumper, 0xf);
       engine.display.drawText(Display::Origin::Text::BASELINE_CENTER, 64, 82, Display::Font::intel_one_mono_8_pt,
                               pressLeftButton, 0xf);
       engine.display.drawText(Display::Origin::Text::BASELINE_CENTER, 64, 94, Display::Font::intel_one_mono_8_pt,
                               toStart, 0xf);
       engine.display.update();

       subscribe(&engine.input); // listen for user input
     }

     void handle(::Actor::Message *message) {
       switch (message->signal) {
         case START_SCREEN: {
           showStartScreen();
           subscribe(&engine.input); // listen for user input
           break;
         }
         case Kywy::Events::D_PAD_LEFT_PRESSED: { // user event to start a new game
           unsubscribe(&engine.input);            // no longer need to listen for user input

           // start the game!

           break;
         }
       }
     }
   } gameManager;

   extern "C" void app_main(void) {
     // load sprite frames
     slimeSheet.addFrames(0, 0, 32, 32, 3);
     Sprite slime(slimeSheet.frames, 3, 32, 32);

     engine.start();
     gameManager.start();
   }

This code block represents all the code covered in this tutorial up to this point. It's a valid C++ code snippet that
you can copy and paste into a file to run.

----

This section has introduced you to the concept of actors and how they are implemented in Kywy. You've learned how to
define a custom actor for managing the game and how to display the start screen. As you continue to explore Kywy and
build your game, you'll see how actors provide a powerful and flexible framework for creating engaging and interactive
experiences.
