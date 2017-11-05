int left[] = {7,6};
int right[] = {5,4};

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i<2; i++){
    pinMode(left[i],OUTPUT);
    pinMode(right[i],OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  digitalWrite(left[0],HIGH);
  digitalWrite(left[1],HIGH);
  digitalWrite(right[0],HIGH);
  digitalWrite(right[1],HIGH);
  delay(1000);
  */
  digitalWrite(left[0],HIGH);
  digitalWrite(left[1],LOW);
  digitalWrite(right[0],LOW);
  digitalWrite(right[1],LOW);
  delay(1000);

  /*
  digitalWrite(left[0],LOW);
  digitalWrite(left[1],HIGH);
  digitalWrite(right[0],LOW);
  digitalWrite(right[1],HIGH);
  delay(1000);

  
  digitalWrite(left[0],LOW);
  digitalWrite(left[1],LOW);
  digitalWrite(right[0],LOW);
  digitalWrite(right[1],LOW);
  delay(1000);
//  digitalWrite(right[1],HIGH);
*/
}
