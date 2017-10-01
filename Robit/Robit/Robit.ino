#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 //max 400-500

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
int d = 5;
int cnt = 0;
int cntMax = d*5;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  for (pos = 0; pos <= 180; pos++) { // goes from 0 degrees to 180 degrees
    delay(d);
    cnt+=d;
    if (cnt >= cntMax){
      cnt = 0;
      unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS). 
      delay(20);
      Serial.println(uS/US_ROUNDTRIP_CM);
      if (uS / US_ROUNDTRIP_CM > 1 && uS/US_ROUNDTRIP_CM < 10){
        Serial.println("Target aquired");
        pos=pos-1;
      }else{
        myservo.write(pos);
      }
    }
     
    //Serial.print("Ping: ");
    //Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
    //Serial.println("cm");
    //if ((uS/US_ROUNDTRIP_CM) > 100){
                             // waits 15ms for the servo to reach the position
                   
    
  }
//  for (pos = 180; pos > 0; pos--){
  //  delay(d);
//    myservo.write(pos);
//  }
}
