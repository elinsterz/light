/* Key Sconce: Touch Sensor with High Watt LED + Speaker
    version 3
    03/03/19
    Emily Lin

    Notes: When button is pressed the speaker will beep for 1 second
*/



int sensor_pin = A0;
int high_watt_led = 3;
int speaker_pin = 8; 

int touch_state;
int counter;


void setup() {
  Serial.begin(9600);
  pinMode(high_watt_led, OUTPUT);
  pinMode(speaker_pin, OUTPUT);
}

void loop() {
  touch_state = analogRead(sensor_pin);

//if button is pressed, start counting 
 if (touch_state == 1023){
  Serial.println("button pressed");
  counter++; // add to counter
  
  // if counter is less than 2 seconds
  if(counter < 2){
    Serial.println(counter); 
    beep_on(); //start beeps
    delay(1000); 
  } else {
    beep_off(); //stop beeps
  }
 } else {
  counter = 0; //reset counter
  beep_off();
 }
}

//function for stopping beep
void beep_off(){
  noTone(speaker_pin);
  Serial.println("beep off");
}

//function for beeping
void beep_on(){
  tone(speaker_pin,300); 
  Serial.println("beep on");
}
