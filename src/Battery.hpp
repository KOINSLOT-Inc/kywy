// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef KYWY_LIB_BATTERY
#define KYWY_LIB_BATTERY 1

#define KYWY_BATTERY_CHARGING 10
#define KYWY_BATTERY_CHARGED 11
#define KYWY_BATTERY_VOLTAGE 26

namespace Kywy {

class Battery {
public:
  void setup();
  bool isCharging();
  bool isCharged();
  int percentCharged();
};

}  // namespace Kywy

#endif