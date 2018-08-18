byte a = 11,b=9,c=10;
void setup() {
  // put your setup code here, to run once:
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i<255;i++){
    digitalWrite(a,i);
    for (int j = 0; j<255;j++){
      digitalWrite(b,j);  
      for (int k = 0; k<255;k++){
        digitalWrite(c,k);
        delay(200);
      }
    }
  }
}
