#include <LiquidCrystal.h>

int trigpin = 9;
int echopin = 8;

long duration;
int distance;

LiquidCrystal lcd(7,6,5,4,3,2);

void setup(){

  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  Serial.begin(9600);
}

void loop(){
  digitalWrite(trigpin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(2);
  digitalWrite(trigpin,LOW);

  duration = pulseIn(echopin,HIGH);
  distance=duration*0.034/2;
  lcd.print("Dst:");
  lcd.print(distance);
  lcd.print("cm");
  delay(500);
  lcd.clear();
}

