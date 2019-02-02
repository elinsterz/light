int intensity = 255;
float change = 2.55;
int pot_pin = A0;
int no_intensity = 0;

int start;


void setup() {
  // put your setup code here, to run once:
  pinMode(5, OUTPUT);
  start = true;
}

void loop() {
  // read if pot has been turned on
  start = (analogRead(pot_pin) > 0);
  
  // if the pot is turned on, fade in and out
  if (start){
      if (intensity <= 0 || intensity>= 255){
      change = -change;
  }
  intensity += change;
  analogWrite(5, intensity);
  delay(50);
  }

  else {
    start = false;
    analogWrite(5, no_intensity);
  }
}



// 2500 = 2.5 seconds
// delay = 1/5 of a second

//sensor reading in middle to interrupt
//capacitor to smooth change


