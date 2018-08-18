
#include <IRremote.h>

int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
 
  //delay(1000);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver

}

void loop() {
 
  if (irrecv.decode(&results)) {
    Serial.println(results.value);
    irrecv.resume(); // Receive the next value
  }
  delay(100);
  Serial.println("loop");
} 
