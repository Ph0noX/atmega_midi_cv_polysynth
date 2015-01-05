#include <MIDI.h>
#include "AH_MCP4922.h"

//define AnalogOutput (MOSI_pin, SCK_pin, CS_pin, DAC_x, GAIN) 

AH_MCP4922 Pitch1(A0,A1,A2,LOW,LOW);
AH_MCP4922 Velocity1(A0,A1,A2,HIGH,LOW);

AH_MCP4922 Pitch2(A3,A4,A5,LOW,LOW);
AH_MCP4922 Velocity2(A3,A4,A5,HIGH,LOW);

AH_MCP4922 Pitch3(7,8,9,LOW,LOW);
AH_MCP4922 Velocity3(7,8,9,HIGH,LOW);

AH_MCP4922 Pitch4(10,11,12,LOW,LOW);
AH_MCP4922 Velocity4(10,11,12,HIGH,LOW);

AH_MCP4922 pitchDACs[4] = { Pitch1, Pitch2, Pitch3, Pitch4 };
AH_MCP4922 velocityDACs[4] = { Velocity1, Velocity2, Velocity3, Velocity4 };

int noteNumbers[4] = { 0, 0, 0, 0 };  // keep track of notes that have been gated on
int gatePins[4] = { 2, 3, 4, 5 };

int nextNoteOutput = 0;


MIDI_CREATE_DEFAULT_INSTANCE();


void handleNoteOn(byte channel, byte pitch, byte velocity)
{

  pitchDACs[nextNoteOutput].setValue((pitch - 12) * 42);
  velocityDACs[nextNoteOutput].setValue(velocity * 32);
  digitalWrite(gatePins[nextNoteOutput], HIGH);
  
  noteNumbers[nextNoteOutput] = pitch;
  nextNoteOutput = (nextNoteOutput + 1) % 4;

}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{  

  int i = 0;
  while (i < 4) {
    if (noteNumbers[i] == pitch) {
      digitalWrite(gatePins[i], LOW);
    }
    i++;
  }

}

void handleControlChange(byte channel, byte number, byte value)
{
  if (number == 1) {
//    AnalogOutput4.setValue(value * 32);
  }
}

void handlePitchBend(byte channel, int bend)
{
//    AnalogOutput2.setValue((float)bend/4.0 + 2048.0);
}


// -----------------------------------------------------------------------------

void setup()
{

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);

  pinMode(gatePins[0], OUTPUT);
  pinMode(gatePins[1], OUTPUT);
  pinMode(gatePins[2], OUTPUT);
  pinMode(gatePins[3], OUTPUT);

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  // Connect the handleNoteOn function to the library,
  // so it is called upon reception of a NoteOn.
  MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function

  // Do the same for NoteOffs
  MIDI.setHandleNoteOff(handleNoteOff);
  
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandlePitchBend(handlePitchBend);

  // Initiate MIDI communications
  MIDI.begin(4);

  // C8 at full velocity for 8.0V calibration on powerup
  handleNoteOn(4, 108, 127);
  handleNoteOn(4, 108, 127);
  handleNoteOn(4, 108, 127);
  handleNoteOn(4, 108, 127);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop()
{
    MIDI.read();
}

