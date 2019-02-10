int piezoPin = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int piezoValue = analogRead(piezoPin);
  Serial.println(piezoValue);
}
