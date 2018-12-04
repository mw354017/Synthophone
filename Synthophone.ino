using namespace std;
#include <algorithm>
#include <iterator>
#include <array>


#define midiChannel 1
#define breathThreshold 60
#define NOTE_OFF 1
#define NOTE_ON 2
#define sax 9
int currentNote;
int breathValue;
int maxBreath;
int velocity=100;
int currentState;
int ccValue;
int keySetup=sax;
int transpose=0;
int temp;

String currentNoteLetter = "C#";

void getNote();
void sendInt(String type, int data);
void sendString(String type, String data);

#define SERIAL_USB

void setup() 
{
    Serial.begin(9600); // You can choose any baudrate, just need to also change it in Unity.
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

  currentState = NOTE_OFF;
  for(int i=0; i<8; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() 
{
  temp = Serial.read();
  if(temp!=-1)
  {
    transpose = temp;
  }
  breathValue = analogRead(A0);
  sendInt("breath ", breathValue);
  if(currentState==NOTE_OFF)
  {
    if(breathValue > breathThreshold)
    {
      getNote();
      usbMIDI.sendNoteOn(currentNote, velocity, midiChannel);
      sendString("note ", currentNoteLetter);
      currentState=NOTE_ON;
    }
  }
  else if(currentState==NOTE_ON)
  {
    if(breathValue > breathThreshold)
    {
      getNote();
      sendString("note ", currentNoteLetter);
      //usbMIDI.sendNoteOn(currentNote, velocity, midiChannel);
      ccValue=map(breathValue,breathThreshold, maxBreath,0,127);
      usbMIDI.sendControlChange(7,ccValue,midiChannel);
    }
    else
    {
      usbMIDI.sendNoteOff(currentNote, velocity, midiChannel);
      currentState=NOTE_OFF;
    }
  }
}

void getNote() 
{
  // This function reads from our 8 switches and returns a midi note value.
  int BASE=60;
  std::array<bool, 8> key {0,0,0,0,0,0,0,0};
  for(int i=0; i<8; i++)
  {
    key[i]=!digitalRead(i);
  }
  std::array<bool, 8> Cs {0,0,0,0,0,0,0,0};
  std::array<bool, 8> C {0,0,0,0,0,0,1,0};
  std::array<bool, 8> B {0,0,0,0,0,0,0,1};
  std::array<bool, 8> Bb {0,0,0,1,0,0,0,1};
  std::array<bool, 8> A {0,0,0,0,0,0,1,1};
  std::array<bool, 8> Gs {0,0,0,0,1,1,1,1};
  std::array<bool, 8> G {0,0,0,0,0,1,1,1};
  std::array<bool, 8> Fs {0,0,1,0,0,1,1,1};
  std::array<bool, 8> F {0,0,0,1,0,1,1,1};
  std::array<bool, 8> E {0,0,1,1,0,1,1,1};
  std::array<bool, 8> Eb {1,1,1,1,0,1,1,1};
  std::array<bool, 8> D {0,1,1,1,0,1,1,1};
 
 BASE+=transpose;
  if(key==Cs)
  {
    currentNote=BASE; //C#
    currentNoteLetter = "C#";
  }
  else if(key==C)
  {
    currentNote=BASE - 1; //C
    currentNoteLetter = "C";
  }
  else if(key==B)
  {
    currentNote=BASE - 2; //B
    currentNoteLetter = "B";
  }
  else if(key==Bb)
  {
    currentNote=BASE - 3; //Bb
    currentNoteLetter = "Bb";
  }
  else if(key==A)
  {
    currentNote=BASE - 4; //A
    currentNoteLetter = "A";
  }
  else if(key==Gs)
  {
    currentNote=BASE - 5; //G#
    currentNoteLetter = "A#";
  }
  else if(key==G)
  {
    currentNote=BASE - 6; //G
    currentNoteLetter = "G";
  }
  else if(key==Fs)
  {
    currentNote=BASE - 7; //F#
    currentNoteLetter = "F#";
  }
  else if(key==F)
  {
    currentNote=BASE - 8; //F
    currentNoteLetter = "F";
  }
  else if(key==E)
  {
    currentNote=BASE - 9; //E
    currentNoteLetter = "E";
  }
  else if(key==Eb)
  {
    currentNote=BASE - 10; //D#
    currentNoteLetter = "D#";
  }
  else if(key==D)
  {
    currentNote=BASE - 11; //D
    currentNoteLetter = "D";
  }
  else
  {
    
  }
 

}

void sendInt(String type, int data){
    Serial.print(type);
    Serial.println(data); // need a end-line because wrmlh.csharp use readLine method to receive data
}


void sendString(String type, String data){
    Serial.print(type);
    Serial.println(data); // need a end-line because wrmlh.csharp use readLine method to receive data
}
