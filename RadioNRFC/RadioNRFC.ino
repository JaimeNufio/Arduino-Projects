#include <SPI.h>  
#include "RF24.h"

RF24 myRadio (7, 8);

struct package{
  enum dir {LEFT,RIGHT,CENTER};
  dir xdir = LEFT, ydir = RIGHT;
};

byte addresses[][6] = {"0"}; 



typedef struct package Package;
Package data;
String table[] = {"Left","Right","Center"};
  
void setup() 
{
  Serial.begin(115200);
  delay(1000);

  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

void loop()  
{

  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
    }
    Serial.print("\nPackage:");
    Serial.print("\n");
    Serial.println(table[data.xdir]);
    Serial.println(table[data.ydir]);
  }

}
