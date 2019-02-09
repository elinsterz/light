
#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>
 
const int neoPixelPin = 5;   // control pin
const int pixelCount = 7;    // number of pixels
int change = 5;              // increment to change hue by
int onePixel = 0;
 
// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);
ColorConverter converter;
 
int h = 45;         // hue
int s = 80;        // saturation
int i = 40;        // intensity
 
void setup() {
  strip.begin();    // initialize pixel strip
  strip.clear();    // turn all LEDs off
  strip.show();     // update strip

  Serial.begin(9600);
}
 
void loop() {
  // create a single color from hue, sat, intensity:
  RGBColor color = converter.HSItoRGB(h, s, i);
 
  strip.setPixelColor(onePixel, color.red, color.green, color.blue);    // set the color for this pixel
  strip.show();   // update the strip
  delay(100);
 
  // increment hue to fade from red (0) to reddish orange (15) and back:
  h = h + change;
  if (h < 15 || h > 55) {
    change = -change;
    Serial.println(onePixel);
  }
}
