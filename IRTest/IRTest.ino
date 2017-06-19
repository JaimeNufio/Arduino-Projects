#include <Servo.h>
#include <IRremote.h>

Servo servo;
int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  servo.attach(10);
  //servo.write(0);
  //delay(1000);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

}

void loop() {
 
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
    if (results.value == 16724175){
      Serial.println("one.");
      servo.write(1);
      delay(1000);
    }
  }
} 
