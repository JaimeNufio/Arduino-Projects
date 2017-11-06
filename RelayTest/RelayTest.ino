int pin = 8;
int time = 2*1000;
void setup() {
  // put your setup code here, to run once:
  pinMode(pin,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pin,HIGH);
  delay(time);
  digitalWrite(pin,LOW);
  delay(time);
}
