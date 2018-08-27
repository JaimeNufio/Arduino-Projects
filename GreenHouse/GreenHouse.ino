#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

byte blu,pCount,sCount,mCount;
unsigned long long int hCount;
int water=24,lights=8, waterActual,lightsActual,waterTimer, lightTimer, altLights=0,altWater;

bool pause = false,lightMode = false;


LiquidCrystal_I2C lcd(0x3F,16,2);

void setup() {
  // put your setup code here, to run once:
  pinMode(7,OUTPUT);
  Serial.begin(9600);
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Started");
  delay(1000);
  lcd.clear();
  pinMode(13,OUTPUT);
  updateNumbers();
}

void updateText(bool saving){
  bool oops = false;
  String err;
  if (lights >=23 || lights <=0){
    if (lights <= 0){
      lights = 0; 
    }else{
      lights = 22;
    }
    oops = true;
    err="L Mx/Mn 22/1";
  }
  if (water >=71 || water <=0){
    oops = true;
    if (water <= 0){
      water = 1;
    }else{
      water = 70;
    }
    err="W Mx/Mn 70/1";
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("W: ");
  lcd.print(water);
  lcd.print("Hr");
  lcd.print("  L: ");
  lcd.print(lights);
  lcd.print("Hr");

  if(oops){
    lcd.setCursor(0,1);
    lcd.print(err);
  }

  if (saving){
    lcd.setCursor(0,1);
    lcd.print("Saved and Reset");
    digitalWrite(13,HIGH);

  }
  
}

void switchLight(){
  lightTimer = 0;
  lightMode=!lightMode;
  digitalWrite(13,!digitalRead(13));
}

void updateNumbers(){
  waterActual = water; lightsActual = lights;
  lightTimer = 0; waterTimer = 0;
  altLights = 24-lights;
  altWater = 73-water;
  updateText(true);
}

void loop() {

  if (Serial.available() > 0 ) {
    
    blu = Serial.read(); 

    switch(blu){
      case 1:

        if (!pause){
          pause=true;
          water++;
          updateText(false);
        }
        break;
      case 2:

      if (!pause){
        pause=true;
        water--;
        updateText(false);
      }
      break;
      case 3:

        if (!pause){
          pause=true;
          lights++;
          updateText(false);
        }
        break;
      case 4:

      if (!pause){
        pause=true;
        lights--;
        updateText(false);
      }
      break;
      case 5:
      //"save" changes
        updateNumbers();
        
      break;
      case 123:
      case 124:
      case 125:
      case 126:
        pause = false;
        break;
    }
  }else{
    //digitalWrite(13,LOW);
  }

  delay(50);
  pCount++;
  if (pCount >= 1000/50){
 
    pCount=0;
    sCount++;
    lightTimer++;
    waterTimer++;
  }
  if (sCount >= 60){
       
    sCount = 0;
    mCount++;
    
  }
  if(mCount >= 60){
    mCount=0;
    hCount++;
  }

  //Normal Opperations
  //Trigger on Hours Reached
  //Trigger on Opp Hours Reached
  if ((!lightMode && lightTimer >= lightsActual) ||(lightMode && lightTimer >= altLights)){
    Serial.println(lightMode);
    switchLight();
  }

  if ((!lightMode && lightTimer > lights) ||(lightMode && lightTimer > altLights)){
    //switchLight();
  }
  
}
