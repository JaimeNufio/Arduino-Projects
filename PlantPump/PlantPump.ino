//Sample Code
/*
 # Example code for the moisture sensor
 # Connect the sensor to the A0 (Analog 0) pin on the Arduino board
 # the sensor value description
 # 0 ~300 dry soil
 # 300~700 humid soil
 # 700~950 in water
*/

  
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2); 

void setup(){
 lcd.init();  //initialize the lcd
 lcd.backlight();  //open the backlight
 lcd.setCursor(0,0);
 Serial.begin(57600);
}

void loop(){
 Serial.print("Moisture Sensor Value:");
 Serial.println(analogRead(0));
 lcd.setCursor(0,0);
 lcd.print((float)analogRead(0)/1023);
 lcd.print("%");
 delay(100);
}
