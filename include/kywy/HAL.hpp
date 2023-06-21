// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#if !defined(HAL_DISPLAY_PROTOCOL_I2C) && !defined(HAL_DISPLAY_PROTOCOL_SPI)
#define HAL_DISPLAY_PROTOCOL_SPI
#endif

#ifndef HAL_DISPLAY_PROTOCOL_I2C
#define HAL_DISPLAY_PIN_I2C_SDA -1
#define HAL_DISPLAY_PIN_I2C_SCL -1
#define HAL_DISPLAY_I2C_ADDRESS 0x0
#endif

#ifndef HAL_DISPLAY_PROTOCOL_SPI
#define HAL_DIPSLAY_PIN_SPI_CS -1
#define HAL_DIPSLAY_PIN_SPI_RST -1
#define HAL_DIPSLAY_PIN_SPI_DC -1
#define HAL_DIPSLAY_PIN_SPI_SCLK -1
#define HAL_DIPSLAY_PIN_SPI_MOSI -1
#else
#define HAL_DISPLAY_PIN_SPI_CS 9
#define HAL_DISPLAY_PIN_SPI_RST 7
#define HAL_DISPLAY_PIN_SPI_DC 6
#define HAL_DISPLAY_PIN_SPI_SCLK 5
#define HAL_DISPLAY_PIN_SPI_MOSI 4
#endif

#ifndef HAL_BUTTON_LEFT_PIN
#define HAL_BUTTON_LEFT_PIN 15
#endif

#ifndef HAL_BUTTON_RIGHT_PIN
#define HAL_BUTTON_RIGHT_PIN 3
#endif

#ifndef HAL_D_PAD_LEFT_PIN
#define HAL_D_PAD_LEFT_PIN 18
#endif

#ifndef HAL_D_PAD_RIGHT_PIN
#define HAL_D_PAD_RIGHT_PIN 2
#endif

#ifndef HAL_D_PAD_UP_PIN
#define HAL_D_PAD_UP_PIN 1
#endif

#ifndef HAL_D_PAD_DOWN_PIN
#define HAL_D_PAD_DOWN_PIN 0
#endif

#ifndef HAL_LED_PIN
#define HAL_LED_PIN 8
#endif
