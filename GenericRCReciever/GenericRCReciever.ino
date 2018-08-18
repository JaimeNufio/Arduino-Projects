#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (8,7);
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
  byte aValY, aValX, bValY, bValX;
  bool buttonA, buttonB;
};

typedef struct package Package;
Package data;

void setup() {
  pinMode(2,OUTPUT);
  pinMode(4,OUTPUT);
  Serial.begin(115200);
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

void loop() {

  if ( myRadio.available()) {
    // digitalWrite(sign,HIGH);
    while (myRadio.available()){
      myRadio.read( &data, sizeof(data) );
    }
    Serial.print("Ax:");
    Serial.print(X[(&data)->aX]);
    Serial.print(" | Ay:");
    Serial.print(Y[(&data)->aY]);
    Serial.print(" | Bx:");
    Serial.print(X[(&data)->bX]);
    Serial.print(" | By:");
    Serial.print(Y[(&data)->bY]);
    if (data.buttonA == true){
      Serial.print(" | ButtonA");
      digitalWrite(2,HIGH);
    }else{
      digitalWrite(2,LOW);
    }
    if (data.buttonB == true){
      Serial.print(" | ButtonB");
      digitalWrite(4,HIGH);
    }else{
      digitalWrite(4,LOW);
    }
    Serial.println(":)");
  }else{
//    digitalWrite(sign, LOW);
    //Serial.println(":(");
  }
}

