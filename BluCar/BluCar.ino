char blu = 0;   

struct package{
  enum XDIRECTION {LEFT,RIGHT,CENTER};
  enum YDIRECTION {FORWARD,BACKWARD,NONE};
  
  XDIRECTION aX;
  YDIRECTION aY;
  
  XDIRECTION bX;
  YDIRECTION bY;
  int aValY, aValX, bValY, bValX;
  bool buttonA, buttonB, readGyro;
  byte packetTest = 0;
  int errorX, errorY;
};

typedef struct package Package;
Package data;

int motorA[] = {14,15,5}; //last is the enable
int motorB[] = {16,17,6};

int favor,sign = 2;

void setup() {

  Serial.begin(9600);
  for (int i = 0; i<3; i++){
    pinMode(motorA[i],OUTPUT);
    pinMode(motorB[i],OUTPUT);
  }
  
 // pinMode(control[0],INPUT); 
 // pinMode(control[1],INPUT);
  pinMode(sign,OUTPUT);

  //default, forward.
  digitalWrite(motorA[0],HIGH);
  digitalWrite(motorB[0],HIGH);
  digitalWrite(motorA[1],LOW);
  digitalWrite(motorB[1],LOW);
  digitalWrite(motorA[2],0);
  digitalWrite(motorB[2],0);

   digitalWrite(motorA[2],255);
   digitalWrite(motorB[2],150);
}

void setSpd(){ 
  int valX = data.aValX, valY = data.aValY;
   //valX = valY = 100;
  if (data.aX == package::XDIRECTION::LEFT){
//  Serial.println("LEFT");
 //   digitalWrite(motorA[2],205);//(int)(valY*favor));//Left Wheel
 //   digitalWrite(motorB[2],0);
  }else if (data.aX == package::XDIRECTION::RIGHT){
//   Serial.println("RIGHT");
 //   digitalWrite(motorA[2],0);
  //  digitalWrite(motorB[2],250);
  }else{
 //   Serial.println("not left or right");
//    digitalWrite(motorA[2],250);
 //   digitalWrite(motorB[2],250);
  }
}

void spin(){
    int valX = data.aValX;
    digitalWrite(motorA[0],LOW);
    digitalWrite(motorB[0],LOW);
    digitalWrite(motorA[1],HIGH);
    digitalWrite(motorB[1],HIGH);
  if (data.aX == package::XDIRECTION::LEFT){
    digitalWrite(motorA[2],0);
    digitalWrite(motorB[2],valX);
  }else if (data.aX == package::XDIRECTION::RIGHT){
    digitalWrite(motorA[2],valX);
    digitalWrite(motorB[2],0);
  }else{
    digitalWrite(motorA[0],LOW);
    digitalWrite(motorB[0],LOW);
    digitalWrite(motorA[1],LOW);
    digitalWrite(motorB[1],LOW);
    analogWrite(motorA[1],0); 
    analogWrite(motorA[2],0);    
  }
}

void setDir(){
  if (data.aY == package::YDIRECTION::FORWARD){
    digitalWrite(motorA[0],LOW);
    digitalWrite(motorB[0],LOW);
    digitalWrite(motorA[1],HIGH);
    digitalWrite(motorB[1],HIGH);
 //   Serial.print("wheels forawrd");
//    Serial.print(", favors ");
 //   Serial.println(X[data.x]);
    setSpd();
  }else if (data.aY == package::YDIRECTION::BACKWARD){
    digitalWrite(motorA[0],HIGH);
    digitalWrite(motorB[0],HIGH);
    digitalWrite(motorA[1],LOW);
    digitalWrite(motorB[1],LOW);
  //  Serial.print("wheels backward");
 //   Serial.print(", favors ");
  //  Serial.println(X[data.x]);
    setSpd();
  }else{
   // Serial.println("No Y movement, ");
    data.aValY = 0; 
    spin();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
 if(Serial.available() > 0)  // Send data only when you receive data:
  {

    blu = Serial.read();      //Read the incoming data and store it into variable data
//    Serial.print(data);        //Print Value inside data in Serial monitor
 //   Serial.print("\n");        //New line 
    
    switch(blu){
      //On Buttons for Directional
      case 1:
              digitalWrite(sign,HIGH);
        data.aY = package::YDIRECTION::FORWARD;
        break;
      case 2:
              digitalWrite(sign,HIGH);
        data.aY = package::YDIRECTION::BACKWARD;
        break;
      case 3:
              digitalWrite(sign,HIGH);
        data.aX = package::XDIRECTION::LEFT;
        break;
      case 4:
              digitalWrite(sign,HIGH);
        data.aX = package::XDIRECTION::RIGHT;
        break;

      //Buttons Alpha Beta
      case 5:
        data.buttonA = true;
        digitalWrite(sign,HIGH);
        break;
      case 6:
        digitalWrite(sign,HIGH);
        data.buttonB = true;
        break;



      //Off Buttons Alpha Beta
      case (122):
        data.buttonA = false;
        digitalWrite(sign,LOW);
        break;
      case (121):
        digitalWrite(sign,LOW);
        data.buttonB = false;
        break;

      //Off Buttons for Directional  
      case (124):
              digitalWrite(sign,LOW);
        data.aX = package::XDIRECTION::CENTER;
        break;
      case (123):
              digitalWrite(sign,LOW);
        data.aX = package::XDIRECTION::CENTER;
        break; 
      case (126):
              digitalWrite(sign,LOW);
        data.aY = package::YDIRECTION::NONE;
        break;
      case (125):
              digitalWrite(sign,LOW);
        data.aY = package::YDIRECTION::NONE;
        break; 
     }
     setDir();
  }
}
