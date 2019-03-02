/* Key Sconce: Touch Sensor with High Watt LED + Transistor
 *  version 2
 *  02/27/19
 *  Emily Lin
 */
 
int sensor_pin = A0;
int high_watt_led = 3;

int touch_state;

void setup() {
  Serial.begin(9600);
  pinMode(high_watt_led, OUTPUT);
}

void loop() {
  touch_state = analogRead(sensor_pin);

  // if sensor pin is pressed, turn on high watt led
  if (touch_state == 1023){
    digitalWrite(high_watt_led, HIGH);
    Serial.println("high");
  } else {
    digitalWrite(high_watt_led, LOW);
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
