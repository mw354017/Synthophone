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

void getNote();

void setup() 
{
  currentState = NOTE_OFF;
  for(int i=0; i<8; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() 
{
  breathValue = analogRead(A0);
  if(currentState==NOTE_OFF)
  {
    if(breathValue > breathThreshold)
    {
      getNote();
      usbMIDI.sendNoteOn(currentNote, velocity, midiChannel);
      currentState=NOTE_ON;
    }
  }
  else if(currentState==NOTE_ON)
  {
    if(breathValue > breathThreshold)
    {
      getNote();
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
  #define BASE 60
  
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

  if(key==Cs)
  {
    currentNote=BASE; //C#
  }
  else if(key==C)
  {
    currentNote=BASE - 1; //C
  }
  else if(key==B)
  {
    currentNote=BASE - 2; //B
  }
  else if(key==Bb)
  {
    currentNote=BASE - 3; //Bb
  }
  else if(key==A)
  {
    currentNote=BASE - 4; //A
  }
  else if(key==Gs)
  {
    currentNote=BASE - 5; //G#
  }
  else if(key==G)
  {
    currentNote=BASE - 6; //G
  }
  else if(key==Fs)
  {
    currentNote=BASE - 7; //F#
  }
  else if(key==F)
  {
    currentNote=BASE - 8; //F
  }
  else if(key==E)
  {
    currentNote=BASE - 9; //E
  }
  else if(key==Eb)
  {
    currentNote=BASE - 10; //D#
  }
  else if(key==D)
  {
    currentNote=BASE - 11; //D
  }
  else
  {
    
  }

}
