/*
  Candle
  Light & Interactivity Class
  Emily Lin

*/


#include <SimpleTimer.h>
#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>

// the timer object
SimpleTimer timer;
 
const int neoPixelPin = 5;        // control pin
const int pixelCount = 7;         // number of pixels
int centerPixel = 0;              // center pixel
int pixels[] = {0,1,2,3,4,5,6};   // all pixels
 
int h[] = {12,15,20,15,20,15,20};            // hue
int s = 100;                                 // saturation
int i[] = {50,50,50,50,50,50,50};            // intensity

int calmChange[] = {1,2,4,2,4,2,4};                 // increments for change
int calmLowerRange[] = {80,60,10,60,10,60,10};       // values for lower range
int calmUpperRange[] = {100,80,30,80,30,80,30};     // values for upper range

int crazyChange[] = {1,5,15,70,5,50,70};              // increments for change
int crazyLowerRange[] = {50,0,0,0,0,0,0};             // values for lower range
int crazyUpperRange[] = {100,40,80,100,40,80,100};  // values for upper range

int wildChange[] = {random(10,50), random(20,30), random(30,60), random(60,90), random(20,30), random (30,60), random(60,90)};  
int wildLowerRange[] = {0,random(0,1),random(0,1),random(0,1),random(0,1),random(0,1),random(0,1) };            // values for lower range
int wildUpperRange[] = {random(0,100),random(0,30),random(0,50),random(0,30),random(0,50),random(0,30),random(0,50)};   // values for upper range


// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);
ColorConverter converter;

// counts seconds
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

int iWild[] = {random(0,5),random(0,50),random(0,5),random(0,50),random(0,5),random(0,50),random(0,5)};
  
  // run timer
  timer.run();
  
//  // create a single color from hue, sat, intensity:
//  RGBColor color = converter.HSItoRGB(h, s, i);

  //for loop to go through elements of pixel array
  for (int j= 0; j<7; j++){
    // create a single color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(h[j], s, iWild[j]);
    
    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);    
    strip.show();   // update the strip
  }

//  // set the color for this pixel
//  strip.setPixelColor(centerPixel, color.red, color.green, color.blue);    
//  strip.show();   // update the strip
    delay(100);

//  fade intensity with an array:
// for (int x = 0; x< 7; x++){
//   i[x] = i[x] + wildChange [x];
//   if(i[x] < wildLowerRange[x]  ||  i[x] > wildUpperRange[x]) {
//    wildChange[x] = -wildChange[x];
//   }
// }

//  // fade intensity:
//  i = i + change;
//  if (i < 40 || i > 80) {
//    change = -change;
//  }

  

  // fade hue:
//  h = h + change;
//  if (h < 10 || h > 80) {
//    change = -change;
//  }
}
