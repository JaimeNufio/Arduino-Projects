// motor one
int enA = 12;
int in1 = 8;
int in2 = 9;
// motor two
int enB = 13;
int in3 = 10;
int in4 = 11;

int a=3;
int b=2;

int relay = 7;
int tempspd= 170;
boolean currDir = true;

void setup(){
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(relay, OUTPUT);
//  pinMode(s1, INPUT);
//  pinMode(s2, INPUT);
  digitalWrite(relay,HIGH);

  recto(true);
  Serial.begin(9600);
}

//in1,in2
//HIGH,LOW forward
//LOW,HIGH backward
//let true be forward

void turn(int t){
  changeDir();
  delay(t);
  spin();
  delay(t);
  recto(currDir);
}

void changeDir(){
  currDir=!currDir;
  recto(currDir);
}


void spin(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(100);
  if (digitalRead(a) == LOW || digitalRead(b) == LOW){
    spin();
  }
}

void recto(boolean dir){
  accel(0);
 // motorsOff();
  if (dir){
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

  }else{
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  accel(tempspd);
}
//let left be true
//enable 1 is the right wheel
void turn (boolean dir){
  if (dir){    
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }else{
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW); 
  }
}

//let true be accel, false decel
//let nspd be new speed

void accel(int nspd){
 /* if (nspd > spd){
    for (int i = spd; i < nspd; i++){
     analogWrite(enA, i);
     analogWrite(enB, i);
     delay(20);
    }
  }else{
    for (int i = spd; i > nspd; i--){
     analogWrite(enA, i);
     analogWrite(enB, i);
     delay(20);
    }
  }*/
  analogWrite(enA, nspd);
  analogWrite(enB, nspd);
}

void motorsOff(){
 digitalWrite(in1,LOW);
 digitalWrite(in2,LOW);
 digitalWrite(in3, LOW);
 digitalWrite(in4, LOW);
 analogWrite(enA, 0);
 analogWrite(enB, 0);
}

void check(int input, int output){
  if (digitalRead(input)){
    digitalWrite(output,HIGH);
    Serial.print(input);
  }else{
    digitalWrite(output,LOW);
  }
}

void loop(){
  
  delay(5);

  if (digitalRead(a) == LOW || digitalRead(b) == LOW){
    turn(500);
  }
  /*
  delay(10);
  if (digitalRead(a)==LOW){
    recto(true);
  }
  
  if (digitalRead(b)==LOW){
    recto(false);
  }*/
 // digitalWrite(relay,HIGH);
//  delay(1000);
//  digitalWrite(relay,LOW);
} 
