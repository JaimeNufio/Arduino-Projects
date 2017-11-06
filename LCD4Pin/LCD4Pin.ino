//A5 to SCL
//A4 to SDA
  
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char words[] = "Happy Birthday";
char nicknames[4][16] = {"Jocelis :3      ","J-Dawg ;)       ","Silly :P        ", "Royalty :o      "};

LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup(){
  lcd.init();  //initialize the lcd
  lcd.backlight();  //open the backlight
  lcd.setCursor(0,0);
  lcd.print("Happy Birthday!!");
}

void erase(int line){
  lcd.setCursor(0,line);
  for (int i = 0; i<16; i++){
    lcd.print("  ");
  }
}

void text(int line, int digit,int del,char words[]){
  lcd.setCursor(digit,line);
  for (int i = 0; i<16-digit; i++){
    lcd.setCursor(digit+i,line);
    lcd.print(words[i]);
    delay(del*100);
  }
}

void loop(){
    
    for (int k = 0; k<3;k++){
       text(1,0,2,nicknames[k]);
       delay(200);
       erase(1);
       delay(200);
    }
    delay(200);
}


