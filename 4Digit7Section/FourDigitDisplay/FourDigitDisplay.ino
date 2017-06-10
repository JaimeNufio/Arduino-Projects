int Segments[] = {1,2,3,4,5,7,10,11};
int Digits[] = {8,10,11,2};
int Numbers[][] = {
  {7,5},
  {11,7,5,1,2},
  {11,7,5,4,2},
  {10,5,7,4},
  {11,10,5,4,2},
  {11,10,5,4,2,1},
  {11,7,4},
  {11,10,5,7,1,2,4},
  {11,10,5,7,4}
};


void setup(){

  for (int i = 0; i<sizeof(Segments)/sizeof(Segments)-1;i++){
    pinMode(Segments[i]+2,OUTPUT);
    digitalWrite(Segments[i]+2,HIGH);
  }
  for (int i = 0; i<sizeof(Digits)/sizeof(Digits)-1;i++){
    pinMode(Digits[i],OUTPUT);
    digitalWrite(Digits[i],HIGH);
  }
}

boolean inArray(int[] Array, int num){
    for (int i = 0; i<sizeof(Array)/sizeof(Array)-1;i++){ 
      if  (Array[i] == num){
        return true;
      }
    }
    return false;
}

void lightup(int[] series, boolean false){
  for (int i = 0; i<sizeof(series)/sizeof(series)-1;i++){
    if inArray(Segment, inArray(series[i]){
    
    }
  }
}

void loop(){
    for (int i = 0; i<sizeof(Segments)/sizeof(Segments)-1;i++){ //  for each digit
      digitalWrite(Segments[i]+2,HIGH); //  All on, test.
    }
    for (int i = 0; i<sizeof(Digits)/sizeof(Digits)-1;i++){ //  for each digit
      delay(200);
      if (i>0){
        digitalWrite(Digits[i-1],HIGH); //clog previous pipe if there is one
      }
      digitalWrite(Digits[i],LOW);  //  free current pipe

      lightup(numbers[],false);

      digitalWrite(Digits[i],HIGH); //  clog pipe before next loop
  }
}

