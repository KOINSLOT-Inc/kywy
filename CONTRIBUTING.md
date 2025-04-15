<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Contributing

## Local Development Workflow

We suggested using the Arduino IDE for local development.

To do this, first clone this repo either directly into the `Arduino/libraries` folder on your machine, or elsewhere and
symlink the repo into the `Arduino/libraries` folder.

Next, create a `.development` file and a `src/src.ino` file (these are `.gitignore`'d). Per [this
thread](https://arduino.stackexchange.com/a/43314), the `.development` file will let the Arduino IDE save over library
files and the `src/src.ino` file will let you open then whole library in the IDE by opening that file.

Now you can edit any library file in the IDE and test changes manually via the `src/src.ino` file. You can also use the
IDE for formatting files (`<cmd/ctrl-T>`) and the IDE's built in LSP for easy code completion.

You are also welcome to use whatever text editor you prefer for editing library files. We recommend the IDE here only
because it requires the fewest external dependencies/installs to get editor tooling.
