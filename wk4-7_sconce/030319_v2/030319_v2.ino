/* Key Sconce: Touch Sensor with High Watt LED + Speaker
    version 3
    03/03/19
    Emily Lin
*/

#include <SimpleTimer.h>
SimpleTimer timer;

int sensor_pin = A0;
int high_watt_led = 3;
int speaker_pin = 8; 

int touch_state;

void beep_off(){
  noTone(speaker_pin);
}

//function is called only once
void beep_on(){
  tone(speaker_pin,300); 
  Serial.println("beep on");
  timer.setTimeout(1000, beep_off); // turn off after 1 second
}

void setup() {
  Serial.begin(9600);
  pinMode(high_watt_led, OUTPUT);
  pinMode(speaker_pin, OUTPUT);
  tone(speaker_pin,300); 
}

void loop() {
  touch_state = analogRead(sensor_pin);

  // if sensor pin is pressed, turn on high watt led
  // if sensor pin is pressed, speaker pin beeps
  if (touch_state == 1023) {
    digitalWrite(high_watt_led, HIGH);
    Serial.println("high");

    timer.setTimeout(1, beep_on);
 
    //tone(speaker_pin,300);  
  } else {
    digitalWrite(high_watt_led, LOW);
    //noTone(speaker_pin);
    beep_off();
  }

  /*
    // if sensorPin is pressed, turn on led1
    if (touchState == 1023){
     digitalWrite(led_1, HIGH);
    } else {
     digitalWrite(led_1, LOW);
    }
  */
  
  timer.run();

  // serial print touch state
  Serial.print("touch state: ");
  Serial.println(touch_state);

}
