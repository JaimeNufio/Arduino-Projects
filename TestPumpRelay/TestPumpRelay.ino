int relay = 7;
int val = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(relay,OUTPUT);
 // digitalWrite(relay,HIGH);
  Serial.begin(9600);
  delay(100);
 Serial.println("started");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);

  digitalWrite(relay,HIGH);
  delay(1000);
  val = analogRead(0);
  Serial.println(val);
  delay(1000);
  digitalWrite(relay,LOW);
}
