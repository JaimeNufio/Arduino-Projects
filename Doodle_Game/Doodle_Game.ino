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
enum dir {LEFT,RIGHT,UP,DOWN,STILL};
dir curr = RIGHT;
String lookup[] = {"LEFT","RIGHT","UP","DOWN"};
//starts at 16 in the y
int pos[] = {20,6};
int base[] ={4,20};
int target[] = {20,15};
byte positions[100][2];
int snakeSize = 1; 
int score = 0, highscore = 5;
bool grow = false, STOP = false;

void play(){
  STOP = true;
  pinMode(6,OUTPUT);
  for (int i = 0; i < 255; i+=20){
    digitalWrite(6,i);
    delay(5);
    digitalWrite(6,0);
    delay(1000);
  }
}

void setup() {
  
  //play();
  grow = false;
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  
  snakeSize = 10;
  score = 0;
  curr = LEFT;
  goal();
  for (int i = 0; i<snakeSize; i++){
    positions[i][0] = pos[0]+i;
    positions[i][1] = pos[1];
    point((int)positions[i][0],(int)positions[i][1]);
    Serial.print("X: ");
    Serial.print(positions[i][0]);
    Serial.print(" Y: ");
    Serial.println(positions[i][1]);
  }
  Serial.print("Snake is size ");
  Serial.println(snakeSize);
  
  heading();
  // initialize and clear display
  display.display();
  pinMode(7,INPUT);
  digitalWrite(7,HIGH);

}

void point(int a[]){
  point(a[0],a[1]);
}

void point(int a, int b){
  //display.drawPixel(pos[0],pos[1], WHITE);
  a = (a*5)+base[0];
  b = (b*5)+base[1];
 // int cnt = 0;
  for (int x = -2; x < 3; x++){
    for (int y = -2; y <3; y++){
      //cnt=cnt+1;
      if (!(x == 0 && y== x)){
        display.drawPixel(a+x,b+y,WHITE);
      }
    }
  }
}

bool placementCheck(){
  Serial.println("placementCheck");
  for (int i = 0; i<snakeSize; i++){     
    if (positions[i][0] == target[0] && positions[i][1] == target[1]){
      return true;
    }
  }
  return false;
}

void goal(){
  do{
    target[0] = random(2, 22);
    target[1] = random(2,8);
  }while(placementCheck());
  Serial.println("Moved target");
}
//check for collision
void check(){
  if (target[0] == positions[0][0] && target[1] == positions[0][1] && !grow){
    score++;
    grow = true;  
    goal();
  }
      
  for (int i = 4; i<snakeSize; i++){     
    if (positions[i][0] == positions[1][0] && positions[i][1] == positions[1][1]){
      Serial.print("Collide at ");
      Serial.println(i);
        STOP = true;   
        curr = STILL;
        gameover();        
    }
  }      
}

void box(){
  for (int i = 0; i<127;i++){
    display.drawPixel(i,0,WHITE);
    display.drawPixel(i,63,WHITE);
    display.drawPixel(0,i,WHITE);
    display.drawPixel(127,i,WHITE);
    display.drawPixel(i,16,WHITE);
  }
}

void heading(){ 
  box();
  display.setCursor(2,2);
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.print(" Snake      Score:");
  display.print(score); 
  
  for (int i = 0; i<128;i++){
    display.drawPixel(i,12,WHITE);
    display.drawPixel(i,15,WHITE);
  }
  if (!grow)
    point(target);
 // display.display();
}

void gameover(){
  if (score > highscore){
    highscore = score;
  }
  display.clearDisplay();
  box();
  for (int i = 0; i<128;i++){
    display.drawPixel(i,12,WHITE);
    display.drawPixel(i,15,WHITE);
  }
  display.setCursor(2,2);
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.print(" H.Score:");
  display.print(highscore);
  display.print("  Score:");
  display.print(score);  
  display.setCursor(37,30);
  display.print("GAME OVER");
  display.setCursor(32,40);
  display.print("Play again?");
  display.display(); 
}

void loop() {
    dir xdir,ydir;
    valx = analogRead(x);
    valy = analogRead(y);
    if(valx > 550 && curr != LEFT){
      curr = RIGHT;
    }else if (valx < 450 && curr != RIGHT){
      curr = LEFT;
    }else if(valy > 550 && curr != UP){
      curr = DOWN;
    }else if (valy < 450 && curr != DOWN){
       curr = UP;
    }
  
    switch(curr){
      case LEFT:
        pos[0]--;
        Serial.println(lookup[curr]);
        break;
      case RIGHT:
        pos[0]++;
        Serial.println(lookup[curr]);
        break;
      case UP:
        pos[1]--;
        Serial.println(lookup[curr]);
        break;
      case DOWN:
        pos[1]++;
        Serial.println(lookup[curr]);
        break;   
      default:
        break;   
    }
    
  if (!STOP){
    if (pos[1] < 0){
      pos[1] = 8;
    }
    if (pos[1] > 8){
      pos[1] = 0;
    }
    if (pos[0] < 0){
      pos[0] = 24;
    }
    if (pos[0] > 24){
      pos[0] = 0;
    }
  
    if(grow){
      Serial.println("growing");
      positions[snakeSize+1][0] = positions[snakeSize][0];
      positions[snakeSize+1][1] = positions[snakeSize][1];
      snakeSize++;
      grow = false;
    }
  
  
    display.clearDisplay();
    for (int i = snakeSize-1; i>0; i--){
      positions[i][0] = positions[i-1][0];
      positions[i][1] = positions[i-1][1];
      /* 
      Serial.print(i);
      Serial.print(": ");
      Serial.print(positions[i][0]);
      Serial.print(",");
      Serial.println(positions[i][1]);
      */
      point((int)positions[i][0],(int)positions[i][1]);
    }
    
    positions[0][0] = pos[0];
    positions[0][1] = pos[1];
    point(positions[0][0],positions[0][1]);
  
    check(); 
    if (!STOP)
      heading();
    display.display();  
  }
  if (STOP && curr != STILL){
    STOP = false;
    setup();
  }

  delay(100);
}
