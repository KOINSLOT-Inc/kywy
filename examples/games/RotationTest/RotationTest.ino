// SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Kywy.hpp"

Kywy::Engine engine;

const uint8_t spriteSheetData[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00,
  0x00, 0x03, 0xc0, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x18, 0x18, 0x00,
  0x00, 0x70, 0x0e, 0x00, 0x01, 0xc0, 0x03, 0x80, 0x07, 0x00, 0x00, 0xe0, 0x0c, 0x02, 0x40, 0x30,
  0x18, 0x0e, 0x70, 0x18, 0x10, 0x38, 0x1c, 0x08, 0x10, 0x00, 0x00, 0x08, 0x30, 0x00, 0x00, 0x0c,
  0x20, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x04,
  0x20, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x04, 0x30, 0x00, 0x00, 0x0c, 0x1f, 0xff, 0xff, 0xf8
};

uint8_t rotatedBitmap[(48 * 48)/8];
const uint8_t *rotatedFrames[] = {rotatedBitmap};


// static uint8_t rotatedBitmap[sizeof(spriteSheetData)];
const uint8_t *frames[] = { spriteSheetData };
Sprite slime(frames, 1, 32, 32);


class slimeRotator : public Actor::Actor {
public:
  int angle = 0;

  void initialize() {
    slime.setPosition(58, 64);
    slime.setVisible(true);
    slime.setNegative(true);
  }

  void printBitmap(const uint8_t *bitmap, int width, int height) {
    int byteCount = (width * height) / 8;
    Serial.println("const uint8_t bitmap[] = {");
    for (int i = 0; i < byteCount; ++i) {
        if (i % 8 == 0 && i != 0) {
            Serial.println();  // Add new line after every 8 bytes for readability
        }
        Serial.print("0x");
        Serial.print(bitmap[i], HEX);
        Serial.print(", ");
    }
    Serial.println("\n};");
}

  void handle(::Actor::Message *message) {
    static int tickCount = 0;
    const int n = 5;
    int width = slime.width;
    int height = slime.height;

    switch (message->signal) {
      case Kywy::Events::TICK:
        if (tickCount % n == 0) {
          engine.display.clear();
          slime.debugPrintBitmapInfo();

          const uint8_t *currentFrame = slime.frames[slime.frame];
          printBitmap(currentFrame, width, height);
          
          // slime.rotate(spriteSheetData, rotatedBitmap, 48, 48, float(angle));
          printBitmap(rotatedBitmap, width, height);

          static const uint8_t* frames2[1];
          frames2[0] = currentFrame;

          slime.setSheet(frames2,1);

          angle += 45;
          if (angle >= 360)
            angle = 0;
          slime.render();
          engine.display.update();
        }


        tickCount++;
    }
  }
} slimeRotator;

void setup() {
  engine.start();
  slime.setDisplay(&engine.display);

  slimeRotator.subscribe(&engine.input);
  slimeRotator.subscribe(&engine.clock);
  slimeRotator.start();
}

void loop() {
  delay(1000);
}
