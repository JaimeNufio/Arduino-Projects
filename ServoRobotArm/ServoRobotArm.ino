#include <PID_v1.h>
#include <Servo.h>

Servo claw,axis1,axis2,axis3,axis4;
double Setpoint, Input, Output;

int t= 15, x = 90, currval, fuck = 5;
Servo* axi[] = {&axis1,&axis2,&axis3,&axis4};

//2 - Neck,3 - waist,4 -legs,5 - spine.
byte pins[] = {2,3,4,5},
  val[] = {90,90,90,90},
  stp[] = {10,-10,-7,4},
  servos[] = {5,10,6,9};
enum dir  {FORWARD,BACKWARD,STILL};

bool mode = true,pressing = false;

void setup() {
  // put your setup code here, to run once:
  claw.attach(3); // claw is always 3.
  for (byte i = 0; i<4; i++){
    axi[i]->attach(servos[i]);
  }
//axis1.write(90);
//axis2.write(90);
  pinMode(2,INPUT_PULLUP);

  Serial.begin(9600);
}

dir readDir(byte analog){

  
  currval = map(analogRead(analog),0,1024,0,180);
  if (currval > 120){
    return FORWARD;
  }else if (currval < 60){
    return BACKWARD;
  }else{
    return STILL;
  }
  
}
void debug(){
  x+=fuck;
  Serial.println(x);
  if (x > 180 || x < 0){
    fuck*=-1;
  }
  for (byte i = 1; i<4; i++){
    axi[i]->write(x);
    delay(15);
  }
}

void debug(byte analog){
  x+=fuck;
  Serial.println(x);
  if (x > 180 || x < 0){
    fuck*=-1;
  }
  for (byte i = 1; i<4; i++){
    axi[i]->write(x);
    delay(15);
  }
}

void loop(){
 // /*
  for (byte i = 0; i<4; i++){
    switch (readDir(pins[i])){
      case FORWARD:
        val[i]+=stp[i];
        break;
      case BACKWARD:
        val[i]-=stp[i];
        break;
      default:
        break;
    }
    if (val[i] > 180){
      val[i] = 180;
    }else if (val[i] < 10){
      val[i] = 10;
    }
    axi[i]->write(val[i]);
    Serial.print(i);
    Serial.print(": ");
    Serial.println(val[i]);
    delay(t);
  }
 // */
 // debug();
  if (!digitalRead(2) && !pressing){
    pressing = true;
    mode=!mode;
  }
  if (digitalRead(2)){
    pressing = false;
  }

  if (mode){
    claw.write(0);
    delay(t);
  }else{
    claw.write(90);
    delay(t);
  }
//  delay(t);
}
