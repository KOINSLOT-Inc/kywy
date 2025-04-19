<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Getting Started

Starting with Kywy is as easy as 1, 2, 3!

## 1. Install the Arduino IDE

Visit the [Arduino website](https://www.arduino.cc/en/software/) to install the Arduino IDE.

## 2. Install Kywy Board and Library

Open the Arduino IDE.

In the left-hand bar, click the Board Manager. Type "pico" in the search bar and install the "Arduino Mbed OS RP2040
Boards".

Select the board you just installed by clicking `Tools` -> `Board` -> `Arduino Mbed OS RP2040 Boards` -> `Raspberry Pi
Pico`.

In the left-hand bar, click the Library Manager. Type "kywy" in the search bar and install the "Kywy" library.

## 3. Upload Your First Program

Plug in your Kywy.

Update your empty sketch to look like:

```c++
#include "Kywy.hpp"

Kywy::Engine engine;

void setup() {
  // put your setup code here, to run once:
  engine.start();

  engine.display.drawText(5, 5, "Hello, world!");
  engine.display.update();
}

void loop() {
  // put your main code here, to run repeatedly:
}
```

Click the "Upload" button in the top left corner. After the code compiles and uploads you should see "Hello,
world!" displayed on your Kywy!
