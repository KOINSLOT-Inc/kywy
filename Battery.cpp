// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <Arduino.h>

#include "Battery.hpp"

namespace Kywy {

void Battery::setup() {
  pinMode(KYWY_BATTERY_CHARGING, INPUT_PULLUP);
  pinMode(KYWY_BATTERY_CHARGED, INPUT_PULLUP);
  pinMode(KYWY_BATTERY_VOLTAGE, INPUT);
}

bool Battery::isCharging() { return !digitalRead(KYWY_BATTERY_CHARGING); }
bool Battery::isCharged() { return !digitalRead(KYWY_BATTERY_CHARGED); }

int Battery::percentCharged() {
  if (isCharged()) { return 100; }

  int rawVoltagePinReading = analogRead(KYWY_BATTERY_VOLTAGE);

  float voltage, minVoltage, maxVoltage;
  if (isCharging()) {
    // values derived from linear regression of the following collected data with R^2 = 0.989
    // mins	voltage	reading
    // 0	  3.8	    591
    // 10	  4.03	  626
    // 20	  4.14	  639
    // 30	  4.16	  644
    // 40	  4.2	    645
    // 45	  4.16	  639
    voltage = (0.00722 * rawVoltagePinReading) - 0.474;

    minVoltage = 3.8;
    maxVoltage = 4.16; // shift down a bit so we don't flip flop between 100 and less than 100
  } else {
    // values derived from linear regression of the following collected data with R^2 = 0.999
    // mins	voltage	reading
    // 0	  3.54	  539
    // 10	  3.8	    585
    // 20	  3.99	  614
    // 30	  4.1	    632
    // 40	  4.16	  641
    // 45	  4.14	  638
    voltage = (0.00611 * rawVoltagePinReading) + 0.24;

    minVoltage = 3.54;
    maxVoltage = 4.14; // shift down a bit so we don't fip flob between 100 and less than 100
  }

  int percentCharged = (int)(100.0 * ((voltage - minVoltage) / (maxVoltage - minVoltage)));
  percentCharged = percentCharged > 100 ? 100 : percentCharged;
  percentCharged = percentCharged < 0 ? 0 : percentCharged;

  // round to nearest 10 to avoid fast changing numbers and greater percieved accuracy that we actually have
  int remainder = percentCharged % 10;
  int roundingCorrection = (remainder >= 5) ? (10 - remainder) : (-1 * remainder);

  // Serial.println("voltage: " + String(voltage) + ", " + "minVoltage: " + String(minVoltage) + ", " + "maxVoltage: " + String(maxVoltage) + ", " + "percentCharged: " + String(percentCharged) + ", " + "remainder: " + String(remainder) + ", " + "roundingCorrection: " + String(roundingCorrection));

  percentCharged += roundingCorrection;

  return percentCharged;
}

} // namespace Kywy