/* 
 *  Candle - Accelerometer Test
 *  Light & Interactivity
 *  Emily Lin
 *  02/11/19
 */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MMA8451.h>

const int sda = A4;
const int scl = A5;

void setup() {
  //initialize serial:
  Serial.begin(9600);
}

void loop() {
  Serial.println(scl);
  Serial.println(sda);

}
