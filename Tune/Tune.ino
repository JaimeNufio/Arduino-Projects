
//#define tempo 400

String t[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
#define G5 79
#define FS5 78
#define F5 77
#define E5 76
#define DS5 75
#define D5 74
#define CS5 73
#define C5 72
#define B4 71 //71 is the normal B, but in Bminor set to 70
#define AS4 70
#define A4 69
#define GS4 68
#define G4 67
#define FS4 66
#define F4 65
#define E4 64
#define DS4 64
#define D4 62
#define CS4 61
#define C4 60
#define R 250

#define BUZZ 10

byte notesDaisy[] = {
  C5, A4, F4, C4, D4, E4, F4, D4, F4, C4,
  D4, B4, A4, F4, D4, E4, F4, G4, A4, G4, A4, B4, A4, G4,
  C5, A4, G4, F4, G4, A4, F4, D4, F4, D4, C4, C4,
  F4, A4, G4, R, F4, A4, G4, A4, B4, C5, A4, F4, G4, C4, F4
};

//in multiples of quarters
byte lenDaisy[] = {
  3, 3, 3, 3, 1, 1, 1, 2, 1, 6,
  3, 3, 3, 3, 1, 1, 1, 2, 1, 5, 1, 1, 1, 1,
  2, 1, 1, 4, 1, 2, 1, 2, 1, 1, 4, 1,
  2, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 5
};

byte notesGreen[] = {
  A4, C5, D5, E5, F5, E5, D5, B4,  G4, A5, B4, C5, A4, A4, GS4, A4,
  B4, FS4, E4, A4, C5, D5, E5, F5, E5, D5, B4, G4, A4, B4,
  C5, B4, A4, GS4, FS4, GS4, A4, A4, G5, G5, F5, E5, F5, B4, G4, A4, B4,

  // A4,B4,C4,D5,E5,C5,
};

//in multiples of eigths
byte lenGreen[] = {
  
  2,  4, 2,  3, 1, 2,  4, 2,  3, 1, 2,  4, 2,  3, 1, 2,
  4, 2,  4, 2,  4, 2,  3, 1, 2,  4, 2,  3, 1, 2,
  3, 1, 2,  3, 1, 2,  6,  6,  6, 3, 1, 2, 2, 1,  3, 1, 2,

  //1,1,1,1,1,1,
};

int beat = 0, tempo = map(analogRead(1),0,710,0,800),currTempo,
  currSong = 0;
//0 Daisy Bell
//1 Greensleeves


void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7,INPUT_PULLUP);
  digitalWrite(4,LOW);
  Serial.begin(19200);
  
}

int scale(int place) {
  if (place == C4) {
    return 262;
  }
  return (pow(2, (float)(place - 69) / 12) * 440);
}

int getBeat(int bpm) {
  return ((float)60 / bpm) * 1000;
}

String mapNote(byte note) {
  if (note < 200) {
    return t[(60 - note) % 13];
  } else {
    return "Rest";
  }
}

void playNote(int midi, int len) {
  //midi is MIDI number, len is the multiple of a quarter note
  //TODO implement either and smaller notes.
  //  getBeat needs to divide by 4, or more and here we'd multiply

  //midi+=12; //shift an octave
  len = beat * len;
  //Serial.println(scale(midi));
  buzz(BUZZ, scale(midi), len / 2);
  delayMicroseconds(len / 2 * 1.3);
  buzz(BUZZ, 0, len);
}

void pN(int midi, int len) {
  playNote(midi, len);
}

void changeSong(){
  currSong++;
  if (currSong > 1){
    currSong = 0;
  }
  if (currSong == 0){
    playMusic(notesDaisy,lenDaisy,sizeof(notesDaisy)/sizeof(byte));
  }else{
    playMusic(notesGreen,lenGreen,sizeof(notesGreen)/sizeof(byte));
  }
}

void playMusic(byte notes[], byte len[],int s){
    Serial.println(sizeof(notes));
    for (unsigned i = 0; i < s; i++) {
      Serial.print("Note: ");
      Serial.print(mapNote(notes[i]));
      Serial.print(" Beats: ");
      Serial.print(len[i]);
      Serial.print(" Frequency: ");
      Serial.print(scale(notes[i]));
      Serial.print(" Tempo: ");
      Serial.println(map(analogRead(1),0,710,0,800));
      //Serial.println(analogRead(1));
    currTempo = map(analogRead(1), 0, 710, 0, 800);
      if (abs(tempo-currTempo) > 10 and currTempo > 100){
       tempo = currTempo;
        beat = getBeat(tempo);
      }
      playNote(notes[i], len[i]);
      if (digitalRead(7)==LOW){
        digitalWrite(4,HIGH);
        changeSong();
        break;
      }else{
        digitalWrite(4,LOW);
      }
  }
}

void loop() {
  beat = getBeat(tempo);
  //playMusic(notesGreen,lenGreen,sizeof(notesDaisy)/sizeof(byte));
  changeSong();
 // delay(15000);
}

void buzz(int targetPin, long frequency, long length) {
//  digitalWrite(13, HIGH);
  long delayValue = 1000000 / abs(frequency) / 2;
  long numCycles = abs(frequency) * length / 1000;


  for (long i = 0; i < numCycles; i++) {
    if (frequency != 250) {
      digitalWrite(targetPin, HIGH);
    }
    delayMicroseconds(delayValue);
    digitalWrite(targetPin, LOW);
    delayMicroseconds(delayValue);
  }
//  digitalWrite(13, LOW);

}
