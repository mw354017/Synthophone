
/*
NAME:                 TeensieWI
WRITTEN BY:           JOHAN BERGLUND
CREDITS:              State machine from the Gordophone blog by GORDON GOOD
DATE:                 2016-06-09
FILE SAVED AS:        TeensieWI.ino
FOR:                  PJRC Teensy LC and Teensyduino
CLOCK:                48.00 MHz                                       
PROGRAMME FUNCTION:   Simple Wind Controller using the Freescale MPX5010GP breath sensor
                      and capacitive touch keys. Output to USB MIDI.  
HARDWARE NOTES:
    
* The Freescale MPX5010GP pressure sensor output (V OUT) is connected to pin 21(A7).
* (Warning: no voltage limiting before input, can harm Teensy if excessive pressure is applied.)
* 
* Sensor pinout
* 1: V OUT (pin with indent)
* 2: GND (to GND pin of Teensy)
* 3: VCC (to 5V pin of Teensy)    
* 4: n/c
* 5: n/c
* 6: n/c
*     
* Touch sensors are using the Teensy LC built in touchRead function.
* Electrodes connect directly to Teensy pins.
* 
* Force Sensitive Resistor (DIY or other) connected from 3.3V to pin 14 (A0).
* A resistor is connected from the input to GND. Value about 10K. (Try 1K to 25K if input is too far off).
* Adjust FSR_Thr and FSR_Max to values that makes sense for your FSR.
* 
* Slide switch connecting pin 11 to GND for switching FSR between modulation and pitch bend down.
* 
*/ 

//_______________________________________________________________________________________________ DECLARATIONS
#include <array>

#define BASE 60
#define octaveShift 12

#define ON_Thr 90       // Set threshold level before switching ON
#define ON_Delay   20   // Set Delay after ON threshold before velocity is checked (wait for tounging peak)
#define breath_max 400  // Threshold for maximum breath
#define FSR_Thr 30
#define FSR_Max 200
#define PB_sens 4095    // Pitch Bend sensitivity 0 to 8191 where 8191 is full pb range, 4095 half

// The three states of our state machine

// No note is sounding
#define NOTE_OFF 1

// We've observed a transition from below to above the
// threshold value. We wait a while to see how fast the
// breath velocity is increasing
#define RISE_WAIT 2

// A note is sounding
#define NOTE_ON 3

// Send CC data no more than every CC_INTERVAL
// milliseconds
#define CC_INTERVAL 20 


//variables setup

int state;                         // The state of the state machine
unsigned long ccSendTime = 0L;     // The last time we sent CC values
unsigned long breath_on_time = 0L; // Time when breath sensor value went over the ON threshold
int initial_breath_value;          // The breath value at the time we observed the transition

unsigned long lastDebounceTime = 0;         // The last time the fingering was changed
unsigned long debounceDelay = 20;           // The debounce time; increase if the output flickers
int lastFingering = 0;             // Keep the last fingering value for debouncing

byte MIDIchannel=2;                // MIDI channel 1

int breathLevel=0;   // breath level (smoothed) not mapped to CC value

int pressureSensor;  // pressure data from breath sensor, for midi breath cc and breath threshold checks
byte velocity;       // remapped midi velocity from breath sensor

int currentNote;    // note calculated from fingering (switches) and octave joystick position
byte activeNote;     // note playing
byte startNote=73;   // set startNote to C# (change this value in steps of 12 to start in other octaves)
byte fsrMode=1;        // 0 is bend down, 1 is Modulation
byte fsrIsOn=0;
int fsrModulation;
int fsrBend;


            // Key variables, TRUE (1) for pressed, FALSE (0) for not pressed
byte LH1;   // Left Hand key 1 (pitch change -2) 
            // Casio style 2nd octave: If LH1 is not touched when LH2 and LH3 are, pitch change +9
byte LH2;   // Left Hand key 2  (with LH1 also pressed pitch change is -2, otherwise -1)
byte LH3;   // Left Hand key 3 (pitch change -2)
byte LHp1;  // Left Hand pinky key 1 (pitch change +1)
byte RH1;   // Right Hand key 1 (with LH3 also pressed pitch change is -2, otherwise -1)
byte RH2;   // Right Hand key 2 (pitch change -1)
byte RH3;   // Right Hand key 3 (pitch change -2)
byte RHp2;  // Right Hand pinky key 2 (pitch change -1)
byte RHp3;  // Right Hand pinky key 3 (pitch change -2, in this version -1 if RHp2 is pressed)
byte OCTup; // Octave switch key (pitch change +12) 
byte OCTdn; // Octave switch key (pitch change -12) 

void sendData(String type, int data);


//_______________________________________________________________________________________________ SETUP
void setup() {
  Serial.begin(38400);
  state = NOTE_OFF;  // initialize state machine
  pinMode(11,INPUT_PULLUP);
  fsrMode=digitalRead(11);
  for(int i=0; i<8; i++)
  {
    pinMode(i, INPUT_PULLUP);
  }
}

//_______________________________________________________________________________________________ MAIN LOOP

void loop() {
  pressureSensor = analogRead(A0); // Get the pressure sensor reading from analog pin A7
  sendData("breath ", pressureSensor);

  if (state == NOTE_OFF) {
    if (pressureSensor > ON_Thr) {
      // Value has risen above threshold. Move to the ON_Delay
      // state. Record time and initial breath value.
      breath_on_time = millis();
      initial_breath_value = pressureSensor;
      state = RISE_WAIT;  // Go to next state
    }
  } else if (state == RISE_WAIT) {
    if (pressureSensor > ON_Thr) {
      // Has enough time passed for us to collect our second
      // sample?
      if (millis() - breath_on_time > ON_Delay) {
        // Yes, so calculate MIDI note and velocity, then send a note on event
        //readSwitches();
        getNote();
        // We should be at tonguing peak, so set velocity based on current pressureSensor value        
        // If initial value is greater than value after delay, go with initial value, constrain input to keep mapped output within 1 to 127
        velocity = map(constrain(max(pressureSensor,initial_breath_value),ON_Thr,breath_max),ON_Thr,breath_max,1,100)+27;
        breathLevel=constrain(max(pressureSensor,initial_breath_value),ON_Thr,breath_max);
        breath(); // send breath data
        usbMIDI.sendNoteOn(currentNote, velocity, MIDIchannel); // send Note On message for new note 
        activeNote=currentNote;
        state = NOTE_ON;
      }
    } else {
      // Value fell below threshold before ON_Delay passed. Return to
      // NOTE_OFF state (e.g. we're ignoring a short blip of breath)
      state = NOTE_OFF;
    }
  } else if (state == NOTE_ON) {
    if (pressureSensor < ON_Thr) {
      // Value has fallen below threshold - turn the note off
      usbMIDI.sendNoteOff(activeNote, velocity, MIDIchannel); //  send Note Off message 
      breathLevel=0;
      state = NOTE_OFF;
    } else {
      // Is it time to send more CC data?
      if (millis() - ccSendTime > CC_INTERVAL) {
         // deal with Breath, Pitch Bend and Modulation
         breath();
         //fsr();
         ccSendTime = millis();
      }
      getNote();
      //readSwitches();
      if (currentNote != lastFingering){ //
        // reset the debouncing timer
        lastDebounceTime = millis();
      }
      if ((millis() - lastDebounceTime) > debounceDelay) {
      // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state
        if (currentNote != activeNote) {
          // Player has moved to a new fingering while still blowing.
          // Send a note off for the current note and a note on for
          // the new note.      
          velocity = map(constrain(pressureSensor,ON_Thr,breath_max),ON_Thr,breath_max,7,100)+27; // set new velocity value based on current pressure sensor level
          usbMIDI.sendNoteOn(currentNote, velocity, MIDIchannel); // send Note On message for new note         
          usbMIDI.sendNoteOff(activeNote, 0, MIDIchannel); // send Note Off message for previous note (legato)
          activeNote=currentNote;
        }
      }
     }
  }
  lastFingering=currentNote; 
}
//_______________________________________________________________________________________________ FUNCTIONS

void breath(){
  int breathCC;
  breathLevel = breathLevel*0.8+pressureSensor*0.2; // smoothing of breathLevel value
  breathCC = map(constrain(breathLevel,ON_Thr,breath_max),ON_Thr,breath_max,0,127);
  usbMIDI.sendControlChange(2, breathCC, MIDIchannel);
  Serial.print(breathCC);
  Serial.println();
}

//***********************************************************
/*
void fsr(){
  int fsrReading;
  fsrReading=analogRead(A1);
  if (fsrReading >= FSR_Thr){
    if (fsrMode){
      // modulation
      fsrModulation = map(constrain(fsrReading,FSR_Thr,FSR_Max),FSR_Thr,FSR_Max,0,127);
      usbMIDI.sendControlChange(1, fsrModulation, MIDIchannel);
      fsrIsOn=1;
    } else {
      // bend down
      fsrBend = map(constrain(fsrReading,FSR_Thr,FSR_Max),FSR_Max,FSR_Thr,(8191 - PB_sens),8192);
      usbMIDI.sendPitchBend(fsrBend, MIDIchannel);
      fsrIsOn=1;
    }
  } else if (fsrIsOn){
    //send 0 value
    if (fsrMode){
      usbMIDI.sendControlChange(1, 0, MIDIchannel);
    } else {
      usbMIDI.sendPitchBend(8192, MIDIchannel); // 8192 is 0 pitch bend
    }
    fsrIsOn=0;
  }
}
*/

//***********************************************************

/*
void readSwitches(){  
  // Read switches and put value in variables
  LH1=touchRead(17)>1500;
  LH2=touchRead(4)>1500;
  LH3=touchRead(3)>1500;
  LHp1=touchRead(18)>1500;
  RH1=touchRead(19)>1500;
  RH2=touchRead(22)>1500;
  RH3=touchRead(23)>1500;
  RHp2=touchRead(1)>1000;
  RHp3=touchRead(0)>1000;
  OCTup=touchRead(15)>1500;
  OCTdn=touchRead(16)>1500;
  //calculate midi note number from pressed keys  
  currentNote=startNote-2*LH1-LH2-(LH2 && LH1)-2*LH3+LHp1-RH1-(RH1 && LH3)-RH2-2*RH3-RHp2-2*RHp3+(RHp2 && RHp3)+12*OCTup-12*OCTdn+9*(!LH1 && LH2 && LH3);
}
*/


void getNote() 
{
  // This function reads from our 8 switches and returns a midi note value.
  #define BASE 60
  #define octaveShift 12
  
  std::array<bool, 8> key {0,0,0,0,0,0,0,0};
  for(int i=0; i<8; i++)
  {
    key[i]=!digitalRead(i);
  }

  
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
/*  else if(keySetup==binary)
  {
    bool Cs[8]={0,0,0,0,0,0,0,0};
    bool C[8]={0,0,0,0,0,0,1,0};
    bool B[8]={0,0,0,0,0,0,0,1};
    bool Bb[8]={0,0,0,1,0,0,0,1};
    bool A[8]={0,0,0,0,0,0,1,1};
    bool Gs[8]={0,0,0,0,1,1,1,1};
    bool G[8]={0,0,0,0,0,1,1,1};
    bool Fs[8]={0,0,1,0,0,1,1,1};
    bool F[8]={0,0,0,1,0,1,1,1};
    bool E[8]={0,0,1,1,0,1,1,1};
    bool Eb[8]={1,1,1,1,0,1,1,1};
    bool D[8]={0,1,1,1,0,1,1,1};
  }
  
  */

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

  ///Read from touch octave keys

  
  if(touchRead(23)>2000 && !(touchRead(22)>2000 || touchRead(19)>2000 || touchRead(18)>2000))
  {
    currentNote -= 3*octaveShift;
  }
  else if(touchRead(23)>2000 && touchRead(22)>2000 && !(touchRead(19)>2000 || touchRead(18)>2000))
  {
    currentNote -= 2*octaveShift;
  }
  else if(touchRead(22)>2000 && !(touchRead(23)>2000 || touchRead(19)>2000 || touchRead(18)>2000))
  {
    currentNote -= octaveShift;
  }
  else if(touchRead(19)>2000 && !(touchRead(23)>2000 || touchRead(22)>2000 || touchRead(18)>2000))
  {
    currentNote += octaveShift;
  }
  else if(touchRead(19)>2000 && touchRead(18)>2000 && !(touchRead(23)>2000 || touchRead(22)>2000))
  {
    currentNote += 2*octaveShift;
  }
  else if(touchRead(18)>2000 && !(touchRead(23)>2000 || touchRead(22)>2000 || touchRead(19)>2000))
  {
    currentNote += 3*octaveShift;
  }
  
      //Serial.println("note B");

}


void sendData(String type, int data){
    Serial.print(type);
    Serial.println(data); // need a end-line because wrmlh.csharp use readLine method to receive data
}

