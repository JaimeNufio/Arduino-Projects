
#define leftright
int cycle = 5,cnt = 0;

byte vin []= {12,13,14,15,16,17,18,19,0,1}, //COLUMN
grounds[] = {2,3,4,5,6,7,8,9,10,11}; //ROW

void setup() {
  // put your setup code here, to run once:
  for (byte i = 0; i<sizeof(vin)/sizeof(byte); i++){
    pinMode(grounds[i],OUTPUT);
    digitalWrite(grounds[i],HIGH);
    pinMode(vin[i],OUTPUT);
    digitalWrite(vin[i],LOW);
  }
 // Serial.begin(115200);
}

///*
uint16_t scale[11] = {
 0,0x1,0x03,0x7,0xF,0x1F,0x3F,0x7F,0xFF,0x1FF,0x3FF,  
};

uint16_t manaR[10] = {
  0x070,0x1BC,0x035,0x007,0x012,0x0F6,0x1EC,0x1FE,0x1F0,0x0F8,
};

uint16_t cat[10] = {
  0x303,
  0x2CD,
  0x279,
  0x186,
  0x34B,
  0x249,
  0x2B5,
  0x37B,
  0x186,
  0x0FC
  };

uint16_t cat1[10] = {
  0x000,
  0x303,
  0x2CD,
  0x2FF,
  0x34B,
  0x249,
  0x201,
  0x3B7,
  0x1FE,
  0x0FC
  };

uint16_t alien1[10] = {
  0x102,
  0x084,
  0x078,
  0x0FC,
  0x1FE,
  0x3B7,
  0x2FD,
  0x2FD,
  0x102,
  0x1CE,
};

uint16_t alien2[10] = {
  0x102,
  0x084,
  0x279,
  0x2FD,
  0x1FE,
  0x1B6,
  0x0FC,
  0x0FC,
  0x102,
  0x1CE,
};


uint16_t J[10] = {
  0x3FF,
  0x3FF,
  0x038,
  0x01C,
  0x00C,
  0x30C,
  0x30C,
  0x39C,
  0x1F8,
  0x0F0,
};

uint16_t Values[10] = {}, ValuesDisp[10] = {};

void draw(uint16_t buf[10]){
  //row
  for (byte i = 0; i<10;i++){
    //column
     
    for (byte j = 0; j<10;j++){
      uint16_t temp = buf[i];
      #ifdef leftright
      digitalWrite(vin[9-j],bitRead(temp,j));
      #endif
      #ifndef leftright
      digitalWrite(vin[j],bitRead(temp,j));
      #endif
    }
    digitalWrite(grounds[i],LOW);
    delay(1);
    //delay here
    digitalWrite(grounds[i],HIGH);
  }
}


void turnOn(byte x, byte y){
 // allOff();
  digitalWrite(vin[x],HIGH);      
  digitalWrite(grounds[y],LOW);
  delay(2);  
}

void allOff(){
   for (int i = 0; i<sizeof(vin)/sizeof(byte); i++){
    digitalWrite(grounds[i],HIGH);
    digitalWrite(vin[i],LOW);     
  }
}

void spiral(byte x, byte y){
  byte cnt = 0, offset = 0, endset = 0;
  int i = 0, j = 0;
  while (cnt < x*y){
    for (i = offset, j = offset; i<x-offset; ++i){
    //  Serial.print(i);
    //  Serial.print(",");
    // Serial.println(j);
      turnOn(i,j);
      cnt++;
    }
   // Serial.println("---");
    for(--i,++j; j<y-offset; ++j){
   //   Serial.print(i);
   //   Serial.print(",");
   //   Serial.println(j);
      turnOn(i,j);
      cnt++;
    }
   // Serial.println("---");
    for(--i,--j; i>offset-1; --i){
   //   Serial.print(i);
   //   Serial.print(",");
   //   Serial.println(j);
      turnOn(i,j);
      cnt++;
    }
   // Serial.println("---");
    for(++i,--j; j>offset; j--){
    //  Serial.print(i);
    //  Serial.print(",");
    //  Serial.println(j);
      turnOn(i,j);
      cnt++;
    }
    offset++;
  //  Serial.println("---loop---");
  }
  // Serial.println(cnt);
}

void add(){
  Values[0]++;
  for (int i = 0; i<9; i++){
    if (Values[i] > 10){
      Values[i+1]++;
      Values[i] = 1;
    }
    ValuesDisp[i] = scale[Values[i]];
  }
}

void loop(){
 draw(ValuesDisp);
 cnt++;
 if (cnt > 100){
    add();
  cnt = 0;
 }
}
