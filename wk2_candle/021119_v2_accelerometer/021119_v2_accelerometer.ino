/*
  Candle
  Light & Interactivity Class
  Emily Lin

  02/11/19

  Notes: 
  - this one has all the different states, but is with a touch sensor
  - need to replace with accelerometer
*/

// libraries 
#include <SimpleTimer.h>
#include <Adafruit_NeoPixel.h>
#include <ColorConverter.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

//accelerometer object
Adafruit_MMA8451 mma = Adafruit_MMA8451();

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

//off values
int iOff[] = {0,0,0,0,0,0,0};


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

  Serial.begin(9600);  //initalize Serial

  //wait 1 sec then call function
  timer.setInterval(1000, repeatMe); 

  //setup taken from MMA8451 demo
  //serial prints if accelerometer is found or not
  Serial.println("Adafruit MMA8451 test!"); 
  
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");
}
 
void loop() {

  /*///////// ACCELEROMETER READING & SETUP //////////*/
  /* Taken from MMA8451 demo, serial print the acceleration values. Read the 'raw' data in 14-bit counts */
  
  mma.read();
  Serial.print("X:\t"); Serial.print(mma.x); 
  Serial.print("\tY:\t"); Serial.print(mma.y); 
  Serial.print("\tZ:\t"); Serial.print(mma.z); 
  Serial.println();

  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.println("m/s^2 ");
  
  /* Get the orientation of the sensor */
  uint8_t o = mma.getOrientation();
  
  // array of variables for intensity of "wild" state 
  int iWild[] = {random(0,5),random(0,50),random(0,5),random(0,50),random(0,5),random(0,50),random(0,5)};

  // run timer
  timer.run();
  
  
  /*///////// ACCELEROMETER //////////*/
  // if statements that respond to accelerometer settings

  // if acc y is > 2 or < 0, go to normal,crazy state
   if ((event.acceleration.y > 2) || (event.acceleration.y < 0)){
    
    //pass crazy state values into function normal state
    normalState(crazyChange, crazyLowerRange, crazyUpperRange);
    Serial.println("crazy");
    
  } 
    // else if acc z is < 2  or acc z is > 10, go to wild
    else if (event.acceleration.z >10) {
      wildState(iWild);
      Serial.println("wild state");

      // if sensor is pressed for 5 seconds, turn leds off
      int timerId = timer.setTimer(5000, offState, 1);
      
  } else {
  
  //else it is normal, calm state
    normalState(calmChange, calmLowerRange, calmUpperRange);
    Serial.println("calm state");
  }
}

/*///////// PREVIOUS CODE WITH TOUCH SENSOR //////////*/

//  // print sensor value 
//  int sensorValue = analogRead(sensorPin);
//  Serial.println(sensorValue);
//
//  // if sensor is  pressed, state is crazy
//  if(sensorValue >= 1){
//    //pass iWild intensity into function
//    wildState(iWild);
//    
//    //normalState(crazyChange, crazyLowerRange, crazyUpperRange);
//    Serial.println("wild state");
//    
//    // if sensor is pressed for 5 seconds, turn leds off
//    int timerId = timer.setTimer(5000, offState, 1);
//  } 
//
//  // if sensor is not pressed, state is calm
//  if(sensorValue == 0){
//    normalState(calmChange, calmLowerRange, calmUpperRange);
//    Serial.println("calm state");
//  } 


/*///////// NORMAL STATE //////////*/

void normalState(int iChange[],int lowerRng[],int upperRng[]){
  //for loop to go through elements of pixel array
  for (int j= 0; j<7; j++){
    // create a color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(h[j], s, i[j]);
    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);    
    strip.show();   // update the strip
  }
  
  // fade intensity with an array:
  for (int x = 0; x< 7; x++){
    i[x] = i[x] + iChange[x];
    if(i[x] < lowerRng[x]  ||  i[x] > upperRng[x]) {
    iChange[x] = -iChange[x];
    }
  }
  delay(100);
}

/*///////// WILD STATE //////////*/
void wildState(int intensityW[]){

  //for loop to go through elements of pixel array
  for (int j= 0; j<7; j++){
    // create a single color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(h[j], s, intensityW[j]);
    
    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);    
    strip.show();   // update the strip
  }
}

/*///////// OFF STATE //////////*/
void offState(){
  int iOff = 0;
  
  //for loop to go through elements of pixel array
  for (int j= 0; j<7; j++){
    // create a single color from hue, sat, intensity:
    RGBColor color = converter.HSItoRGB(0, 0, iOff);
    
    strip.setPixelColor(pixels[j], color.red, color.green, color.blue);    
    strip.show();   // update the strip
  }
  delay(1000);
}
