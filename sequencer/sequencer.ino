#define DEBUG

#include "Debug.h"
#include "AnalogButtonHandler.h";
#include "Sequencer.h";
#include "UI.h";



// Define below to write debug output to Serial
#define START_BPM 60

// Pins
uint8_t buttonPinNote = A0;
uint8_t buttonPinLogic = A1;
uint8_t clockPin = 9;
uint8_t latchPin = 11;
uint8_t dataPin = 12;
uint8_t midiPin = 18;

// Midi
uint8_t midiNote = 0;
uint8_t previousMidiNote = 0;
const uint8_t noteOnCommand = 144;
const uint8_t noteOffCommand = 128;
const uint8_t noteVelocityOn = 100;
const uint8_t noteVelocityOff = 0;

// UI
UI ui(clockPin, dataPin, latchPin);
volatile bool updateUI = true;

// Sequencer
Sequencer sequencer(START_BPM, &updateUI);

// Buttons
#define BUTTON_DEBOUNCE 30
unsigned long lastButtonReadNote = 0;
unsigned long lastButtonReadLogic = 0;
AnalogButtonHandler buttonHandlerNote(A0, BUTTON_DEBOUNCE); 
AnalogButtonHandler buttonHandlerUI(A1, BUTTON_DEBOUNCE); 

void setup() {
  // Set pins
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  pinMode(midiPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  // Setup serial
  Serial1.begin(31250);
  Serial.begin(9600);
  buttonHandlerNote.attachCallback(processButtonPressNote);
  buttonHandlerUI.attachCallback(processButtonPressLogic);
  sequencer.init();
  ui.init();
  
}

void loop() {
  unsigned long currentTime = millis();
  buttonHandlerNote.readValue(currentTime);
  buttonHandlerUI.readValue(currentTime);
  sequencer.loop();

  if(updateUI) {
    DEBUG_PRINTLN("updateUI");
    uint8_t myNote = sequencer.getNote(); 
    ui.setMaskValue(myNote);
    ui.setNoteLed();
    ui.updateDisplay(sequencer.getMode(), sequencer.getStep(), sequencer.getTempo());    
    updateUI = false;
  }
}

void processButtonPressNote(int buttonValue) {
  // Store current note as previous
  previousMidiNote = midiNote;
  bool dontSetNote = false;


  
  if(buttonValue < 5) {
    dontSetNote = true;    
  }  else if(buttonValue <  40) {
    midiNote = ui.handleNoteSelect(24);
  } else if(buttonValue <  55) {
    midiNote = ui.handleNoteSelect(26);
  } else if(buttonValue <  65) {
    midiNote = ui.handleNoteSelect(28);
  } else if(buttonValue <  78) {
    midiNote = ui.handleNoteSelect(29);
  } else if(buttonValue <  90) {
    midiNote = ui.handleNoteSelect(31);
  } else if(buttonValue <  105) {
    midiNote = ui.handleNoteSelect(33);
  } else if(buttonValue <  120) {
    midiNote = ui.handleNoteSelect(35);
  } else if(buttonValue <  140) {
    midiNote = ui.handleNoteSelect(36);
  } else if(buttonValue <  160) {
    midiNote = ui.handleNoteSelect(36);
  } else if(buttonValue <  195) {
    midiNote = ui.handleNoteSelect(27);
  } else if(buttonValue <  250) {
    midiNote = ui.handleNoteSelect(30);
  } else if(buttonValue <  350) {
    midiNote = ui.handleNoteSelect(32);
  } else if(buttonValue <  510) {
    midiNote = ui.handleNoteSelect(34);
  }
  if(!dontSetNote && sequencer.setMidiNote(midiNote)) {
    DEBUG_PRINT("buttonValue ");
    DEBUG_PRINTLN(buttonValue);
    DEBUG_PRINT("midi note ");
    DEBUG_PRINTLN(midiNote);
    DEBUG_PRINT("Note set in sequencer ");
    DEBUG_PRINTLN(sequencer.getNote());
    DEBUG_PRINT("Step ");
    DEBUG_PRINTLN(sequencer.getStep());
    //writeMidi();
    updateUI = true;
  }
}

void writeMidi() {
  // Note           C   D   E   F   G   A   B   C
  // Midi code      24  26  28  29  31  33  35  36
  // whiteNoteMask  1   2   4   8   16  32  64  128
  // Note           C#  D#  F#  G#  A#
  // Midi code      25  27  30  32  34  
  // blackNoteMask  1   2   4   8   16
  // NoteOn   144
  // NoteOff  128
  if(previousMidiNote == midiNote) {
    // Don't act
  } else if (previousMidiNote == 0 && midiNote > 0) {
    // Send note on
    writeSerialMidi(noteOnCommand, midiNote, noteVelocityOn);
    digitalWrite(LED_BUILTIN, HIGH);
  } else if(previousMidiNote > 0 && midiNote == 0) {
    // Send note off
    writeSerialMidi(noteOffCommand, previousMidiNote, noteVelocityOff);
    digitalWrite(LED_BUILTIN, LOW);
  } else if(previousMidiNote > 0 && midiNote > 0) {
    writeSerialMidi(noteOffCommand, previousMidiNote, noteVelocityOff);
    writeSerialMidi(noteOnCommand, midiNote, noteVelocityOn);    
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void writeSerialMidi(uint8_t cmd, uint8_t note, uint8_t velocity) {
  Serial1.write(cmd);
  Serial1.write(note);
  Serial1.write(velocity);

  DEBUG_PRINT(cmd == noteOffCommand ? "OFF " : "ON ");
  DEBUG_PRINTLN(cmd);
  DEBUG_PRINTLN(note);
  DEBUG_PRINTLN(velocity);
  DEBUG_PRINTLN("-------");
}
