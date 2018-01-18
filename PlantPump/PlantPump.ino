//Sample Code
/*
  # Example code for the moisture sensor
  # Connect the sensor to the A0 (Analog 0) pin on the Arduino board
  # the sensor value description
  # 0 ~300 dry soil
  # 300~700 humid soil
  # 700~950 in water
*/

int up = 50; //update time
int secInDay = 10 * 60 * 24;
int indicator = 3;
float avg = 0;
int cnt = 30;//sseconds between prints
int wait = 500;
int counter = 0;
int longCounter = 0;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight
  lcd.setCursor(0, 0);

  pinMode(indicator, OUTPUT);

  Serial.begin(57600);

}

void loop() {
  //Serial.print("Moisture Sensor Value:");
  lcd.setCursor(0, 0);
  //Serial.println(analogRead(3));
  lcd.print((( 1 - ( (float) analogRead(3) / 1023) )) * 100);
  lcd.print("% || ");
  // lcd.setCursor(0,1);
  //Serial.println(analogRead(0));
  lcd.print((( 1 - ( (float) analogRead(2) / 1023) )) * 100);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print((( 1 - ( (float) analogRead(1) / 1023) )) * 100);
  lcd.print("% || ");
  avg = ((analogRead(3) + analogRead(2) + analogRead(1)) / (3)) ;

  //Serial.println(1-(avg/1023));
  counter += wait;
  if (counter > cnt * 1000) {
    counter = 0;
    longCounter += cnt;
    Serial.print(longCounter);
    Serial.print(", ");
    Serial.print((1 - (avg / 1023)) * 100, 3);
    Serial.println("%");
  }

  lcd.print(100 * (1 - (avg / 1023)));
  lcd.print("%");

  /*
    if (analogRead(indicator)>600){
    digitalWrite(indicator,HIGH);
    }else{
    digitalWrite(indicator,LOW);
    }
  */

  delay(up);
}
