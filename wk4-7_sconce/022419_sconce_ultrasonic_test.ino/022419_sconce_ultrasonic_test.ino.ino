/* Not sure if i will use ultrasonic anymore
 
/* Ultrasonic Code not my own!
    Code from tutorial:
    https://www.youtube.com/watch?v=ZejQOX69K5M
*/

const int trigPin = 12;
const int echoPin = 11;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delay(200);

  digitalWrite(trigPin, HIGH);
  delay(100);
  digitalWrite(trigPin, LOW);

  // high means pulsing function will wait for the pin to go high caused by the bounce soundwave
  duration = pulseIn (echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);
}
