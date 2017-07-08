//28byj-48

int pinA = 7;
int pinB = 6;
int pinC = 5;
int pinD = 4;
int pins[] = {pinA,pinB,pinC,pinD};
int pinsOpp[] = {pinD,pinC,pinB,pinA};
int motorDelay = 5;
int AnalogY = A0;
int AnalogX = A1;
int Button = 2;
int analogXVal = 0;

bool dir = true;
bool on = true;

void setup(){
  Serial.begin(9600);
  pinMode(pinA,OUTPUT);
  pinMode(pinB,OUTPUT);
  pinMode(pinC,OUTPUT);
  pinMode(pinD,OUTPUT);
  pinMode(Button,INPUT);
}
void loop(){
  analogXVal = analogRead(AnalogX);
  if (analogXVal > 550){
    dir = true;
  }else if (analogXVal < 400){
    dir = false;
  }

  Serial.println(analogXVal);

  //singleExcite(1);
  twoStep(1,dir,on);
}

void twoStep(int dTime, bool forward, bool on){
  if (on){
    int current = 0;
    int next = 0;
    for (int i = 0; i<4;i++){
      current = i;
      if (current == 3){
        next = 0;
      }else{
        next = current+1;
      }
      if (forward){
        digitalWrite(pins[current],HIGH);
        delay(dTime);
        digitalWrite(pins[next],HIGH);
        delay(dTime);
        digitalWrite(pins[current],LOW);
      }else{
        digitalWrite(pinsOpp[current],HIGH);
        delay(dTime);
        digitalWrite(pinsOpp[next],HIGH);
        delay(dTime);
        digitalWrite(pinsOpp[current],LOW);    
      }
    }
  }
}

void singleExcite(int dTime){
    for (int i = 0; i<4;i++){
    for (int j = 0; j<4; j++){
        digitalWrite(pins[j],LOW);
    }
    digitalWrite(pins[i], HIGH);
    delay(dTime);
  }
}

