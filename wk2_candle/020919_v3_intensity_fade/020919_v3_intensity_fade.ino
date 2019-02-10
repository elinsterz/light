/*
  Candle
  Lighting & Interactivity Class
  Emily Lin
  
  - NeoPixel RGBW x 1
  - Piezo Sensor x 1

*/


#include <SimpleTimer.h>
#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>

// the timer object
SimpleTimer timer;
 
const int neoPixelPin = 5;   // control pin
const int pixelCount = 7;    // number of pixels
int change = 5;              // increment to change hue by
int centerPixel = 0;              // center pixel
int pixels[] = {0,1,2,3,4,5,6};   // all pixels
 
int h = 25;         // hue
int s = 100;        // saturation
int i = 60;        // intensity

 
// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);
ColorConverter converter;

// a function to be executed periodically
void repeatMe() {
    Serial.print("Uptime (s): ");
    Serial.println(millis() / 1000);
}
 
void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // update strip

  Serial.begin(9600);

  //wait 1 sec then call function
  timer.setInterval(1000, repeatMe); 
}
 
void loop() {
  // run timer
  timer.run();
  
  // create a single color from hue, sat, intensity:
  RGBColor color = converter.HSItoRGB(h, s, i);

  //for loop to go through elements of pixel array
  for (int j= 0; j<7; j++){
    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);    
    strip.show();   // update the strip
  }

//  // set the color for this pixel
//  strip.setPixelColor(centerPixel, color.red, color.green, color.blue);    
//  strip.show();   // update the strip
  delay(100);


  // fade intensity:
  i = i + change;
  if (i < 40 || i > 80) {
    change = -change;
  }

  

  // fade hue:
//  h = h + change;
//  if (h < 10 || h > 80) {
//    change = -change;
//  }



}
