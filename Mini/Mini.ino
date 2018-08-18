
bool pr, mode,estado;
void setup() {
  // put your setup code here, to run once:
  pinMode(12,INPUT);
  pinMode(10,OUTPUT);
  Serial.begin(9600);
   pr = false;
   mode = false;
  //Serial.print("hmm");
}

void loop() {
 
  if(digitalRead(12) == LOW){
   pr = false;
  }
  if(digitalRead(12) == HIGH && !pr){
   pr = true;
   mode = !mode;
   Serial.print("Mode: ");
   Serial.println(mode);
  }
  
  //blink
  estado = mode;

  if (estado){
    digitalWrite(10,HIGH);
  }else{
    digitalWrite(10,LOW);
  }
}

