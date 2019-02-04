int intensity = 255;
float change = 2.55;
int pot_pin = A0;
int no_intensity = 0;
int led_1 = 3;
int led_2 = 5;
int led_3 = 6;
int led_4 = 10;
int led_5 = 11;

int ledPins[] = {3,5,6,10,11};

int start;


void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < 5; i++){
   pinMode(ledPins[i], OUTPUT);
  }
  start = true;
}

void loop() {
  // read if pot has been turned on
  start = (analogRead(pot_pin));
  Serial.println(start);
  
  // if the pot is turned on, fade in and out
  if (start){
      if (intensity <= 0 || intensity >= 255){
      change = -change;
  }
  intensity += change;
  for (int i = 0; i<5; i++){
    analogWrite(ledPins[i], intensity);
  }
  delay(50);
  }

  else {
    start = false;
    for (int i = 0; i < 5; i++) {
      analogWrite(ledPins[i], no_intensity);
    }
  }
}



// 2500 = 2.5 seconds
// delay = 1/5 of a second

//sensor reading in middle to interrupt
//capacitor to smooth change


