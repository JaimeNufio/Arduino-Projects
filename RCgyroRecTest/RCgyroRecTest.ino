
#include <SPI.h>  
#include "RF24.h"
#include "nRF24L01.h"
#include <Wire.h>
RF24 myRadio (7,8);


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

int xBound =45, yBound = 45;

byte addresses[][6] = {"0"}; 
typedef struct package Package;
Package data;

int motorA[] = {7,6,5}; //last is the enable
int motorB[] = {8,9,17};
/////int control[] = {0,1};

int sign = 2;

float favor;

String X[] = {"LEFT","RIGHT","CENTER"};
String Y[] = {"FORWARD","BACKWARD","NONE"};

void setup() {

  Serial.begin(115200);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
 
  // put your setup code here, to run once:
 myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX); //switch MIN to MAX, HIgh or LOW
  myRadio.setDataRate(RF24_250KBPS);//( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  myRadio.printDetails();
  Serial.println("Radio should be set");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
  if (myRadio.available()) {
     Serial.println("data");
      while (myRadio.available()){
          myRadio.read( &data, sizeof(data) );
          if (data.errorX > xBound){
            digitalWrite(5,HIGH);
            digitalWrite(4,LOW);
          }else if (data.errorX < -xBound){
            digitalWrite(4,HIGH);
            digitalWrite(5,LOW);
          }else{
            digitalWrite(5,LOW);
            digitalWrite(4,LOW);
          }
          if (data.errorY > yBound){
            digitalWrite(2,HIGH);
            digitalWrite(3,LOW);
          }else if (data.errorY < -yBound){
            digitalWrite(3,HIGH);
            digitalWrite(2,LOW);
          }else{
            digitalWrite(2,LOW);
            digitalWrite(3,LOW);
          }
                Serial.println(data.errorX);
      Serial.println(data.errorY);
      }

  }else{
    Serial.println("no");
    digitalWrite(2,LOW);
    digitalWrite(3,LOW);
    digitalWrite(4,LOW);
    digitalWrite(5,LOW);
  }
   

}
