// Libraries required//////////////////////////////////////////////////

#include <Stepper.h>
#include "Adafruit_CCS811.h"
#include "RTClib.h"

//Variables/////////////////////////////////////////////////////////////

//LEDs //–––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––//
//Digital pins connected to transisor base for RGB control
#define REDPIN 5      //red color
#define GREENPIN 6     //green color
#define BLUEPIN 3      //blue color

#define FADESPEED 100     // make this higher to slow down the color change rate

//Stepper Motor //–––––––––––––––––––––––––––––––––––––––––––––––––––––––//

# define STEPS 200 //define the number of steps per revolution (Range: 1-200)


//CCS811 CO2,Temp Sensor //––––––––––––––––––––––––––––––––––––––––––––––//

//Nothing yet defined



//Instances for classes used here///////////////////////////////////////

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's 
// attached to including BIN1, BIN2, AIN1 and ain2
Stepper stepper(STEPS, 11, 10, 9, 8); 

//Adafruit CO2 sensor ccs definition
Adafruit_CCS811 ccs;

// Timer initiation
RTC_Millis rtc;

///////////////////////////////////////////////////////////////////////////

void setup() {
  // RTC timer setup
  Serial.begin(57600);
  rtc.begin();
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
  }
  
  //LED pin assignments as outputs
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);

  Serial.println("CCS811 test");
  if(!ccs.begin()){
    Serial.println("Failed to start sensor! Please check your wiring.");
    while(1);
  }

  //calibrate temperature sensor
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);

}

/////////////////////////////////////////////////////////////////////////////

void loop() {

//Read CO2 and Temperature and assign variables to it––––––––––––––––––––//
// !!! we might have to build in an averaging functiopn so the motor speed does not
// change too erratically
float temp = ccs.calculateTemperature();
float CO2 = ccs.geteCO2();
float TVOC = ccs.getTVOC();

  if(ccs.available()){
//    float temp = ccs.calculateTemperature();
//    float CO2 = ccs.geteCO2();
//    float TVOC = ccs.getTVOC();
    
    if(!ccs.readData()){
      Serial.print("CO2: ");
      Serial.print(CO2);
      Serial.print("ppm, TVOC: ");
      Serial.print(TVOC);
      Serial.print("ppb   Temp:");
      Serial.println(temp);
    }
    else{
      Serial.println("ERROR!");
      while(1);
    }
  }
  delay(500);


 
//Display by Stepper rotation speed––––––––––––––––––––//

  if (CO2 < 500){
    stepper.setSpeed(20); 
  } else if ( 500 <= CO2 < 800){
    stepper.setSpeed(50); 
  } else if (800 <= CO2 < 1500 ){
    stepper.setSpeed(100);
  }else{
    stepper.setSpeed(120);
  }

  // Speed will change based on the CO2 level
  // will proabably need mapping function
  Serial.println("Forward");
  stepper.step(STEPS);
 
 
//Display by LED––––––––––––––––––––––––––––––––––––––––//
  int r, g, b;
// tets arrays
//  int arr_0 [3] = {0, 0, 255};
//  int arr_1 [3] = {180, 0, 0}; 
//int arr_0 [3] = {199, 217, 234};
//int arr_1 [3] = {242, 135, 111};

// defining color presets as arrays:
// these sets will be used to indicate cold, chilly, warm and hot temperatures
int color_sets[][3] = {{199, 217, 234}, {242, 135, 111}, {255, 255, 0}, {255, 255, 255}};


//Here we look at the time (hour) reading and change the color based on that
// for further details check out the rtc_format example for the RTClib.h

    DateTime now = rtc.now();
    char buf[100];
    strncpy(buf,"DD.MM.YYYY  hh:mm:ss\0",100);
    Serial.println(now.format(buf));
    delay(1000);
    Serial.println(now.second()); // we should check against now.hour() to see what time of the day
//    
//    if (now.second() <= 10){
//      Serial.println("GO TO BED!!");
//      set_LED_colors(color_sets[2]);
//    } else{
//      set_LED_colors(color_sets[3]);
//    }


//    set_LED_colors(color_sets[2]);
//    delay(1000);
//    change_LED_colors(color_sets[2], color_sets[3]);
//    delay(1000);
//    set_LED_colors(color_sets[3]);
//    delay(5000);

}

/////////////////////////////////////////////////////////////////////////////
//functions used for preparing the LED strip in various colors

void set_LED_colors (int color[3]) {
  //variables fed into the function are indexed for R,G,B
  // This makes it easier to feed color combination of interest into the LEDs

  analogWrite(REDPIN, color[0]);
  analogWrite(GREENPIN, color[1]);
  analogWrite(BLUEPIN, color[2]);
}


void change_LED_colors (int color_i[3], int color_f[3]) {
  //This function will help fade from one color preset (i) to another set (f)
  //by slowly fading red, then green and finally blue
   
  //initial color combination
  int r_i = color_i[0];
  int g_i = color_i[1];
  int b_i = color_i[2];
  
  //final color combination
  int r_f = color_f[0];
  int g_f = color_f[1];
  int b_f = color_f[2];

  // delay for color transitions
  const int r_delay = 10; 
  const int g_delay = 10; 
  const int b_delay = 10;

    // fade red pin first
    // the while loop compares the red values, if there is a difference it activates a fade 
      
    while (r_f != r_i) {
      analogWrite(REDPIN, r_i);
      delay(r_delay);
      // if statement determines whether final or initial r value is larger
      // based on the result the r_i value will slowly increase or decrease
      if (r_f > r_i) { r_i++; } else { r_i--;}
    }

    // fade green pin second
     // the while loop compares the green values, if there is a difference it activates a fade 
    while (g_f != g_i) {
      analogWrite(GREENPIN, g_i);
      delay(g_delay);
      
      if (g_f > g_i) { g_i++; } else { g_i--;}
    }
 
    // fade blue pin third
    // the while loop compares the blue values, if there is a difference it activates a fade 
    while (b_f != b_i) {
      analogWrite(BLUEPIN, b_i);
      delay(b_delay);
      
      if (b_f > b_i) { b_i++; } else { b_i--;}
    }

}
