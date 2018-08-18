#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// OLED display TWI address
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int x = 0,y = 1;
int valx,valy;
enum dir {LEFT,RIGHT,CENTER};
String types[] = {"LEFT","RIGHT","CENTER"};
//starts at 16 in the y
int pos[] = {62,32};

void setup() {
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();
  pinMode(7,INPUT);
  digitalWrite(7,HIGH);

  Serial.begin(9600);
  // display a pixel in each corner of the screen
/*
  display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 63, WHITE);
  display.drawPixel(127, 63, WHITE);

  // display a line of text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(27,30);
  display.print("Hello, world!");

  // update display with all of the above graphics
  display.display();
  
  display.setCursor(5,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Snake?");
  display.display();
  

    /*
  for (int i = 0 ; i<127; i++){
    for (int j = 16; j<64; j++){
      display.drawPixel(i,j,WHITE);
     if (j%1==0){
      display.display();
     }
    }
  }*/
    display.drawPixel(pos[0],pos[1], WHITE);
    heading();
    display.display();
}

void heading(){
  display.setCursor(32,0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Doodle V1.1");
  display.display();
}
void loop() {
  dir xdir,ydir;
  valx = analogRead(x);
  if(valx > 550){
    xdir = LEFT;
    pos[0]++;
  }else if (valx < 450){
    xdir = RIGHT;
    pos[0]--;
  }else{
    xdir = CENTER;
  }
  
  valy = analogRead(y);
  if(valy > 550){
    ydir = LEFT;
    pos[1]++;
  }else if (valy < 450){
    ydir = RIGHT;
    pos[1]--;
  }else{
    ydir = CENTER;
  }

  if (pos[1] < 17){
    pos[1] = 63;
  }
  if (pos[1] > 63){
    pos[1] = 17;
  }

  if (pos[0] < 0){
    pos[0] = 127;
  }
  if (pos[0] > 127){
    pos[0] = 0;
  }
  
  Serial.print("X: ");
  Serial.print(xdir);
  Serial.print(" Y: ");
  Serial.println(ydir);
  //x 0,505,1022
  //y 0,515,1023
  
  display.drawPixel(pos[0],pos[1], WHITE);
  display.drawPixel(pos[0],pos[1]+1, WHITE);
  display.drawPixel(pos[0],pos[1]-1, WHITE);
  display.drawPixel(pos[0]-1,pos[1], WHITE);
  display.drawPixel(pos[0]+1,pos[1], WHITE);
  display.display();

  if (digitalRead(7) == LOW){
    display.clearDisplay();
    heading();
  }
  //delay(100);
  
}

