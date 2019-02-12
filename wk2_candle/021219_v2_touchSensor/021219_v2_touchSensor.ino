/*
  Candle
  Light & Interactivity Class
  Emily Lin
  02/11/19
  Notes:
  - changed plans after deciding that accelerometer was not a great interaction
  - went back to touch sensor
  - button hold reference: https://playground.arduino.cc/Code/HoldButton
  *** STILL NOT WORKING! ****
*/


#include <SimpleTimer.h>
#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>

// the timer object
SimpleTimer timer;

int sensorPin = A0;               //touch sensor for now

const int neoPixelPin = 5;        // control pin
const int pixelCount = 7;         // number of pixels
int centerPixel = 0;              // center pixel
int pixels[] = {0, 1, 2, 3, 4, 5, 6}; // all pixels

int h[] = {12, 15, 20, 15, 20, 15, 20};      // hue
int s = 100;                                 // saturation
int i[] = {50, 50, 50, 50, 50, 50, 50};      // intensity

//calm values
int calmChange[] = {1, 2, 6, 2, 6, 2, 6};           // increments for change
int calmLowerRange[] = {80, 10, 20, 10, 20, 10, 20}; // values for lower range
int calmUpperRange[] = {100, 50, 70, 50, 70, 50, 70}; // values for upper range

//crazy values
int crazyChange[] = {1, 5, 15, 50, 5, 15, 50};        // increments for change
int crazyLowerRange[] = {50, 0, 40, 60, 0, 40, 60}; // values for lower range
int crazyUpperRange[] = {100, 40, 80, 100, 40, 80, 100}; // values for upper range

//off values
int iOff[] = {0, 0, 0, 0, 0, 0, 0};

//timer variable
//int timerId;

//capture current time

int current;  // current state of the button
long millis_held;  // how long was the button held (millisesconds)
long secs_held;    // how long was the button held (seconds)
byte previous = HIGH;
unsigned long firstTime = 0;  // how long since the button was first pressed


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
  //timer.setInterval(1000, repeatMe);
}

void loop() {

  // variable for intensity of "wild" state
  long iWild[] = {random(0, 5), random(0, 50), random(0, 5), random(0, 50), random(0, 5), random(0, 50), random(0, 5)};

  // run timer (maybe not neccesary?)
  //timer.run();

//  // print sensor value
//  int sensorValue = analogRead(sensorPin);
//  Serial.println(sensorValue);

  current = analogRead(sensorPin);

  // if the button is pressed, remember the start time
  // change to wild
  if(current >= 1023){
    
    wildState(iWild);
    Serial.println("wild state");
    

    if (millis() - firstTime >= 5000) {

      firstTime = millis();
         
    }
//
//    Serial.println ("firstTime:");
//    Serial.println(firstTime);

    secs_held = (millis() - firstTime) / 1000;
    Serial.println("seconds held: ");
    Serial.println(secs_held);

    // If the button was held for 5 seconds go from wild to off state  
    if (secs_held >= 4) {

      offState();
      Serial.println("offState state");
      delay(10000);
    }
  
 
  }


  // if sensor is not pressed, state is calm
  if (current == 0) {
    normalState(calmChange, calmLowerRange, calmUpperRange);
    Serial.println("calm state");
    firstTime = 0;
  }
}

void normalState(int iChange[], int lowerRng[], int upperRng[]) {
  //for loop to go through elements of pixel array
  for (int j = 0; j < 7; j++) {
    // create a color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(h[j], s, i[j]);
    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);
    strip.show();   // update the strip
  }

  // fade intensity with an array:
  for (int x = 0; x < 7; x++) {
    i[x] = i[x] + iChange[x];
    if (i[x] < lowerRng[x]  ||  i[x] > upperRng[x]) {
      iChange[x] = -iChange[x];
    }
  }
  delay(100);
}

void wildState(long intensityW[]) {

  //for loop to go through elements of pixel array
  for (int j = 0; j < 7; j++) {
    // create a single color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(h[j], s, intensityW[j]);

    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);
    strip.show();   // update the strip
  }
}

void offState() {
  int iOff = 0;

  //for loop to go through elements of pixel array
  for (int j = 0; j < 7; j++) {
    // create a single color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(0, 0, iOff);

    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);
    strip.show();   // update the strip
  }
  delay(1000);
}



/*
 * 
  // loop for 5 seconds
  while (millis() - startTime < 5000) {

    // if sensor is  pressed, state is wild
    if (sensorValue >= 1) {
      cnt++; // increment counter

      wildState(iWild);
      Serial.println("wild state");
    }
  }

  // Time is up
  if (cnt == 4) {
    offState();
    Serial.println("off state");
  }
 */
