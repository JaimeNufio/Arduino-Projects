#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (3,4);

struct package{
  enum XDIRECTION {LEFT,RIGHT,CENTER};
  enum YDIRECTION {FORWARD,BACKWARD,NONE};
  
  XDIRECTION aX;
  YDIRECTION aY;
  
  XDIRECTION bX;
  YDIRECTION bY;
  int aValY, aValX, bValY, bValX;
  bool buttonA, buttonB;
  byte packetTest;
};

byte addresses[][6] = {"0"}; 
typedef struct package Package;
Package data;

int motorA[] = {7,6,5}; //last is the enable
int motorB[] = {8,9,10};
//int control[] = {0,1};

int sign = 2,valX,valY;

float favor;

String X[] = {"LEFT","RIGHT","CENTER"};
String Y[] = {"FORWARD","BACKWARD","NONE"};

//x 0,505,1022
//y 0,515,1023

void setup() {
  
  Serial.begin(115200);
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  
  for (int i = 0; i<3; i++){
    pinMode(motorA[i],OUTPUT);
    pinMode(motorB[i],OUTPUT);
  }
  
 // pinMode(control[0],INPUT); 
 // pinMode(control[1],INPUT);
  pinMode(sign,OUTPUT);

  //default, forward.
  digitalWrite(motorA[0],HIGH);
  digitalWrite(motorB[0],HIGH);
  digitalWrite(motorA[1],LOW);
  digitalWrite(motorB[1],LOW);
  digitalWrite(motorA[2],0);
  digitalWrite(motorB[2],0);
  digitalWrite(sign,HIGH);
  delay(1000);
}

void setSpd(){
  
  int valX = data.aValX, valY = data.aValY;
  favor = ((float)valX)/(255*2);
  if (data.aX == package::XDIRECTION::LEFT){
//  Serial.println("LEFT");
    analogWrite(motorA[2],(int)(valY*favor));//Left Wheel
    analogWrite(motorB[2],valY);
  }else if (data.aX == package::XDIRECTION::RIGHT){
//   Serial.println("RIGHT");
    analogWrite(motorA[2],valY);
    analogWrite(motorB[2],(int)(valY*favor));
  }else{
 //   Serial.println("not left or right");
    analogWrite(motorA[2],valY);
    analogWrite(motorB[2],valY);
  }
}

void spin(){
    int valX = data.aValX;
  //  Serial.print("spin-");
    digitalWrite(motorA[0],LOW);
    digitalWrite(motorB[0],LOW);
    digitalWrite(motorA[1],HIGH);
    digitalWrite(motorB[1],HIGH);
  if (data.aX == package::XDIRECTION::LEFT){
    analogWrite(motorA[2],0);
    analogWrite(motorB[2],valX);
  //  Serial.println("left");
  }else if (data.aX == package::XDIRECTION::RIGHT){
    analogWrite(motorA[2],valX);
    analogWrite(motorB[2],0);
 //   Serial.println("right");
  }else{
    digitalWrite(motorA[0],LOW);
    digitalWrite(motorB[0],LOW);
    digitalWrite(motorA[1],LOW);
    digitalWrite(motorB[1],LOW);
    analogWrite(motorA[1],0); 
    analogWrite(motorA[2],0);    
  }
}

void setDir(){
  if (data.aY == package::YDIRECTION::FORWARD){
    digitalWrite(motorA[0],LOW);
    digitalWrite(motorB[0],LOW);
    digitalWrite(motorA[1],HIGH);
    digitalWrite(motorB[1],HIGH);
 //   Serial.print("wheels forawrd");
//    Serial.print(", favors ");
 //   Serial.println(X[data.x]);
    setSpd();
  }else if (data.aY == package::YDIRECTION::BACKWARD){
    digitalWrite(motorA[0],HIGH);
    digitalWrite(motorB[0],HIGH);
    digitalWrite(motorA[1],LOW);
    digitalWrite(motorB[1],LOW);
  //  Serial.print("wheels backward");
 //   Serial.print(", favors ");
  //  Serial.println(X[data.x]);
    setSpd();
  }else{
   // Serial.println("No Y movement, ");
    data.aValY = 0; 
    spin();
  }

}

void loop() {

 // Serial.println("---");
 // Serial.println(data.packetTest);
//  Serial.println("---");
  if ( myRadio.available()) {       
   // bool done = false;
    while(myRadio.available()){
      myRadio.read( &data, sizeof(data) );
      delay(5);
      Serial.println(data.packetTest); 
    //  digitalWrite(sign,HIGH);
    //   setDir();
/*
      Serial.print("Ax:");
      Serial.print(X[(data).aX]);
      Serial.print(" | Ay:");
      Serial.print(Y[(data).aY]);
      Serial.print(" | Bx:");
      Serial.print(X[(&data)->bX]);
      Serial.print(" | By:");
      Serial.print(Y[(&data)->bY]);
     */
///*
      if (data.buttonA){
        Serial.print(" | ButtonA");
      }
      if (data.buttonB){
        Serial.print(" | ButtonB");
      }
      Serial.println(" ");
//  */
   }
   // Serial.println(":)");
  }else{
    digitalWrite(sign, LOW);
    Serial.println(":(");
  }

  /*
  Serial.print("X: ");
//  Serial.print(X[x]);
  Serial.print(" ");
  Serial.print(valX);
  Serial.print(" Y: ");
 // Serial.print(Y[y]);
  Serial.print(" ");
  Serial.print(valY);  
  Serial.print(" with modifier ");
  Serial.println(favor);
  delay(200);*/
}
