int AnalogX = A0;
int AnalogY = A1;
int sensorValueX = 0;
int sensorValueY = 0;

void setup(){
  Serial.begin(9600);

}

void loop() {
  sensorValueX = analogRead(AnalogX);
  sensorValueY = analogRead(AnalogY);
  Serial.print("X: ");
  if (sensorValueX < 1){
    Serial.print("Left Max");
  }else if (sensorValueX > 0 && sensorValueX < 320){
    Serial.print("Left Kinda");
  }else if (sensorValueX > 540){
    Serial.print("Right Max");
  }else if (sensorValueX > 355 && sensorValueX < 540){
    Serial.print("Right Kinda");
  }
  Serial.print("\nY: ");
  if (sensorValueY < 1){
    Serial.println("Down Max");
  }else if (sensorValueY > 0 && sensorValueY < 320){
    Serial.println("Down Kinda");
  }else if (sensorValueY > 540){
    Serial.println("Up Max");
  }else if (sensorValueY > 355 && sensorValueY < 540){
    Serial.println("Up Kinda");
  }
  Serial.println("");
  delay(500);
}
