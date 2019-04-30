#include <SerialFlash.h>
#include <SD.h>
#include <SPI.h>

const int SDchipSelect = 4;    // Audio Shield has SD card CS on pin 10
const int FlashChipSelect = 5; // digital pin for flash chip CS pin


void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);
Serial.println("Hi");


}



void loop() {

  


  if (!SD.begin()) {
    Serial.println("Unable to access SD card");
  }
  if (!SerialFlash.begin(FlashChipSelect)) {
    Serial.println("Unable to access SPI Flash chip");
  }

// // initialize SD card:
//  bool SDAvailable = SD.begin(SD_CHIP_SELECT);
//  if (SDAvailable) {
//    String logFile = "DATALOG.CSV";
//    File dataFile = SD.open(logFile, FILE_WRITE);
//    if (dataFile) {
//        dataFile.println("Writing to the file");
//        dataFile.close();
//    }
//  }
}
