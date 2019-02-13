/*
  Candle
  Light & Interactivity Class
  Emily Lin
  02/12/19

  
  Notes:
  - changed plans after deciding that accelerometer was not a great interaction
  - went back to touch sensor
  *** WORKING! ****
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

int h[] = {10, 15, 20, 15, 20, 15, 20};      // hue
int s = 100;                                 // saturation
int i[] = {80, 30, 10, 30, 30, 10, 30};      // intensity

//calm values
int calmChange[] = {1, 2, 8, 2, 8, 2, 8};           // increments for change
int calmLowerRange[] = {10, 5, 2, 5, 2, 5, 2 }; // values for lower range
int calmUpperRange[] = {100, 25, 70, 25, 70, 25, 70}; // values for upper range

//crazy values
int crazyChange[] = {1, 5, 15, 50, 5, 15, 1};        // increments for change
int crazyLowerRange[] = {1, 5, 2, 5, 2, 5, 1}; // values for lower range
int crazyUpperRange[] = {10, 40, 50, 100, 180, 40, 10}; // values for upper range

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
  long iWild[] = {random(0, 30), random(0, 6), random(0, 1), random(0, 2), random(0, 6), random(0, 1), random(0, 2)};

  // run timer (maybe not neccesary?)
  //timer.run();

//  // print sensor value
//  int sensorValue = analogRead(sensorPin);
//  Serial.println(sensorValue);

  current = analogRead(sensorPin);

  // if the sensor is pressed, remember the start time
  // change to crazy state
  if(current >= 1023){
    
    normalState(crazyChange, crazyLowerRange, crazyUpperRange);
    Serial.println("crazy state");
    
    //if time running minus is over 4 seconds, update the firstTime to be this moment
    if (millis() - firstTime >= 5000) {
      
      firstTime = millis();       
      
    }
    
    // variable to count seconds
    secs_held = (millis() - firstTime) / 1000;
    Serial.println("seconds held: ");
    Serial.println(secs_held);

   // If the button was held for 4 seconds, go from crazy to wild state  
    if (secs_held >= 2) {

      wildState(iWild);
      Serial.println("wild state");    
    }


    // If the button was held for 3 seconds go from wild to off state  
    if (secs_held >= 4) {

      offState();
      Serial.println("offState state");
      delay(500);
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
  delay(8000);
}
