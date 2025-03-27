// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_SD_CARD
#define KYWY_LIB_SD_CARD 1

#define KYWY_SD_CARD_CLK 18
#define KYWY_SD_CARD_MISO 16
#define KYWY_SD_CARD_MOSI 19
#define KYWY_SD_CARD_CS 15

namespace Kywy {

class SDCard {
public:
  void setup();
  void test();
};

} // namespace Kywy

#endif