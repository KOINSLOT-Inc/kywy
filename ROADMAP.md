<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Engine Roadmap

_last updated: april 30th, 2025_

## Overview

The Kywy engine is currently in a beta state: fully functional and usable, and ready for a period of maturation,
refinement based on user feedback, and new feature implementation.

Since the engine is out in the wild for use, we'll call it at least `v1`.

This roadmap will sketch out our path forward along three major work streams: documentation, codebase maturity, and new
features development.

Note: T-Shirt sizes (S, M, L, etc.) are given for each item.

## Work Stream: Documentation

The documentation work stream is first, purposefully, because it's our highest priority work in this roadmap. We need a
good base of content that we can hand to users so that they can get started, learn, and debug without needing to rely
solely on help from the Discord server.

*Primary Goal: A Kywy user should be able to go from zero knowledge to a functional game using only our documentation.*

Items:

* [x] (S) `README.md` for the repo that gives an overview, links to Kywy website/socials/store, and links to further
  documentation.
* [x] (M) Spike: Documentation Hosting. Where should we host docs? GitHub Wiki, hosted static site, markdown in repo?
* [x] (M) Automated API Documentation. Use a tool like `doxygen` to generate API docs for us, or see if there's a tool
  typically used by Arduino libraries.
    * [ ] build out docstrings and api documentation
* [x] (M) Getting Started Guide: Should walk through from scratch to uploading a `Hello, World!` program to the Kywy.
* [ ] (M) Engine Overview Guide: Should walk through all the features of the engine at a high level, and also talk about the
  engine design philosophy and an opinionated way to make games with it.
* [ ] (M) Actor Guide
* [ ] (M) State Machine Guide
* [ ] (M) FAQ Page
* [ ] Tutorials:
    * [x] (S) Writing to the screen
    * [x] (S) Reading button input
    * [x] (S) Using the clock
    * [x] (S) Bouncing Ball - walk through how to use the previous three to create a bouncing ball that you can move left and
      right (without actors)
    * [x] (S) Bouncing Ball w/ Actors - convert the previous tutorial code to one with actors, add a second ball using a
      second actor that can move independently (e.g. 1 uses d-pad, one uses L/R buttons).
    * [ ] (M) Some tutorial about adding a state machine to this, need a clean way to transition from the previous tutorial to
      this one
* [ ] (L) Documentation System: A way to automatically lint and/or update docs so that as we update the engine we can
  propagate changes to a growing codebase
* [ ] (L) Example walk-through for each example, better comments for examples so they're readable
* [ ] (L) Building a game from start to finish
* [ ] (M) Community showcase: with permission granted, incorporate exemplary community projects into our codebase with an
  overview and annotations written by us. These should be hosted/owned by us so that they cannot be changed underneath
  us though.


## Work Stream: Codebase Maturity

The codebase maturity work stream is second, purposefully, because we need to make sure we refine the engine to a point
that it is enjoyable to learn and use.

*Primary Goal: Our codebase should be intuitive, easy to use, and reliable.*

Items:

* [x] (M) Formatting/Linting: Arduino has a [lint action](https://github.com/arduino/arduino-lint-action) we can use in
  pipelines and an [LSP] (https://github.com/arduino/arduino-language-server) we can use locally.
    * [ ] also linting for any other file types we have
* [ ] (L) ~Build~ & Test Locally and in Pipelines
    * [ ] (L) runners with Kywy hardware
* [x] (M) Local developer workflow: how to easily update the engine and examples without lots of copying back and forth
* [x] (M) Get library added to Arduino IDE for trivial installs
* [ ] (L) UAT & Address Feedback
* [ ] (S) Sync practices in examples to make sure we have a consistent style
    * [ ] (M) Design and Practices Guide
* [ ] (S) Contributing guide/working agreement for codebase contributors
* [ ] (L) Debug Tooling
    * [ ] Debug lines for bitmaps/sprites/text/etc.
    * [ ] Debug logging system for actors/state machines
    * [ ] Click-to-tick or maybe press-to-tick feature to step through frames, would be best if we could use the serial
      interface to let the user send a serial signal to advance frames so we don't have to hijack the buttons to do it
    * [ ] Actor flow/Performance analyzer mode: review how events are flowing through actors and how much time it's taking to
      handle each event

## Work Stream: New Feature Development

The new feature development work stream is third, purposefully, because the new features listed below are
"nice-to-haves", especially when compared to the items in the work streams above.

*Primary Goal: Allow easier and more advanced game development with new engine features.*

Items:

* [ ] (M) Advanced sprite sheets (named frames, animation cycles, etc)
* [ ] (L) UI Layout Tool: easily create menus/buttons, screen overlays, etc.
* [ ] (L) Physics: colliders and forces.
    * [ ] (L) Particle system?
* [ ] (L) Scene Management: Manage changes between major game states (e.g. start screen -> in game -> game over screen -> in
  game), need a global actor management system for this, but brings us much closer to a clean multi-launcher. Also
  allows for much more complex games to be made (e.g. a basic RPG).
    * [ ] (L) Update & Render Loop, don't force the user to do it
* [ ] (M) Bitmap rotation (fun pixel interpolation with linear algebra)
* [ ] (M) SD Card: Finish the SD Card implementation
    * [ ] (M) Saved state system
    * [ ] (M) Asset loading system
* [ ] (S) `DrawBatteryIndicator` for easy display of battery level
