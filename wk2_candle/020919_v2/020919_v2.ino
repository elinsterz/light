#include <SimpleTimer.h>
#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>

// the timer object
SimpleTimer timer;
 
const int neoPixelPin = 5;   // control pin
const int pixelCount = 7;    // number of pixels
int change = 1;              // increment to change hue by
int onePixel = 0;
 
// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);
ColorConverter converter;
 
int h = 25;         // hue
int s = 100;        // saturation
int i = 60;        // intensity

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

  // set the color for this pixel
  strip.setPixelColor(onePixel, color.red, color.green, color.blue);    
  strip.show();   // update the strip
  delay(100);

   // increment hue to fade from red (0) to reddish orange (15) and back:
  h = h + change;
  if (h < 20 || h > 30) {
    change = -change;
  }
}
