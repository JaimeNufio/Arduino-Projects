#include <Servo.h>

Servo myservo; 
Servo leftarm;
Servo rightarm;

int xVal = 0;
int yVal = 1;
int sensorVal = 0;
int xPos = 0;
int  yPos = 0;
float correctMax = 1023;

void setup() {
  myservo.attach(9);
  leftarm.attach(5);
  rightarm.attach(6);
  Serial.begin(9600);
}

int aproach(int goal, int num){
  int x = (goal - num)/abs(goal - num);
  Serial.println(x);
  Serial.println(xPos);
  return x;
}

void loop() {
  delay(5);
  sensorVal = 1.8*analogRead(xVal)*(100/correctMax);
  if (xPos > 100 || xPos < 80){
    xPos+=aproach(90,sensorVal);
  }
  if (xPos > 180){
    xPos = 180;
  }else if (xPos < 0){
    xPos = 0;
  }  
  
  sensorVal = 1.8*analogRead(yVal)*(100/correctMax);
  if ((sensorVal) > 100){
    yPos++;
  }else if ((sensorVal) < 80 ){
    yPos--;
  }
  if (yPos > 180){
    yPos = 180;
  }else if (yPos < 0){
    yPos = 0;
  }
  myservo.write(xPos);
  leftarm.write(yPos);
  rightarm.write(180-yPos);
}
