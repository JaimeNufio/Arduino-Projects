
const int trigPin[] = {7, 5, 3};
const int echoPin[] = {6, 4, 2};
const int light[] = {12,11,10};
long duration[3];

int distance;
int distanceMax;
int contendor;

void setup() {
  for (int i = 0; i < 3; i++){
    pinMode(trigPin[i], OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin[i], INPUT); // Sets the echoPin as an Input
    Serial.begin(9600); // Starts the serial communication
  }
}
void loop() {
  distanceMax = 100;
  contendor = -1;
  for (int i = 0; i < 3; i++) {
    // Clears the trigPin
      digitalWrite(light[i],LOW);
    digitalWrite(trigPin[i], LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin[i], LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration[i] = pulseIn(echoPin[i], HIGH);
    // Calculating the distance
    distance = duration[i] * 0.034 / 2;
     // Prints the distance on the Serial Monitor
    Serial.print(i);
    Serial.print(", Distance: ");
    Serial.println(distance);
    if(distance < distanceMax){
      distanceMax = distance;
      contendor = i;
    }
  }

  Serial.print(contendor);
  Serial.print(", dist: ");
  Serial.println(distanceMax);
  if (contendor != -1){
    digitalWrite(light[contendor],HIGH);
  }
  delay(45);
}


