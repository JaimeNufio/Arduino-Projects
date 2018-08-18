#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7,8);

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

byte addresses[][6] = {"0"}; 
typedef struct package Package;
Package data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(3,OUTPUT);
//  pinMode(10,OUTPUT);
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
} 

void loop() {
  // put your main code here, to run repeatedly:
  if ( myRadio.available()) {
      while (myRadio.available()){
        myRadio.read( &data, sizeof(data) ); 
        //Jaime you're an idiot who missed this very important line
        Serial.println(data.packetTest);
        if (data.aValX != 0 || data.aValY != 0){
          Serial.println("buttons");
          digitalWrite(3,HIGH);
          delay(10);
        }
    }
  
    Serial.println(":)");
  }else{
          digitalWrite(3,LOW);
  //  digitalWrite(sign, LOW);
  Serial.println(":(");
  }
  delay(10);
}
