/*****************************************************
 * This file is to test the key switches using the serial monitor built into the Arduino IDE.
 ****************************************************/
#include <array>
#define sax 1;
int fingeredNote;
int keySetup=sax;

void setup() 
{
  // Initializes serial communication at 9600 bits per second:
  Serial.begin(9600);
  for(int i=0; i<8; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
}

// the loop routine runs over and over again forever:
void loop() 
{
  getNote();
  Serial.print(fingeredNote);
  /// Optionally prints the value that would be sent over MIDI Control Change.
  Serial.println();
  /// delay 100 milliseconds (1/10 of a second)
  delay(100);
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

  ///Read from touch sensors


  
  //if(keySetup==sax)
  //{
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
  //}

  if(key==Cs)
  {
    fingeredNote=BASE; //C#
  }
  else if(key==C)
  {
    fingeredNote=BASE - 1; //C
  }
  else if(key==B)
  {
    fingeredNote=BASE - 2; //B
  }
  else if(key==Bb)
  {
    fingeredNote=BASE - 3; //Bb
  }
  else if(key==A)
  {
    fingeredNote=BASE - 4; //A
  }
  else if(key==Gs)
  {
    fingeredNote=BASE - 5; //G#
  }
  else if(key==G)
  {
    fingeredNote=BASE - 6; //G
  }
  else if(key==Fs)
  {
    fingeredNote=BASE - 7; //F#
  }
  else if(key==F)
  {
    fingeredNote=BASE - 8; //F
  }
  else if(key==E)
  {
    fingeredNote=BASE - 9; //E
  }
  else if(key==Eb)
  {
    fingeredNote=BASE - 10; //D#
  }
  else if(key==D)
  {
    fingeredNote=BASE - 11; //D
  }
  else
  {

  }

}
