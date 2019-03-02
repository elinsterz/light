/* Key Sconce: Touch Sensor with High Watt LED + Speaker
    version 3
    03/03/19
    Emily Lin
*/


int sensor_pin = A0;
int high_watt_led = 3;
int speaker_pin = 8; 


int touch_state;

void setup() {
  Serial.begin(9600);
  pinMode(high_watt_led, OUTPUT);
  pinMode(speaker_pin, OUTPUT);
}

void loop() {
  touch_state = analogRead(sensor_pin);

  // if sensor pin is pressed, turn on high watt led
  // if sensor pin is pressed, speaker pin beeps
  if (touch_state == 1023) {
    digitalWrite(high_watt_led, HIGH);
    Serial.println("high");
    tone(speaker_pin,300, 4);  
  } else {
    digitalWrite(high_watt_led, LOW);
    noTone(speaker_pin);
  }

  /*
    // if sensorPin is pressed, turn on led1
    if (touchState == 1023){
     digitalWrite(led_1, HIGH);
    } else {
     digitalWrite(led_1, LOW);
    }
  */

  // serial print touch state
  Serial.print("touch state: ");
  Serial.println(touch_state);

}
