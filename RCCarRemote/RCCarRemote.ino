//#include <EEPROM.h>

#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7, 8);

byte addresses[][6] = {"0"};

//enum XDIRECTION {LEFT,RIGHT,CENTER};
//enum YDIRECTION {FORWARD,BACKWARD,NONE};

struct package{
  enum XDIRECTION {LEFT,RIGHT,CENTER};
  enum YDIRECTION {FORWARD,BACKWARD,NONE};
  int valX, valY;
  
  XDIRECTION x;
  YDIRECTION y;
};
int valX, valY;

int control[] = {0,1};

typedef struct package Package;
Package data;

void setup(){
  pinMode(control[0],INPUT);
  pinMode(control[1],INPUT);
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
  //  Serial.print("test");
  valX = analogRead(control[0]);
  valY = analogRead(control[1]);

  //read analog to determine direction
  
  if (valY > 530){
    data.y = package::YDIRECTION::BACKWARD;
    data.valY = map(valY,1023,515,255,0);
  }else if (valY < 480){
    data.y = package::YDIRECTION::FORWARD;
    data.valY = map(valY,1,415,255,0);
  }else{
    data.y = package::YDIRECTION::NONE;
    data.valY = 0;
  }

  if (valX > 525){
    data.x = package::XDIRECTION::RIGHT;
    data.valX = map(valX,505,1024,0,255);
  }else if (valX < 485){
    data.x = package::XDIRECTION::LEFT;
    data.valX = map(valX,1,505,255,0);
  }else{
    data.x = package::XDIRECTION::CENTER;
    data.valX = 0;
  } 
  data.valX = abs(data.valX);
  data.valY = abs(data.valY);
  Serial.print("endtest");
//  setDir();
}

void loop(){
  steer();
 myRadio.write(&data, sizeof(data)); 

//  Serial.print("\nPackage:");
 // Serial.println(data.valX);
//  Serial.print("\n");
//  delay(1000);

}
