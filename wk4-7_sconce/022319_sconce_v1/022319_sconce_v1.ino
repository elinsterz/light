/* Key Sconce: Touch Sensor Test
 *  02/23/19
 *  Emily Lin
 */
 
int sensorPin = A0;
int led1 = 8;
int led2 = 10;

int touchState;

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop() {
  touchState = analogRead(sensorPin);
  
  // if sensorPin is pressed, turn on led1
  if (touchState == 1023){
    digitalWrite(led1, HIGH);
  } else {
    digitalWrite(led1, LOW);
  }

  Serial.println(touchState);
  Serial.println(led1);
}
