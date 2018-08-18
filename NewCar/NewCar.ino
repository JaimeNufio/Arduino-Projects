int pwm[] = {5,6};
int left[] = {10,11};
int right[] = {8,9};
int front = 7;
int relay = 4;
//int spd;

void power(int spd){
  for (int i = 0; i<2; i++){
    analogWrite(pwm[i],spd);
  }
}
void forward(){
//  for (int i = 0; i<1; i++){
    digitalWrite(left[0],HIGH);
    digitalWrite(right[0],HIGH);
//  }
}
void setup() {
  /*
  //pin sets are in pairs
  for (int i = 0; i<2; i++){
    pinMode(pwm[i],OUTPUT);
    pinMode(left[i],OUTPUT);
    pinMode(right[i],OUTPUT);
  }
  pinMode(front,INPUT);
  pinMode(relay,OUTPUT);

 */
  ///power(255);
 // forward();
}


void loop() {
   digitalWrite(relay,HIGH);

 pinMode(6,OUTPUT);
 pinMode(13,OUTPUT);
 pinMode(12,OUTPUT);
 pinMode(relay,OUTPUT);
 delay(1000);
 analogWrite(6,205);
 digitalWrite(13,HIGH);
 digitalWrite(12,LOW);
    
}
