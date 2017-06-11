#include <LiquidCrystal.h>

LiquidCrystal lcd(7,6,5,4,3,2);

void setup(){
  lcd.begin(16,2);
  //lcd.setCursor(3,1);
 // lcd.print("hello World!"); 
  String text = "Morning Joyce!";
  for (int i = 0; i < text.length(); i++){
    lcd.print(text[i]);
    delay(200);
  }
 
 
}

void loop(){
  int secs = 900;
  String text = "Morning Joyce!";
  String Dance = "> ^.^ >";
  String Dance1 = "< ^.^ <";
  lcd.setCursor(5,1);
  lcd.print(Dance);
  delay(secs);
  lcd.setCursor(0,0);
  lcd.print(text);
  lcd.setCursor(5,1);
  lcd.print(Dance1);
  delay(secs);
}

