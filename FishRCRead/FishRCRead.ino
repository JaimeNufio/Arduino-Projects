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
};

typedef struct package Package;
Package data;

byte motorA[] = {3,4}, motorB[] = {5,6}, radioFound, ena2 = 9, ena1 = 10;

void setup() {
  Serial.begin(115200);
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  
  for (int i = 0; i<2; i++){
    pinMode(motorA[i],OUTPUT);
    pinMode(motorB[i],OUTPUT);
  }
  
  pinMode(ena1, OUTPUT); 
  pinMode(ena2, OUTPUT);
//  pinMode(sign,OUTPUT);

  //default, forward.
 
  //digitalWrite(sign,HIGH);
}

void mouthTest(){
  digitalWrite(ena2,250);
  digitalWrite(motorB[0],LOW);
  digitalWrite(motorB[1],HIGH);
  delay(1000);
  digitalWrite(ena2,0);
  digitalWrite(motorB[0],HIGH);
  digitalWrite(motorB[1],LOW);
  digitalWrite(ena2,250);
  delay(1000);
}

void loop() {
  mouthTest();
/*
  digitalWrite(motorA[0],LOW);
  digitalWrite(motorA[1],HIGH);
  digitalWrite(motorB[0],HIGH);
  digitalWrite(motorB[1],LOW);
  digitalWrite(ena1,250);
  delay(500);
  digitalWrite(ena2,250);
  delay(2000);
  digitalWrite(ena2,0);
  delay(1000);
  digitalWrite(ena1,0);
  delay(5000);
  */
/*
  if ( myRadio.available()) {
//     digitalWrite(sign,HIGH);
    while (myRadio.available()){
      myRadio.read( &data, sizeof(data) );
    }
    Serial.println(":)");
  }else{
//    digitalWrite(sign, LOW);
    Serial.println(":(");
  }
  */
}
