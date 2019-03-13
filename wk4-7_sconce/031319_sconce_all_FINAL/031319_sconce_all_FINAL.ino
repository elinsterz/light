/* Key Sconce: Force Sensor + Speaker
    03/13/19 (Wednesday)
    Emily Lin

    Interaction Notes:
    1. if key is on handle:
    fade light on
    play on melody


    2. if key is not on handle:
    turn light off
    play off melody

    3. if key is on handle, but room is dark:
    dim light so it is a night light

    ----

    Melody Code Credits:
    Arduino, Tom Igoe
    The melody code is in the public domain. http://www.arduino.cc/en/Tutorial/Tone
*/

#include "pitches.h"


int sensor_pin = A0;
int photores_pin = A2;
int high_watt_led = 3;
int speaker_pin = 11; //PWM pin to control the volume through Pulse Width
int num_notes_on = 2; //number of notes played in melody
int num_notes_off = 11; //number of notes played in melody

long int touch_state; // touch state is analog read of touch sensor
int light_state; //if photoresistor gets light
int counter = 0;
int brightness = 0;  //brightness of led

//var for fade
float intensity = 3;
float amplitude;
float change = 2.55;
int start;

// notes in the melody
int melody_on[] = {
  NOTE_A5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_C6, NOTE_D6, NOTE_B5, NOTE_G5, NOTE_C6,
  NOTE_A5, NOTE_A5
};

int melody_off[] = {
  NOTE_G5, NOTE_F5, NOTE_G5, NOTE_F5, NOTE_C6, NOTE_D6, NOTE_B5, NOTE_G5, NOTE_C6,
  NOTE_A5, NOTE_A5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations_on[] = {
  4, 4, 8, 8, 4, 8, 8, 4, 8, 4, 4
};

int noteDurations_off[] = {
  4, 4, 4, 10, 4, 8, 8, 4, 8, 4, 4
};

void setup() {
  Serial.begin(9600);
  pinMode(high_watt_led, OUTPUT);
  pinMode(speaker_pin, OUTPUT);
  start = true;
}

void loop() {

  // if photoresistor gets any light
  light_state = analogRead(photores_pin);
  Serial.print("Light: ");
  Serial.println(light_state);

  //if photoresistor is reading a light state over x amount then perform if statements below
  if (light_state >= 100) {

    //analog read for the touch sensor
    touch_state = analogRead(sensor_pin);

    //  Serial.println(touch_state);

    //if FSR is pressed, start counting
    if (touch_state >= 100) {
      // if counter is less than 2 seconds:
      if (counter < 2) {
        //play on melody
        play_melody(melody_on, noteDurations_on, num_notes_on); 
        delay(1000);
      }

      //turn led on with fade
      analogWrite(high_watt_led, counter + 1);

      //delay(200);
      Serial.println("button pressed");
      counter++; // add to counter
      
    } else {

      //if counter is greater than or equal to 3 seconds, play off melody 
      if (counter >= 3) {
        play_melody(melody_off, noteDurations_off, num_notes_off); //play on melody
      }

      // if FSR not pressed, turn led off and reset counter
      analogWrite(high_watt_led, 0);
      counter = 0; //reset counter
    }
    
  // if light state(photores is not > 100) is very low, turn everything off
  } else {
    //turn led off
    analogWrite(high_watt_led, 0);
  }
}

/* not using beep function anymore
//function for stopping beep
void beep_off() {
  noTone(speaker_pin);
  //Serial.println("beep off");
}
*/

void play_melody(int mel[11], int noteD[11], int num_notes ) {
  // iterate over the notes of the melody:
  int this_note;
  for (int this_note = 0; this_note < num_notes; this_note++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteD[this_note];
    tone(speaker_pin, mel[this_note], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    noTone(speaker_pin);
  }
}
