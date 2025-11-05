// SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

unsigned long tickCount = 0;
bool fast = true;

void setup() {
  Serial.begin(115200);
  // start with a fast tick (33ms)
  engine.clock.setTickDuration(33);
  engine.start();
  Serial.println("ClockTest started");
}

void loop() {
  // nothing to do here; hooks drive the test
  delay(1000);
}

// Called every tick
void onTick(Actor::Message *message) {
  tickCount++;
  if ((tickCount % 50) == 0) {
    // toggle tick speed every 50 ticks
    if (fast) {
      engine.clock.setTickDuration(200); // slow
      Serial.println("Switched to slow ticks (200 ms)");
    } else {
      engine.clock.setTickDuration(33); // fast
      Serial.println("Switched to fast ticks (33 ms)");
    }
    fast = !fast;
  }

  // print a short status every 10 ticks
  if ((tickCount % 10) == 0) {
    Serial.print("tick #");
    Serial.print(tickCount);
    Serial.print(" current tick duration: ");
    Serial.print(engine.clock.getTickDuration());
    Serial.println(" ms");
  }
}
