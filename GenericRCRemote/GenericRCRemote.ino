#include <SPI.h>  
#include "RF24.h"

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
  bool buttonA, buttonB;
  byte packetTest = 0;
};

int valX, valY;
byte stickA[] = {0,1};
byte stickB[] = {3,2};

typedef struct package Package;
Package data;

void setup(){
  pinMode(stickA[0],INPUT);
  pinMode(stickA[1],INPUT);
  pinMode(stickB[0],INPUT);
  pinMode(stickB[1],INPUT);
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);

  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  
    
  Serial.begin(115200);
  delay(1000);
  myRadio.begin();  
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openWritingPipe( addresses[0]);
  myRadio.stopListening();
 // delay(1000);
}

void steer(){
  valX = analogRead(stickA[0]);
  valY = analogRead(stickA[1]);
  //read analog to determine direction
  if (valY > 530){
    data.aY = package::YDIRECTION::BACKWARD;
    data.aValY = map(valY,1023,515,255,0);
  }else if (valY < 480){
    data.aY = package::YDIRECTION::FORWARD;
   data.aValY = map(valY,1,415,255,0);
  }else{
    data.aY = package::YDIRECTION::NONE;
   data.aValY = 0;
  }

  if (valX > 520){
    data.aX = package::XDIRECTION::LEFT;
    data.aValX = map(valX,505,1024,0,255);
  }else if (valX < 480){
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
  if (valY > 530){
    data.bY = package::YDIRECTION::BACKWARD;
    data.bValY =valY;// map(valY,1023,515,255,0);
  }else if (valY < 480){
    data.bY = package::YDIRECTION::FORWARD;
   data.bValY = valY;//map(valY,1,415,255,0);
  }else{
    data.bY = package::YDIRECTION::NONE;
   data.bValY = 0;
  }

  if (valX > 520){
    data.bX = package::XDIRECTION::LEFT;
    data.bValX = valX;//map(valX,505,1024,0,255);
  }else if (valX < 480){
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
  
  Serial.println(" |");
 // Serial.print("\nPackage:");
  Serial.println((&data)->aValY);
  Serial.print("\n");
  delay(10);

}
