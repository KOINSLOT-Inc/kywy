<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

<h1 align='center'>
  Kywy
</h1>

<p align='center'><i>
  the tiny engine with big possibilities: education, game dev, diy electronics, and more
</i></p>

<p align='center'>
  <a href="https://linktr.ee/koinslotkywy"><img alt="Linktree" src="https://img.shields.io/badge/linktree-39E09B?style=for-the-badge&logo=linktree&logoColor=white" /></a>
  &nbsp;
  <a href="https://discord.gg/zAYym57Fy6"><img alt="Discord" src="https://img.shields.io/discord/1172988360063725629?style=for-the-badge&logo=discord" /></a>
  &nbsp;
  <a href="https://kywy.io/"><img alt="Website" src="https://img.shields.io/badge/KYWY.io-AAAAAA?style=for-the-badge&logo=data%3Aimage%2Fpng%3Bbase64%2CiVBORw0KGgoAAAANSUhEUgAAACQAAAAkCAYAAADhAJiYAAAACXBIWXMAAAAnAAAAJwEqCZFPAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAKwSURBVHgBtViNdeIwDBZ9N0A2aDa4bHDpBMcIuQ2yAd6guQngJoCbIHQC2gkCE8AGqvTivApXiuMA33t6hliSP0v%2BUbKARCBiQU1J8ouEf2deGBeSo5f%2FJPvFYnGEe4NIZCQ1SYvpaEmqqWMtJpBZUvNKkgddJ5I99FG5%2BGccKY7as6J%2FJHmZHTEflW0w247Ecd8E%2B4Jk420kVpAKMsoDR%2BeUsCv%2BXEBqO2VSFpnpxml%2BD1G%2FPk3SyMEdoSyDdcxg8ygywTiSVG0pVZOZ305IZoLXZ64pDQqdpuDXwAGv8aroFfh9Z%2FH%2FUtEb0IZOZHQqY1Yt6sgD0p2h5xSfjegv1eiAgRFChdDZGjpqWrBP3YBGC101lxC1K7Qxxe95eFALw3wGoRL7VFloYAR4vVxKfrDzfw4Rw9oY8OxFQwcR4HXa6h%2FQX4SMU6CYU8Pbn9sjyR%2FoL1N5ug79GtjmBSKgy%2FZCY508j58gZucCQptgtq0yuwptOJgI%2FFoO%2Bye4nrFEGfwvIA3PMANPI31h%2Fj8gDRUGh%2BEUMCFZXEnwmvkH%2Fdp64wH4IfY7Kvdr7DeMY42plQJ%2BXQe7iN7S2E182mrXhbn2FN%2BD30Yu3nPEqDUGHA7GEm3UI37lGVZzyt59XzYn5%2BBTTdt3T81fQ2eF9qFbit%2F78GByhtHoSS10wgJPoo347bSHnMvMMNwZA%2BWB3ljqikBXpmttOXEGoaUy%2B8bQdQqZTiG%2FsSYWRimHBwNjFWoQpfgbwW1kZCHXmQEIQv2Quhq%2FV5VVzEBWfXd5Jxsh46YYZXhdzNshTSMTnvQuxVh7r1%2FPIebXZhv4cjAHqG%2FftZ9tNmLHqdE%2B33CESrgF3vkGdXR%2B0EEOaJezDd5z5wpiHU4Hk3MpRKIfrAxyJfQVJEsuuoaPV3xh8%2Be8d0jEJ3i2%2BwETXUzkAAAAAElFTkSuQmCC&link=kywy.io" /></a>
  &nbsp;
  <a href="https://koinslot.myshopify.com"><img alt="Store" src="https://img.shields.io/badge/Store-F7931E?style=for-the-badge&logo=shopify&logoColor=232323&link=https%3A%2F%2Fkoinslot.myshopify.com" /></a>
</p>
<br />

*Don't want the pitch? Click [here](./getting_started.md) to skip straight to the docs!*

# What is Kywy?

Kywy is hardware and software, artfully designed to blur the lines between the two.

<p align='center'><img alt="Front and back render of a Kywy device" src="./assets/kywy_front_back_render.png"/></p>

The hardware features:

* a 144x168 LCD screen
* two buttons plus a joystick
* SD card slot
* all day battery life
* I/O: USB-C, GPIO headers, and I2C

The software is designed to give you easy and intuitive access to all of those with only a few lines of code.

## Kywy for Education

Kywy is designed to have as few steps as possible between you and what you want to see on the screen. With simple
interfaces and robust tutorials, Kywy has what you need to get started with embedded programming. You don't need to
worry about display protocols, SPI/I2C, input polling and debouncing, block devices, file systems, or any other
roablocks between you and making the Kywy hardware do what you want.

We're working with [Betabox Learning](https://betaboxlearning.com) to develop curricula for classrooms and for out-of-the-classroom learning so that
you can get started with embedded programming with one simple product.

## Kywy for Game Dev

Kywy has everything you need to create retro-style games that you can keep in your pocket. The engine boasts
multi-threading, event handling, sprites and sprite sheets, state machines, and much more in the pipeline. 

Check out our [examples](https://github.com/KOINSLOT-Inc/kywy/tree/main/examples/games) to see how you can re-create the
classics like Pong, Doodle Jump, Snake, and Flappy Bird!

## Kywy for DIY Electronics

Kywy is the perfect platform for DIY electronics projects. It removes all the barriers between you and what you want to
do.

Say you want to make a data collection device. With Kywy you get everything but the data collection for free. Just hook
up a sensor to the GPIO pins, write a simple program with the engine, and leverage our platform to give you:

* a screen to display the status
* buttons and functionality to create interative menus
* a battery/power management for performance without wires teathering you to the wall
* SD card integrations for data storage

Instead of having to develop an entire project breadboard, figure out displays & button interactions & storage
solutions, and package everything in a robust and portable form, you can just start and end with a Kywy device that can
get you from zero right to solving the problem you want to solve.

Don't get bogged down with 1000 little side issues; start with Kywy and get right to what you really want to do.

## Where to get a Kywy

Visit our [store](https://koinslot.myshopify.com) to get your Kywy today!

## Getting Started

Check out our [documentation](./getting_started.md) to kickstart your Kywy experience!

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://github.com/codespaces/new?repo=KOINSLOT-Inc/kywy)
