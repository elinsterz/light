
int pot_pin = 0;
int led_pin = 5;
int pot_max = 255;
int pot_min = 0;
int increment = 5;
int count;
int fade_down;
int fade_up;
int start;

void setup() {
  // define pins and booleans
  pinMode(led_pin, OUTPUT);
  pinMode(pot_pin, INPUT);
  count = 0;
  start = true;
  fade_up = true;
}


void loop() {
  start = (analogRead(pot_pin) > pot_min);
  
  // the fade starts if the pot is above pot minimum
  if (start){
  // if the pot amount is increasing, have the increment increase from count
      if(fade_up){
        count += increment;
        Serial.print(count);
        // if fading amount is above pot minimum then it is fading up
        if(fade_up >= pot_min){
          fade_up = true;
        }
      }
    // if the pot amount is decreasing, have the increment decrease from count
    else {
      count -= increment;
     Serial.print(count);
      if(count >= 255){
        fade_up = false;
      }
    } 
  }
// light up led 
analogWrite(led_pin, count);   
}
