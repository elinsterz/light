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
#include <SPI.h>
#include <SD.h>
#include <RTCZero.h>

RTCZero rtc;

const int chipSelect = SDCARD_SS_PIN;

long timestamp = 0;  // timestamp for SD logging, in ms
int interval = 5000;  // logging interval in ms


File logFile;

String textFile;



Adafruit_CCS811 ccs;

void setup() {
  Serial.begin(9600);

//////// Set date and time counters /////

// initialize the realtime clock:
  rtc.begin();
  // set time and date from the compiler:
  setTimeFromCompile();
  setDateFromCompile();
  
  textFile = fileNameFromDate();

/////////////// CO2 sensor Init ///////////
  Serial.println("CCS811 test");

  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  //calibrate temperature sensor
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);
  
/////////////// SD Card Init ///////////
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

// open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  logFile = SD.open(textFile, FILE_WRITE);

  // if the file opened okay, write to it:
  if (logFile) {
    Serial.print("Writing data");
    logFile.println("Test");
    logFile.print("Time (s) ");
    logFile.print(",");
    logFile.print("CO2 (ppm) ");
    logFile.print(",");
    logFile.print("TVOC (ppb) ");  
    logFile.print(",");
    logFile.println("Temperature (C) ");
    // close the file:
    logFile.close();
    Serial.println("first line done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening data.txt");
  }

}



void loop() {
  
  if(ccs.available()){
//   float temp = ccs.calculateTemperature();
    if(!ccs.readData()){
      Serial.print("CO2: ");
      Serial.print(ccs.geteCO2());
      Serial.print("ppm, TVOC: ");
      Serial.print(ccs.getTVOC());
      Serial.print("ppb   Temp:");
      Serial.println(ccs.calculateTemperature());
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(1000);

 if (millis() - timestamp > interval) {
    // read the time
    timestamp = millis();

  textFile = fileNameFromDate();
  logFile = SD.open(textFile, FILE_WRITE);

  // if the file opened okay, write to it:
  if (logFile) {
    Serial.print("Writing to data.txt...");
    logFile.print(float (timestamp / 1000.0));
    logFile.print(",");
    logFile.print(ccs.geteCO2());
    logFile.print(",");
    logFile.print(ccs.getTVOC());
    logFile.print(",");
    logFile.println(ccs.calculateTemperature());
//    logFile.println(",");
//    logFile.println("C"); 
    // close the file:
    logFile.close();
    Serial.println("done.");
    Serial.println("card write time: " + String(millis() - timestamp) + " ms");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
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


