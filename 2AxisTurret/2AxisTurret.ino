#include <Servo.h>

Servo servo;
Servo servo1;
int pos = 0;

void setup(){
  servo.attach(2);
  servo1.attach(3);
  //servo.write(0);
  delay(2000);
}

void loop() {
  for (pos = 0; pos <= 80; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    servo1.write(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 80; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos); 
    servo1.write(pos);                // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
