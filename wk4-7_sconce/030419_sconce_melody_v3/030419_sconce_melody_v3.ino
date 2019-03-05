/* Key Sconce: Touch Sensor + Speaker
    03/04/19 (Monday)
    Emily Lin

    Interaction Notes:
    1. if key is on handle:
    fade light on
    play tune on speaker
    animate fade to match with length of tune

    2. if key is not on handle:
    turn light off

    3. if key is on handle, but room is dark:
    dim light so it is a night light

    ----

    Melody Code Credits:
    Arduino, Tom Igoe
    The melody code is in the public domain. http://www.arduino.cc/en/Tutorial/Tone
*/

#include "pitches.h"

#define PI 3.1415926535897932384626433832795


int sensor_pin = A0;
int high_watt_led = 3;
int speaker_pin = 11; //PWM pin to control the volume through Pulse Width
int number_of_notes = 11; //number of notes played in melody

int touch_state; // touch state is analog read of touch sensor
int counter;
int brightness = 0;  //brightness of led

//var for fade
float intensity = 3;
float amplitude;
float change = 2.55;
int start;

// notes in the melody
int melody[] = {
  NOTE_E5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_C6, NOTE_D6, NOTE_B5, NOTE_G5, NOTE_C6,
  NOTE_A5, NOTE_A5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  8, 4, 8, 8, 4, 8, 8, 4, 8, 4, 4
};

void setup() {
  Serial.begin(9600);
  pinMode(high_watt_led, OUTPUT);
  pinMode(speaker_pin, OUTPUT);
  start = true;
}

void loop() {
  //analog read for the touch sensor
  touch_state = analogRead(sensor_pin);

  intensity += change;
  amplitude =  sin(PI * intensity/255) ;
  
  Serial.println(amplitude);


  //if button is pressed, start counting
  if (touch_state == 1023) {
    //turn led on
    analogWrite(high_watt_led, amplitude);

    Serial.println("button pressed");
    counter++; // add to counter

    // if counter is less than 2 seconds:
    if (counter < 2) {

      Serial.println(counter);
      play_melody(); //play melody
      delay(1000);

    } else {
      beep_off(); //stop beeps
    }
  } else {
    //turn led off
    digitalWrite(high_watt_led, LOW);

    counter = 0; //reset counter
    beep_off();  //turn beep off
  }
}

//function for stopping beep
void beep_off() {
  noTone(speaker_pin);
  Serial.println("beep off");
}

void play_melody() {
  // iterate over the notes of the melody:
  int this_note;
  for (int this_note = 0; this_note < number_of_notes; this_note++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[this_note];
    tone(speaker_pin, melody[this_note], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    // stop the tone playing:
    noTone(speaker_pin);
  }
}
