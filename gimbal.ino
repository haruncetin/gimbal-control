#include <Servo.h>

#define PANPOT      0       // analog pin used to connect the pan potentiometer
#define TILTPOT     1       // analog pin used to connect the tilt potentiometer
#define PANSERVO    9
#define TILTSERVO   10

Servo pan, tilt;            // pan & tilt objects       
int pan_val, tilt_val;      // variables to read the values from the analog pan & tilt pins

void setup() {
  pan.attach(PANSERVO);            // attaches the servo on pin 9 to the servo object that drive the pan servo
  tilt.attach(TILTSERVO);          // attaches the servo on pin 10 to the servo object that drive the tilt servo
}

void loop() {
  pan_val = analogRead(PANPOT);             // reads the value of the pan potentiometer (value between 0 and 1023)
  pan_val = map(pan_val, 0, 1023, 0, 180);      // scale it to use it with the pan servo (value between 0 and 180)
  pan.write(pan_val);                           // sets the pan servo position according to the scaled value
  tilt_val = analogRead(TILTPOT);           // reads the value of the tilt potentiometer (value between 0 and 1023)
  tilt_val = map(tilt_val, 0, 1023, 0, 180);    // scale it to use it with the tilt servo (value between 0 and 180)
  tilt.write(tilt_val);                         // sets the tilt servo position according to the scaled value
  delay(25);                                    // waits for the servos to get there
}
