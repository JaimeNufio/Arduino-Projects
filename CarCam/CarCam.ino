#include <SPI.h>  
#include "RF24.h"
#include "nRF24L01.h"
#include <ArduCAM.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include "memorysaver.h"
//This demo can only work on OV2640_MINI_2MP or OV5642_MINI_5MP or OV5642_MINI_5MP_BIT_ROTATION_FIXED platform.
#if !(defined OV5642_MINI_5MP || defined OV5642_MINI_5MP_BIT_ROTATION_FIXED || defined OV2640_MINI_2MP || defined OV3640_MINI_3MP)
  #error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif
#define SD_CS 14
const int SPI_CS = 15, camSign = 16;
ArduCAM myCAM( OV2640, SPI_CS );
RF24 myRadio (3,4);

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

//x 0,505,1022
//y 0,515,1023

void myCAMSaveToSDFile(){
  char str[8];
  byte buf[256];
  static int i = 0;
  static int k = 0;
  uint8_t temp = 0,temp_last=0;
  uint32_t length = 0;
  bool is_header = false;
  File outFile;
  //Flush the FIFO
  myCAM.flush_fifo();
  //Clear the capture done flag
  myCAM.clear_fifo_flag();
  //Start capture
  myCAM.start_capture();
  Serial.println(F("start Capture"));
  while(!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK));
  Serial.println(F("Capture Done."));  
  length = myCAM.read_fifo_length();
  Serial.print(F("The fifo length is :"));
  Serial.println(length, DEC);
  if (length >= MAX_FIFO_SIZE) //384K
  {
    Serial.println(F("Over size."));
    return ;
  }
  if (length == 0 ) //0 kb
  {
    Serial.println(F("Size is 0."));
    return ;
  }
  //Construct a file name
  k = k + 1;
  itoa(k, str, 10);
  strcat(str, ".jpg");
  //Open the new file
  outFile = SD.open(str, O_WRITE | O_CREAT | O_TRUNC);
  if(!outFile){
    Serial.println(F("File open faild"));
    return;
  }
  myCAM.CS_LOW();
  myCAM.set_fifo_burst();
  while ( length-- )
  {
    temp_last = temp;
    temp =  SPI.transfer(0x00);
    //Read JPEG data from FIFO
    if ( (temp == 0xD9) && (temp_last == 0xFF) ) //If find the end ,break while,
    {
      buf[i++] = temp;  //save the last  0XD9     
      //Write the remain bytes in the buffer
      myCAM.CS_HIGH();
      outFile.write(buf, i);    
      //Close the file
      outFile.close();
      Serial.println(F("Image save OK."));
      is_header = false;
      i = 0;
    }  
    if (is_header == true)
    { 
      //Write image data to buffer if not full
      if (i < 256)
      buf[i++] = temp;
      else
      {
        //Write 256 bytes image data to file
        myCAM.CS_HIGH();
        outFile.write(buf, 256);
        i = 0;
        buf[i++] = temp;
        myCAM.CS_LOW();
        myCAM.set_fifo_burst();
      }        
    }
    else if ((temp == 0xD8) & (temp_last == 0xFF))
    {
      is_header = true;
      buf[i++] = temp_last;
      buf[i++] = temp;   
    } 
  } 
}

void setup() {
  
  Serial.begin(115200);
  pinMode(10,OUTPUT);
  pinMode(10,HIGH);
  for (int i = 0; i<3; i++){
    pinMode(motorA[i],OUTPUT);
    pinMode(motorB[i],OUTPUT);
  }
  
//  pinMode(control[0],INPUT); 
 // pinMode(control[1],INPUT);
  pinMode(sign,OUTPUT);
   pinMode(camSign,OUTPUT);

  //default, forward.
  digitalWrite(motorA[0],HIGH);
  digitalWrite(motorB[0],HIGH);
  digitalWrite(motorA[1],LOW);
  digitalWrite(motorB[1],LOW);
  digitalWrite(motorA[2],0);
  digitalWrite(motorB[2],0);
 // pinMode(10,OUTPUT);
  digitalWrite(sign,LOW);
  digitalWrite(camSign,LOW);
  Serial.begin(115200);
 // /*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t vid, pid;
uint8_t temp;
Wire.begin();
Serial.begin(115200);
Serial.println(F("ArduCAM Start!"));
//set the CS as an output:
pinMode(SPI_CS,OUTPUT);
// initialize SPI:
SPI.begin();
while(1){
  //Check if the ArduCAM SPI bus is OK
  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  temp = myCAM.read_reg(ARDUCHIP_TEST1);
  
  if (temp != 0x55){
    Serial.println(F("SPI interface Error!"));
    delay(1000);continue;
  }else{
    Serial.println(F("SPI interface OK."));digitalWrite(camSign,HIGH);break;
      
  }
}
//Initialize SD Card

while(!SD.begin(SD_CS)){
  Serial.println(F("SD Card Error!"));delay(1000);
}
Serial.println(F("SD Card detected."));

#if defined (OV2640_MINI_2MP)
  while(1){
    //Check if the camera module type is OV2640
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
      Serial.println(F("Can't find OV2640 module!"));
      delay(1000);continue;
    }
    else{
      Serial.println(F("OV2640 detected."));break;
    } 
  }
#elif defined (OV3640_MINI_3MP)
  while(1){
    //Check if the camera module type is OV3640
    myCAM.rdSensorReg16_8(OV3640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg16_8(OV3640_CHIPID_LOW, &pid);
    if ((vid != 0x36) || (pid != 0x4C)){
      Serial.println(F("Can't find OV3640 module!"));
      delay(1000);continue; 
    }else{
      Serial.println(F("OV3640 detected."));break;    
    }
 } 
#else
  while(1){
    //Check if the camera module type is OV5642
    myCAM.wrSensorReg16_8(0xff, 0x01);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg16_8(OV5642_CHIPID_LOW, &pid);
    if((vid != 0x56) || (pid != 0x42)){
      Serial.println(F("Can't find OV5642 module!"));
      delay(1000);continue;
    }
    else{
      Serial.println(F("OV5642 detected."));break;
    } 
  }
#endif
myCAM.set_format(JPEG);
myCAM.InitCAM();
#if defined (OV2640_MINI_2MP)
  myCAM.OV2640_set_JPEG_size(OV2640_320x240);
#elif defined (OV3640_MINI_3MP)
  myCAM.OV3640_set_JPEG_size(OV3640_320x240);
#else
  myCAM.write_reg(ARDUCHIP_TIM, VSYNC_LEVEL_MASK);   //VSYNC is active HIGH
  myCAM.OV5642_set_JPEG_size(OV5642_320x240);
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //*/
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX); //switch MIN to MAX, HIgh or LOW
  myRadio.setDataRate(RF24_250KBPS);//( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  myRadio.printDetails();
  Serial.println("Radio should be set");
 // delay(1000);
  digitalWrite(sign,HIGH);
  digitalWrite(camSign,HIGH);
  delay(500);
  digitalWrite(sign,LOW);
  digitalWrite(camSign,LOW);
  delay(500);
  digitalWrite(sign,HIGH);
  digitalWrite(camSign,HIGH);
  delay(500);
  digitalWrite(sign,LOW);
  digitalWrite(camSign,LOW);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  pinMode(14,OUTPUT);
}

void setSpd(){
  
  int valX = data.aValX, valY = data.aValY;
  favor = 150;//((float)valX)/(500*2); //255
  valY=favor;
 // if (data.aX == package::XDIRECTION::LEFT){
 // Serial.println("LEFT");
    analogWrite(motorA[2],(int)(valY));//Left Wheel
    analogWrite(motorB[2],(int)valY);
 // }else if (data.aX == package::XDIRECTION::RIGHT){
 //  Serial.println("RIGHT");
    analogWrite(motorA[2],(int)valY);
    analogWrite(motorB[2],(int)(valY));
//  }else{
  //  Serial.println("not left or right");
    analogWrite(motorA[2],favor);
    analogWrite(motorB[2],favor);
  //}
}

void spin(){
    int valX = 150;//data.aValX;
    Serial.println("spin!");
  //  Serial.print("spin-");
  if (data.aX == package::XDIRECTION::LEFT){
    digitalWrite(motorA[0],HIGH);
    digitalWrite(motorB[0],LOW);
    digitalWrite(motorA[1],LOW);
    digitalWrite(motorB[1],HIGH);
    analogWrite(motorA[2],valX);
    analogWrite(motorB[2],valX);
  //  Serial.println("left");
  }else if (data.aX == package::XDIRECTION::RIGHT){
    analogWrite(motorA[2],valX);
    analogWrite(motorB[2],valX);
    digitalWrite(motorA[0],LOW);
    digitalWrite(motorB[0],HIGH);
    digitalWrite(motorA[1],HIGH);
    digitalWrite(motorB[1],LOW);
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
  Serial.println(data.aValY);
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
//  }else if (data.aX == package::XDIRECTION::LEFT || data.aX == package::XDIRECTION::RIGHT){
   // Serial.println("No Y movement, ");
   // data.aValY = 0; 
   // spin();
  }else if (data.aY == package::YDIRECTION::NONE){
    Serial.println("No input");
    analogWrite(motorA[1],0); 
    analogWrite(motorA[2],0);   
        digitalWrite(motorA[0],LOW);
    digitalWrite(motorB[0],LOW);
    digitalWrite(motorA[1],LOW);
    digitalWrite(motorB[1],LOW);
  }
  if (data.aX != package::XDIRECTION::CENTER){
    data.aValY = 0;
    spin();
  }

}

void loop() {

  delay(5);
  if (myRadio.available()) {
   //  digitalWrite(14,HIGH);
     Serial.println("data");
     if (data.packetTest != 0){
      digitalWrite(sign,HIGH);
      Serial.println(data.packetTest);
     }
   //  bool done = false;
      while (myRadio.available()){
            digitalWrite(camSign,LOW);
        //setDir();
        myRadio.read( &data, sizeof(data) );
        //delay(100);
              
      Serial.println(data.buttonB);
      if (data.buttonB==true){
        Serial.println("SHOOT");
        digitalWrite(camSign,HIGH);
        myCAMSaveToSDFile();
       delay(5000);
        digitalWrite(camSign,LOW);
      }

      // /*
       Serial.print("Ax:");
       Serial.print(X[(&data)->aX]);
       Serial.print(" | Ay:");
       Serial.print(Y[(&data)->aY]);
       Serial.print(" | Bx:");
       Serial.print(X[(&data)->bX]);
       Serial.print(" | By:");
       Serial.print(Y[(&data)->bY]);
       // */
      }
        setDir();
  }else{
    //digitalWrite(14,LOW);
    digitalWrite(camSign,HIGH);
    digitalWrite(sign, LOW);
    Serial.println(":(");
        Serial.println("No input");
 
  }

  /*
  Serial.print("X: ");
  Serial.print(X[x]);
  Serial.print(" ");
  Serial.print(valX);
  Serial.print(" Y: ");
 // Serial.print(Y[y]);
  Serial.print(" ");
  Serial.print(valY);  
  Serial.print(" with modifier ");
  Serial.println(favor);
  delay(200);
  */
}
