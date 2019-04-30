/* Emily Lin
 *  04/28/19
 * 
 * Notes: 
 * - The power supplied can be no more than 2.8 - 3.3V
 * - "stepper.step" can be no less than 10-20 (otherwise it gets steppy)
 * - only change "stepper.setsSpeed()"
 * 
 * Credits: 
 * Tom Igoe, stepper reference library: https://www.arduino.cc/en/Tutorial/StepperOneRevolution
 * Adafruit: https://learn.adafruit.com/adafruit-tb6612-h-bridge-dc-stepper-motor-driver-breakout/using-stepper-motors
 * 
 */


#include <Stepper.h>

//change this to the number of steps on your motor
# define STEPS 200


// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's 
// attached to

Stepper stepper(STEPS, 4, 5, 6, 7);


void setup() {
  Serial.begin(9600);
  Serial.println("Stepper test!");
  // set the speed of the motor to 30 RPMS
  stepper.setSpeed(100);
}

void loop() {
  Serial.println("Forward");
  stepper.step(10);
  //Serial.println("Backward");
  //stepper.step(-100);
}
