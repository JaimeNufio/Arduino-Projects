//Sample Code
/*
  # Example code for the moisture sensor
  # Connect the sensor to the A0 (Analog 0) pin on the Arduino board
  # the sensor value description
  # 0 ~300 dry soil
  # 300~700 humid soil
  # 700~950 in water
*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>


int indicator = 3;
float avg = 0, tempLight = 0;
int wait = 1000;
int secCounter = 0;
int minCounter = 0;
int hourCounter = 0;
int relay = 8;

int stat = 0;

File myFile;
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void water(){
  digitalWrite(relay,HIGH);
  delay(1000);
  digitalWrite(relay,LOW);
}

void setup() {
  water();
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight
  lcd.setCursor(0, 0);
  
  pinMode(relay,OUTPUT);

  pinMode(indicator, OUTPUT);

 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    //; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    stat = 0;
    return;
  }else{ 
    stat = 1;
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("Log.txt", FILE_WRITE);
  myFile.println("\nMoisture 1, Moisture 2, Moisture 3, Moisture Avg, Light Level");
  // if the file opened okay, write to it:
  if (myFile) {
    //Serial.print("Writing to test.txt...");
    //myFile.println("Boop.");
    // close the file:
    myFile.close();
    stat = 1;
    Serial.println("found SD Card.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
    stat=0;
  }
    
}

void printinfo(){
  myFile = SD.open("Log.txt", FILE_WRITE);

  //(1 - (avg / 1023)) * 100, 3
  //old formula

  myFile.print(analogRead(1));
  myFile.print(" , ");
  myFile.print(analogRead(2));
   myFile.print(" , ");
  myFile.print(analogRead(3));
    myFile.print(" , ");
  myFile.print((analogRead(1)+analogRead(2)+analogRead(3))/3);
    myFile.print(" , ");
  myFile.println(analogRead(0));
 
  Serial.println(analogRead(1));
  Serial.println(analogRead(2));
  Serial.println(analogRead(3));
  Serial.println((analogRead(1)+analogRead(2)+analogRead(3))/3);
  Serial.println(analogRead(0));
  myFile.close();
  
 
}

void loop() {
  
  lcd.setCursor(0, 0);

  avg = ((analogRead(3) + analogRead(2) + analogRead(1)) / (3)) ;


  //Serial.println(1-(avg/1023));
  tempLight = (analogRead(0));
  lcd.print("Moist: ");
  lcd.print(avg);
  //lcd.print(100 * (1 - (avg / 1023)));
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Light: ");
  lcd.print(tempLight);
  lcd.print(" ");
  lcd.print(stat);
  
  //===========================================
  
    printinfo();
    
  //===========================================


  secCounter++;
  if (secCounter >= 60){
    secCounter = 0;
    minCounter++;
  }
  if (minCounter >= 60){
    minCounter==0;
    hourCounter++;
    
  }
  
  if (avg > 420 && hourCounter > 6){
    water();
    secCounter+=1;
    hourCounter == 0;
  }
 
  
  delay(wait);
}



