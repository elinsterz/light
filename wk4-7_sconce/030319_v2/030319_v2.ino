/* Key Sconce: Touch Sensor with High Watt LED + Speaker
    version 3
    03/03/19
    Emily Lin
*/

int sensor_pin = A0;
int high_watt_led = 3;
int speaker_pin = 8; 

int touch_state;
int counter;

void beep_off(){
  noTone(speaker_pin);
}

//function is called only once
void beep_on(){
  tone(speaker_pin,300); 
  Serial.println("beep on");
}

void setup() {
  Serial.begin(9600);
  pinMode(high_watt_led, OUTPUT);
  pinMode(speaker_pin, OUTPUT);
//  tone(speaker_pin,300); 
}

void loop() {
  touch_state = analogRead(sensor_pin);

//if button is pressed
 if (touch_state == 1023){
  Serial.println("button pressed");
  counter++; // add to counter
  if(counter < 3){
    Serial.println(counter);
    tone(speaker_pin,300); 
    delay(1000);
  } else {
    noTone(speaker_pin);
  }
 } else {
  counter = 0;
  noTone(speaker_pin);
 }
}
