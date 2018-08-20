#include <SPI.h>  
#include "RF24.h"
#include "nRF24L01.h"
#include <ArduCAM.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <Servo.h>
#include "memorysaver.h"

//This demo can only work on OV2640_MINI_2MP or OV5642_MINI_5MP or OV5642_MINI_5MP_BIT_ROTATION_FIXED platform.
#if !(defined OV5642_MINI_5MP || defined OV5642_MINI_5MP_BIT_ROTATION_FIXED || defined OV2640_MINI_2MP || defined OV3640_MINI_3MP)
  #error Please select the hardware platform and camera module in the ../libraries/ArduCAM/memorysaver.h file
#endif
#define SD_CS 2
const int SPI_CS = 9;
ArduCAM myCAM( OV2640, SPI_CS );
Servo tilt;


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
  //Serial.println(F("start Capture"));
  while(!myCAM.get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK));
  //Serial.println(F("Capture Done."));  
  length = myCAM.read_fifo_length();
  //Serial.print(F("The fifo length is :"));
  //Serial.println(length, DEC);
  if (length >= MAX_FIFO_SIZE) //384K
  {
    //Serial.println(F("Over size."));
    return ;
  }
  if (length == 0 ) //0 kb
  {
    //Serial.println(F("Size is 0."));
    return ;
  }
  //Construct a file name
  k = k + 1;
  itoa(k, str, 10);
  strcat(str, ".jpg");
  //Open the new file
  outFile = SD.open(str, O_WRITE | O_CREAT | O_TRUNC);
  if(!outFile){
    //Serial.println(F("File open faild"));
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
      //Serial.println(F("Image save OK."));
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

char blu = 0;

struct package {
  enum XDIRECTION {LEFT, RIGHT, CENTER};
  enum YDIRECTION {FORWARD, BACKWARD, NONE};

  XDIRECTION aX;
  YDIRECTION aY;

  XDIRECTION bX;
  YDIRECTION bY;
  int aValY, aValX, bValY, bValX;
  bool buttonA, buttonB, readGyro;
  byte packetTest = 0;
  int errorX, errorY;
};

typedef struct package Package;
Package data;

int motorA[] = {3, 4, 5}; //last is the enable
int motorB[] = {7, 8, 6};

byte favor, warn = 14, sd = 15, cam = 16;

void setup() {

 // tilt.attach(10);
 // tilt.write(76);

  pinMode(warn, OUTPUT);
  pinMode(sd, OUTPUT);
  pinMode(cam, OUTPUT);

  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(motorA[i], OUTPUT);
    pinMode(motorB[i], OUTPUT);
  }

  // pinMode(control[0],INPUT);
  // pinMode(control[1],INPUT);
//  pinMode(sign, OUTPUT);

  //default, forward.
  digitalWrite(motorA[0], LOW);
  digitalWrite(motorB[0], LOW);
  digitalWrite(motorA[1], LOW);
  digitalWrite(motorB[1], LOW);
  // digitalWrite(motorA[2],0);
  // digitalWrite(motorB[2],0);

  analogWrite(motorA[2], 122);
  analogWrite(motorB[2], 255);

 // /*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  uint8_t vid, pid;
  uint8_t temp;
  
  Wire.begin();
  
  //set the CS as an output:
  pinMode(SPI_CS,OUTPUT);
  // initialize SPI:
  SPI.begin();
  while(1){
    //Check if the ArduCAM SPI bus is OK
    myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
    temp = myCAM.read_reg(ARDUCHIP_TEST1);
    
    if (temp != 0x55){
      digitalWrite(warn,HIGH);
    //  //Serial.println(F("SPI interface Error!"));
      delay(1000);continue;
    }else{
      digitalWrite(warn,LOW);
      digitalWrite(cam,HIGH);
    //  //Serial.println(F("SPI interface OK."));
     // digitalWrite(camSign,HIGH);
      break;
        
    }
  }
  //Initialize SD Card
  delay(1000);
  
  while(!SD.begin(SD_CS)){
    digitalWrite(warn,HIGH);
   // //Serial.println(F("SD Card Error!"));
    delay(1000);
  }
  digitalWrite(warn,LOW);
  digitalWrite(sd,HIGH);
  ////Serial.println(F("SD Card detected."));

  delay(1000);
  

  while(1){
    //Check if the camera module type is OV2640
    myCAM.wrSensorReg8_8(0xff, 0x01);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
    myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);
    if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))){
      ////Serial.println(F("Can't find OV2640 module!"));
      delay(1000);continue;
    }
    else{
      ////Serial.println(F("OV2640 detected."));
      break;
    } 
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  digitalWrite(warn,LOW);
  digitalWrite(cam,LOW);
  digitalWrite(sd,LOW);

  myCAM.set_format(JPEG);
  myCAM.InitCAM();
  myCAM.OV2640_set_JPEG_size(OV2640_320x240);
  delay(1000);

  //test photo
  digitalWrite(cam, HIGH);
  myCAMSaveToSDFile();
  delay(5000);
  digitalWrite(cam,LOW);
  
  for(byte i = 0; i<3;i++){
    delay(500);
    digitalWrite(warn,HIGH);
    digitalWrite(cam,HIGH);
    digitalWrite(sd,HIGH);  
    delay(500);
    digitalWrite(warn,LOW);
    digitalWrite(cam,LOW);
    digitalWrite(sd,LOW);  
  }
}

void setSpd() {
  int valX = data.aValX, valY = data.aValY;
  //valX = valY = 100;
  if (data.aX == package::XDIRECTION::LEFT) {
    //  //Serial.println("LEFT");
    //   digitalWrite(motorA[2],205);//(int)(valY*favor));//Left Wheel
    //   digitalWrite(motorB[2],0);
  } else if (data.aX == package::XDIRECTION::RIGHT) {
    //   //Serial.println("RIGHT");
    //   digitalWrite(motorA[2],0);
    //  digitalWrite(motorB[2],250);
  } else {
    //   //Serial.println("not left or right");
    //    digitalWrite(motorA[2],250);
    //   digitalWrite(motorB[2],250);
  }
}

void spin() {
  int valX = data.aValX;
  digitalWrite(motorA[0], LOW);
  digitalWrite(motorB[0], LOW);
  digitalWrite(motorA[1], HIGH);
  digitalWrite(motorB[1], HIGH);
  if (data.aX == package::XDIRECTION::LEFT) {
    digitalWrite(motorA[2], 0);
    digitalWrite(motorB[2], valX);
  } else if (data.aX == package::XDIRECTION::RIGHT) {
    digitalWrite(motorA[2], valX);
    digitalWrite(motorB[2], 0);
  } else {
    digitalWrite(motorA[0], LOW);
    digitalWrite(motorB[0], LOW);
    digitalWrite(motorA[1], LOW);
    digitalWrite(motorB[1], LOW);
    //  analogWrite(motorA[1],0);
    //  analogWrite(motorA[2],0);
  }
}

void setDir() {
  if (data.aY == package::YDIRECTION::FORWARD) {
    digitalWrite(motorA[0], LOW);
    digitalWrite(motorB[0], LOW);
    digitalWrite(motorA[1], HIGH);
    digitalWrite(motorB[1], HIGH);
    //   //Serial.print("wheels forawrd");
    //    //Serial.print(", favors ");
    //   //Serial.println(X[data.x]);
    //   setSpd();
  } else if (data.aY == package::YDIRECTION::BACKWARD) {
    digitalWrite(motorA[0], HIGH);
    digitalWrite(motorB[0], HIGH);
    digitalWrite(motorA[1], LOW);
    digitalWrite(motorB[1], LOW);
    //  //Serial.print("wheels backward");
    //   //Serial.print(", favors ");
    //  //Serial.println(X[data.x]);
    //  setSpd();
  } else {
    digitalWrite(motorA[0], LOW);
    digitalWrite(motorB[0], LOW);
    digitalWrite(motorA[1], LOW);
    digitalWrite(motorB[1], LOW);
    // //Serial.println("No Y movement, ");
    //  data.aValY = 0;
    //   spin();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) // Send data only when you receive data:
  {

    blu = Serial.read();      //Read the incoming data and store it into variable data
    //    //Serial.print(data);        //Print Value inside data in Serial monitor
    //   //Serial.print("\n");        //New line

    switch (blu) {
      //On Buttons for Directional
      case 1:
     //   digitalWrite(sign, HIGH);
        data.aY = package::YDIRECTION::FORWARD;
        break;
      case 2:
    //    digitalWrite(sign, HIGH);
        data.aY = package::YDIRECTION::BACKWARD;
        break;
      case 3:
    //    digitalWrite(sign, HIGH);
        data.aX = package::XDIRECTION::LEFT;
        break;
      case 4:
  //      digitalWrite(sign, HIGH);
        data.aX = package::XDIRECTION::RIGHT;
        break;

      //Buttons Alpha Beta
      case 5:
        data.buttonA = true;
        digitalWrite(cam, HIGH);
        myCAMSaveToSDFile();
        delay(5000);
        digitalWrite(cam,LOW);
        break;
      case 6:
   //     digitalWrite(sign, HIGH);
        data.buttonB = true;
        break;



      //Off Buttons Alpha Beta
      case (122):
        data.buttonA = false;
  //      digitalWrite(sign, LOW);
        break;
      case (121):
    //    digitalWrite(sign, LOW);
        data.buttonB = false;
        break;

      //Off Buttons for Directional
      case (124):
     //   digitalWrite(sign, LOW);
        data.aX = package::XDIRECTION::CENTER;
        break;
      case (123):
     //   digitalWrite(sign, LOW);
        data.aX = package::XDIRECTION::CENTER;
        break;
      case (126):
       // digitalWrite(sign, LOW);
        data.aY = package::YDIRECTION::NONE;
        break;
      case (125):
      //  digitalWrite(sign, LOW);
        data.aY = package::YDIRECTION::NONE;
        break;
    }
    setDir();
  }
}
