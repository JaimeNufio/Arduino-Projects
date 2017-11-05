// ---------------------------------------------------------------------------
// This example code was used to successfully communicate with 15 ultrasonic sensors. You can adjust+
// the number of sensors in your project by changing SONAR_NUM and the number of NewPing objects in the
// "sonar" array. You also need to change the pins for each sensor for the NewPing objects. Each sensor
// is pinged at 33ms intervals. So, one cycle of all sensors takes 495ms (33 * 15 = 495ms). The results
// are sent to the "oneSensorCycle" function which currently just displays the distance data. Your project
// would normally process the sensor results in this function (for example, decide if a robot needs to
// turn and call the turn function). Keep in mind this example is event-driven. Your complete sketch needs
// to be written so there's no "delay" commands and the loop() cycles at faster than a 33ms rate. If other
// processes take longer than 33ms, you'll need to increase PING_INTERVAL so it doesn't get behind.
// ---------------------------------------------------------------------------
#include <NewPing.h>
#include <Servo.h>
#include <math.h>

Servo XAxis;  

#define SONAR_NUM     5 // Number or sensors.
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 50 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

unsigned long pingTimer[SONAR_NUM]; // Holds the times when the next ping should happen for each sensor.
unsigned int cm[SONAR_NUM];         // Where the ping distances are stored.
uint8_t currentSensor = 0;          // Keeps track of which sensor is active.

const int echo[] = {3,5,7,11,13}, trig[] = {2,4,6,10,12};
long dist, nearest;
int contendor;
int pos, stp = 5;

NewPing sonar[SONAR_NUM] = {     // Sensor object array.
  NewPing(trig[0], echo[0], MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(trig[1], echo[1], MAX_DISTANCE),
  NewPing(trig[2], echo[2], MAX_DISTANCE),
  NewPing(trig[3], echo[3], MAX_DISTANCE),
  NewPing(trig[4], echo[4], MAX_DISTANCE)
};

void setup() {
  Serial.begin(115200);
  XAxis.attach(8);
  pingTimer[0] = millis() + 75;           // First ping starts at 75ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++) // Set the starting time for each sensor.
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  pos = 90;
  XAxis.write(pos);
}

void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) { // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {         // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;  // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle(); // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();          // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                          // Sensor being accessed.
      cm[currentSensor] = 0;                      // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck); // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  
  }
  // The rest of your code would go here.
}

void echoCheck() { // If ping received, set the sensor distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void oneSensorCycle() { // Sensor ping cycle complete, do something with the results.
  nearest = MAX_DISTANCE;
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
    if (cm[i] < nearest && cm[i] > 3 && cm[i] != 0){
      nearest = cm[i];
      contendor = i;
    }
  }
  Serial.print(", Range of ");
  Serial.println(contendor);
 
  if (contendor == 1){
    
  }else if (contendor == 0 & pos < (180-(stp*1.5))){
    pos-=stp;
  }else{
    Serial.println("On Target");
  }
    Serial.println(pos);
    //XAxis.write(pos);
}
