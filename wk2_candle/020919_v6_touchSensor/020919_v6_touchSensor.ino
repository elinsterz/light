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

int sensorPin = A0;               //touch sensor for now

const int neoPixelPin = 5;        // control pin
const int pixelCount = 7;         // number of pixels
int centerPixel = 0;              // center pixel
int pixels[] = {0,1,2,3,4,5,6};   // all pixels      
 
int h[] = {12,15,20,15,20,15,20};            // hue
int s = 100;                                 // saturation
int i[] = {50,50,50,50,50,50,50};            // intensity

//calm values
int calmChange[] = {1,2,6,2,6,2,6};                 // increments for change
int calmLowerRange[] = {80,10,20,10,20,10,20};       // values for lower range
int calmUpperRange[] = {100,50,70,50,70,50,70};     // values for upper range

//crazy values
int crazyChange[] = {1,5,15,50,5,15,50};              // increments for change
int crazyLowerRange[] = {50,0,40,60,0,40,60};       // values for lower range
int crazyUpperRange[] = {100,40,80,100,40,80,100};    // values for upper range

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

  // run timer
  timer.run();

  // print sensor value 
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);

  // if sensor is  pressed, state is crazy
  if(sensorValue >= 1){
    wildState();
    Serial.println("wild state");
  } 

  // if sensor is not pressed, state is calm
  if(sensorValue == 0){
    calmState();
    Serial.println("calm state");
  } 
}

void calmState(iChange,lowerRng,upperRng){

  int change = iChange;
  int lowerRange = lowerRng;
  int upperRange = upperRng;
  
  //for loop to go through elements of pixel array
  for (int j= 0; j<7; j++){
    // create a color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(h[j], s, i[j]);
    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);    
    strip.show();   // update the strip
  }
  
  // fade intensity with an array:
  for (int x = 0; x< 7; x++){
    i[x] = i[x] + change[x];
    if(i[x] < lowerRange[x]  ||  i[x] > upperRange[x]) {
    change[x] = -change[x];
    }
  }
  delay(100);
}

void crazyState(){
  //for loop to go through elements of pixel array
  for (int j= 0; j<7; j++){
    // create a color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(h[j], s, i[j]);
    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);    
    strip.show();   // update the strip
  }
  
  // fade intensity with an array:
  for (int x = 0; x< 7; x++){
    i[x] = i[x] + crazyChange[x];
    if(i[x] < crazyLowerRange[x]  ||  i[x] > crazyUpperRange[x]) {
    crazyChange[x] = -crazyChange[x];
    }
  }
  delay(20);
}

void wildState(){
  // variable for intensity of "wild" state 
  int iWild[] = {random(0,5),random(0,50),random(0,5),random(0,50),random(0,5),random(0,50),random(0,5)};

  //for loop to go through elements of pixel array
  for (int j= 0; j<7; j++){
    // create a single color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(h[j], s, iWild[j]);
    
    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);    
    strip.show();   // update the strip
  }
}


