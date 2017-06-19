int pinA = 7;
int pinB = 6;
int pinC = 5;
int pinD = 4;
int pins[] = {pinA,pinB,pinC,pinD};
int pinsOpp[] = {pinC,pinD,pinA,pinB};
int motorDelay = 5;
int AnalogX = A1;

void setup(){
  pinMode(pinA,OUTPUT);
  pinMode(pinB,OUTPUT);
  pinMode(pinC,OUTPUT);
  pinMode(pinD,OUTPUT);
}
void loop(){
  motorDelay = analogRead(AnalogX);
  //singleExcite(1);
  twoStep(1);
}

void twoStep(int dTime){
  int current = 0;
  int next = 0;
  for (int i = 0; i<4;i++){
    current = i;
    if (current == 3){
      next = 0;
    }else{
      next = current+1;
    }
    digitalWrite(pins[current],HIGH);
    delay(dTime);
    digitalWrite(pins[next],HIGH);
    delay(dTime);
    digitalWrite(pins[current],LOW);
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

