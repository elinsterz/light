/*Emily Lin
 * CO2 Light 
   04/24/19

   Notes: 
   - Trying to get CO2 readings to write onto SD Card: "CO2LIGHT"
   - SD CARD pin select: ??

   Credits:
   - Tom Igoe |  MKR Dataset Examples (https://github.com/tigoe/DataloggingExamples/blob/master/MKR_examples/MKR_Datestamped_Files/MKR_Datestamped_Files.ino_
   - Edited from CCS811 air library

*/


/***************************************************************************
  This is a library for the CCS811 air

  This sketch reads the sensor

  Designed specifically to work with the Adafruit CCS811 breakout
  ----> http://www.adafruit.com/products/3566

  These sensors use I2C to communicate. The device's I2C address is 0x5A

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "Adafruit_CCS811.h"
#include <SD.h>
#include <RTCZero.h>
#include <SPI.h>




Adafruit_CCS811 ccs;

// The chip select pin
// For MKRZero it's SDCARD_SS_PIN
// for mem shield, it's 4
const int SD_CHIP_SELECT = 4;

//whether or not the SD card initialized:
bool SDAvailable = false;
//name of the file to write to:
String logFile = "DATALOG.CSV";

long timestamp = 0;  // timestamp for SD logging, in ms
int interval = 1000;  // logging interval in ms

//init realtimeclock:
RTCZero rtc;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;


void setup() {
  Serial.begin(9600);

  Serial.println("CCS811 test");

  //initialize the realtimeclock:
  rtc.begin();

  // initialize SD card:
  SDAvailable = SD.begin(SD_CHIP_SELECT);
  Serial.println("Card working: " + String(SDAvailable));

  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, SD_CHIP_SELECT)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
  }

  if (!ccs.begin()) {
    Serial.println("Failed to start sensor! Please check your wiring.");
    while (1);
  }

  //calibrate temperature sensor
  while (!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);

  //set time and date from the compiler:
  setTimeFromCompile();
  setDateFromCompile();

  //get file name from date:
  logFile = fileNameFromDate();

  
  // print a header to the SD card file:
  File dataFile = SD.open(logFile, FILE_WRITE);
  if (dataFile) {
    dataFile.println("Battery voltage:,%:, elapsed seconds:");
    dataFile.close();
  }
}

void loop() {
  if (ccs.available()) {
    float temp = ccs.calculateTemperature();
    if (!ccs.readData()) {
      Serial.print("CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.print(ccs.getTVOC());
      Serial.print("ppb   Temp:");
      Serial.println(temp);
    }
    else {
      Serial.println("ERROR!");
      while (1);
    }
  }
  delay(500);

  //If the SD card intialized correctly, 
  //write to it:
  if (SDAvailable){
    //update log file name if needed:
    logFile = fileNameFromDate();
    File dataFile = SD.open(logFile, FILE_WRITE);
    dataFile.print("CO2: ");
    dataFile.print(ccs.geteCO2());
    dataFile.print("ppm,");
    dataFile.print("TVOC: ");
    dataFile.print(ccs.getTVOC());
    dataFile.print("ppb,");    
    dataFile.print("Temp: ");
    //dataFile.print(temp);
    dataFile.print("temp");    

    dataFile.println(float(timestamp / 1000.0)); // new line, print time
    dataFile.close();

    //note the card write time:
    Serial.println("cardwrite time: " + String(millis() - timestamp) + " ms");
  }
}

String fileNameFromDate(){
  String result = "";
  if(rtc.getDay() < 10) result += "0";
  result += String(rtc.getDay());
  if(rtc.getMonth() < 10) result += "0";
  result += String(rtc.getMonth());
  if(rtc.getYear() < 10) result += "0";
  result += String(rtc.getYear());
  result += ".CSV";
  return result; 
}

//set the rtc time from the compile time:
void setTimeFromCompile(){
  //get the compile time string:
  String compileTime = String(__TIME__);

  //break the compile time on the colons:
  int h = compileTime.substring(0,2).toInt();
  int m = compileTime.substring(3,5).toInt();
  int s = compileTime.substring(6,8).toInt();

  //set the time from the derived numbers:
  rtc.setTime(h, m, s);
}

//set the rtc time from the compile date:
void setDateFromCompile(){
  String months[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };
  //get the compile date:
  String compileDate = String(__DATE__);
  //get the date substring
  String monthStr = compileDate.substring(0,3);

  int m = 0;  //variable for the date as an integer
  //see which month matches the month String:
  for (int i = 0; i < 12; i++){
    if (monthStr == months[i]){
      //the first month is 1, but its array position is 0, so 
      m = i+1;
      //stop loop
      break;
    }
  }

  //get the day and year as substrings, and convert to numbers:
  int d = compileDate.substring(4,6).toInt();
  int y = compileDate.substring(9,11).toInt();

  //set the date from the derived numbers:
  rtc.setDate(d,m,y);  
}






