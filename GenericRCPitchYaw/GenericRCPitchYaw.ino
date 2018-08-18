#include <SPI.h>  
#include "RF24.h"
#include <Wire.h>
#include <MPU6050_tockn.h>

MPU6050 mpu6050(Wire);

RF24 myRadio (7, 8);

byte addresses[][6] = {"0"};

String X[] = {"left","right","center"};
String Y[] = {"forward","backward","none"};

struct package{
  enum XDIRECTION {LEFT,RIGHT,CENTER};
  enum YDIRECTION {FORWARD,BACKWARD,NONE};
  
  XDIRECTION aX;
  YDIRECTION aY;
  
  XDIRECTION bX;
  YDIRECTION bY;
  int aValY, aValX, bValY, bValX;
  bool buttonA, buttonB, readGyro;
  byte packetTest = 0;
  int errorX, errorY;
};

int valX, valY;
byte stickA[] = {0,1};
byte stickB[] = {3,2};

float startX,startY,startZ;


typedef struct package Package;
Package data;

void setup(){
  pinMode(stickA[0],INPUT);
  pinMode(stickA[1],INPUT);
  pinMode(stickB[0],INPUT);
  pinMode(stickB[1],INPUT);

  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);

  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);

  Wire.begin();        
  Serial.begin(115200);                                   

  digitalWrite(4,HIGH);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  startX = mpu6050.getAngleX();
  startY = mpu6050.getAngleY();
  startZ = mpu6050.getAngleZ();
       
  digitalWrite(4,LOW);
  digitalWrite(5,HIGH);
  delay(500);
  digitalWrite(5,LOW);

  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  myRadio.stopListening();
}

void steer(){
  valX = analogRead(stickA[0]);
  valY = analogRead(stickA[1]);
  //read analog to determine direction
  if (valY > 730){
    data.aY = package::YDIRECTION::BACKWARD;
    data.aValY = map(valY,1023,515,255,0);
  }else if (valY < 280){
    data.aY = package::YDIRECTION::FORWARD;
   data.aValY = map(valY,1,415,255,0);
  }else{
    data.aY = package::YDIRECTION::NONE;
   data.aValY = 0;
  }

  if (valX > 720){
    data.aX = package::XDIRECTION::LEFT;
    data.aValX = map(valX,505,1024,0,255);
  }else if (valX < 280){
    data.aX = package::XDIRECTION::RIGHT;
    data.aValX = map(valX,1,505,255,0);
  }else{
    data.aX = package::XDIRECTION::CENTER;
    data.aValX = 0;
  } 
  data.aValX = abs(data.aValX);
  data.aValY = abs(data.aValY);

  //A is done
///*
  valX = analogRead(stickB[0]);
  valY = analogRead(stickB[1]);
  //read analog to determine direction
  if (valY > 730){
    data.bY = package::YDIRECTION::BACKWARD;
    data.bValY =valY;// map(valY,1023,515,255,0);
  }else if (valY < 280){
    data.bY = package::YDIRECTION::FORWARD;
   data.bValY = valY;//map(valY,1,415,255,0);
  }else{
    data.bY = package::YDIRECTION::NONE;
   data.bValY = 0;
  }
  Serial.println(valX);
  if (valX > 750){
    data.bX = package::XDIRECTION::LEFT;
    data.bValX = valX;//map(valX,505,1024,0,255);
  }else if (valX < 280){
    data.bX = package::XDIRECTION::RIGHT;
    data.bValX = valX;//map(valX,1,505,255,0);
  }else{
    data.bX = package::XDIRECTION::CENTER;
    data.bValX = 0;
  } 
  data.bValX = abs(data.bValX);
  data.bValY = abs(data.bValY);
  
  if (digitalRead(2) == LOW){
    data.buttonA = true;
  }else{
    data.buttonA = false;
  }
  if (digitalRead(3) == LOW){
    Serial.println("pew");
    data.buttonB = true;
  }else{
    data.buttonB = false;
  }
  
 // */

  //B is done
}

float findError(float got, float expected){
  return ((float)(got-expected)/expected);
}

void loop(){
   steer();
   myRadio.write(&data, sizeof(data)); 
   data.packetTest++;


 Serial.print("Ax:");
  Serial.print(X[(data).aX]);
  Serial.print(" | Ay:");
  Serial.print(Y[(data).aY]);
  Serial.print(" | Bx:");
  Serial.print(X[(data).bX]);
  Serial.print(" | By:");
  Serial.print(Y[(data).bY]);
  if (digitalRead(2) == LOW){
    Serial.print(" | ButtonA");
  }
  if (digitalRead(3) == LOW){
    Serial.print(" | ButtonB");
  }
  if (analogRead(7) >400){
    Serial.print(" | Reading Gyro");
    data.readGyro= true;
  }else{
    data.readGyro=false;
  }
  
  Serial.println(" |");
  mpu6050.update();
  data.errorX=mpu6050.getAngleX();//findError(mpu6050.getAngleX(),startX);
  data.errorY=mpu6050.getAngleY();//findError(mpu6050.getAngleY(),startY);
//  data.errorZ=(mpu6050.getAngleZ(),startZ);
  
  Serial.print("ErrorX : ");
  Serial.print(data.errorX);
  Serial.print(" | ErrorY : ");
  Serial.println(data.errorY);

  delay(100);
  
 /*
  count+=PI/64;
  float temp = sin(count)*sin(20*count);
  temp*=1000;
   Serial.println(temp);
   */
}


