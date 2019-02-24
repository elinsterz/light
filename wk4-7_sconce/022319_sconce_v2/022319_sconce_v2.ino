/* Key Sconce: Touch Sensor with High Watt LED + Transistor
 *  version 2
 *  02/23/19
 *  Emily Lin
 */
 
int sensor_pin = A0;
int led_1 = 8;
int led_2 = 10;
int high_watt_led = 4;

int touch_state;

void setup() {
  Serial.begin(9600);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(high_watt_led, OUTPUT);
}

void loop() {
  touch_state = analogRead(sensor_pin);

  // if sensor pin is pressed, turn on high watt led
  if (touch_state == 1023){
    digitalWrite(high_watt_led, HIGH);
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
  Serial.print("high_watt_led: ");  
  Serial.println(high_watt_led); 
}
