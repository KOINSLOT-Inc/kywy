// SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SDCard.hpp"

#include "mbed.h"
#include "SDBlockDevice.h"
#include "BlockDevice.h"
#include "FATFileSystem.h"
#include "LittleFileSystem.h"


namespace Kywy {

void SDCard::setup() {
  bool setupSucceeded;
  // SPI.begin();
  // setupSucceeded = SD.begin(KYWY_SD_CARD_CS);
  // Serial.println("setup succeeded: " + String(setupSucceeded));
}

void SDCard::test() {
  SDBlockDevice *bd = new SDBlockDevice((PinName)KYWY_SD_CARD_MOSI, (PinName)KYWY_SD_CARD_MISO, (PinName)KYWY_SD_CARD_CLK, (PinName)KYWY_SD_CARD_CS, 8000000, true);
  // mbed::BlockDevice *bd = mbed::BlockDevice::get_default_instance();
  mbed::LittleFileSystem fs("fat");

  Serial.println();
  Serial.println();

  int err;

  // Serial.print("Initializing the block device... ");
  // err = bd->init();
  // Serial.println(err ? "FAIL" : "OK");

  // Serial.print("Erasing the block device... ");
  // err = bd->erase(0, bd->size());
  // Serial.println(err ? "FAIL" : "OK");

  // Serial.print("Deinitializing the block device... ");
  // err = bd->deinit();
  // Serial.println(err ? "FAIL" : "OK");

  Serial.print("Reformatting... ");
  err = fs.reformat(bd);
  Serial.println(err ? "FAIL" : "OK");

  Serial.print("Mounting the file system... ");
  err = fs.mount(bd);
  Serial.println(err ? "FAIL" : "OK");

  if (err) {
      // Reformat if we can't mount the filesystem
      Serial.print("Reformatting... ");
      err = fs.reformat(bd);
      Serial.println(err ? "FAIL" : "OK");

      Serial.print("Mounting the file system... ");
      err = fs.mount(bd);
      Serial.println(err ? "FAIL" : "OK");
  }

  while (true) {
    delay(1000);
  }




  // setup();

  // File testFile;
  // testFile = SD.open("test.txt", FILE_WRITE);
  // if (testFile) {
  //   testFile.println("Hello, SD card!"); //writes this to the file everytime but I dont care enough to fix.
  //   testFile.close();
  //   Serial.println("File written successfully.");
  // } else {
  //   Serial.println("Failed to open file for writing.");
  // }

  // // Read back the file
  // testFile = SD.open("test.txt");
  // if (testFile) {
  //   Serial.println("Reading file content:");
  //   while (testFile.available()) {
  //     Serial.write(testFile.read());
  //   }
  //   testFile.close();
  // } else {
  //   Serial.println("Failed to open file for reading.");
  // }
}

} // namespace Kywy