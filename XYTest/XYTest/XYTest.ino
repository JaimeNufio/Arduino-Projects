#include <Servo.h>

Servo myservo, leftarm, rightarm;

int xVal = 0, yVal = 1, sensorVal = 0;
int xPos = 0, yPos = 0;
int lastXPos = 0, lastYPos = 0;

float stp = 10;
float correctMax = 1023;

void setup() {
  myservo.attach(9);
  leftarm.attach(5);
  rightarm.attach(6);
  Serial.begin(9600);
  leftarm.write(90);
  rightarm.write(90);
  myservo.write(90);
}

int aproach(int goal, int num){
  int x = (goal - num)/abs(goal - num);
 // Serial.println(x);
 // Serial.println(xPos);
  return x;  
}

void loop() {
  delay(1);
  sensorVal = 1.8*analogRead(xVal)*(100/correctMax);

    //Serial.println(sensorVal);
    //Serial.println(xPos);
  
  if (sensorVal > 100 || sensorVal < 80){
    xPos+=aproach(90,sensorVal)*stp;
  }else{
  //  Serial.println("X no movement");
  }
   
  if (xPos > 180){
    xPos = 175;
  }else if (xPos < 0){
    xPos = 5;
  }  

  sensorVal = .9*analogRead(yVal)*(100/correctMax);

  if (sensorVal > 60 || sensorVal < 40){
      yPos+=aproach(45,sensorVal)*stp;
  }else{
   // Serial.println("Y no movement");
  }

  if (yPos > 90){
    yPos = 90;
  }else if (yPos < 0){
    yPos = 0;
  }

  if (xPos != lastXPos){
    Serial.println(xPos);
    myservo.write(xPos);  
  }else{
   // Serial.println("X no write");
  }
  if (yPos != lastYPos){
    leftarm.write(180-yPos);
    rightarm.write(yPos);  
  }else{
   // Serial.println("Y no write");
  }
  
  lastXPos = xPos;
  lastYPos = yPos;
}
