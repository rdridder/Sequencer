#define DEBUG
#include "Debug.h"
#include "Arduino.h"
#include "Sequencer.h"

ISR(TIMER3_COMPA_vect)          // timer compare interrupt service routine
{
  Sequencer::isrCallback();
}

// Quarter note one beat long.
// Half note two beats long.
// Whole note is four beats.
// Eighth note is half a quarter note.
// Sixteenth note is half an eight note.

Sequencer::Sequencer(unsigned int bpm, bool *updateUI)
{
  _bpm = bpm;
  _tick = 1;
  _updateUI = updateUI;
  _checkMidi = false;
}

Sequencer * Sequencer::instance;

void Sequencer::isrCallback()
{
  instance->callBack();
}

void Sequencer::loop() {
  if (_checkMidi) {
    
    _previousMidiData[0] = _midiData[0];
    _previousMidiData[1] = _midiData[1];
    _previousMidiData[2] = _midiData[2];
    _midiData[0] = bank1[_step];

    uint8_t midiNote = _midiData[0];
    uint8_t previousMidiNote = _previousMidiData[0];
    
    DEBUG_PRINT(millis());
    DEBUG_PRINTLN(" send midi");

    DEBUG_PRINT("midiNote ");
    DEBUG_PRINTLN(midiNote);

    DEBUG_PRINT("previousMidiNote ");
    DEBUG_PRINTLN(previousMidiNote);
    
    if(midiNote == previousMidiNote) {
      // If the data is the same, don't send anything
    } else if(midiNote == 0 && previousMidiNote > 0) {
      // Send a note off for the previous note
      writeSerialMidi(NOTE_OFF, previousMidiNote, VELOCITY_OFF);             
    } else if(midiNote > 0 && previousMidiNote > 0) {
      // Send a note off for the previous note
      writeSerialMidi(NOTE_OFF, previousMidiNote, VELOCITY_OFF);   
      // Send a note on for the current note
      writeSerialMidi(NOTE_ON, midiNote, VELOCITY_ON);     
    } else if(midiNote > 0 && previousMidiNote == 0) {
      // Send a note on for the current note
      writeSerialMidi(NOTE_ON, midiNote, VELOCITY_ON);     
    }
    DEBUG_PRINT(millis());
    DEBUG_PRINTLN(" after send midi");
    // Do serial midi
    _checkMidi = false;
  }
}

void Sequencer::callBack() {
  // With a _ppqn set to 16 every tick is a note spot
  _tick--;
  if (_tick == 0) {
    nextStep();
    DEBUG_PRINTLN(_step);
    DEBUG_PRINT(millis());
    DEBUG_PRINTLN(" _notePulse");
    // Handle note every 250 milliseconds
    _tick = _notePulse;
    _checkMidi = true;
  }
}

Sequencer::init() {
  instance = this;

  // Calculate the correct settings for the compare match register
  this->calculateInterval();

  // CPU frequency 16Mhz for Arduino
  // maximum timer counter value (256 for 8bit, 65536 for 16bit timer)
  // Divide CPU frequency through the choosen prescaler (16000000 / 256 = 62500)
  // Divide result through the desired frequency (62500 / 2Hz = 31250)
  // Verify the result against the maximum timer counter value (31250 < 65536 success) if fail, choose bigger prescaler.
  // TTCCRx - Timer/Counter Control Register. The prescaler can be configured here.
  // TCNTx - Timer/Counter Register. The actual timer value is stored here.
  // OCRx - Output Compare Register
  // ICRx - Input Capture Register (only for 16bit timer)
  // TIMSKx - Timer/Counter Interrupt Mask Register. To enable/disable timer interrupts.
  // TIFRx - Timer/Counter Interrupt Flag Register. Indicates a pending timer interrupt.
  // https://www.robotshop.com/community/forum/t/arduino-101-timers-and-interrupts/13072

  // Set up interrupt routine
  noInterrupts();
d  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  OCR3A = _prescalerCompare;  // compare match register 16MHz/256/2Hz
  TCCR3B |= (1 << WGM12);     // CTC mode
  TCCR3B |= (1 << CS12);      // 256 prescaler
  //TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt in setMode
  interrupts();               // enable all interrupts
}

uint8_t Sequencer::getMode() {
  return _mode;
}

uint8_t Sequencer::getStep() {
  return _step;
}

bool Sequencer::setMidiNote(uint8_t note) {
  if (_mode != 0) {
    return false;
  }
  bank1[_step] = note;
  return true;
}

uint8_t Sequencer::getNote() {
  return bank1[_step];
}

uint8_t Sequencer::getTempo() {
  return _bpm;
}

void Sequencer::nextStep() {
  if (_step == 15) {
    _step = 0;
  } else {
    _step++;
  }
  *_updateUI = true;
}

void Sequencer::setMode(uint8_t mode) {
  _mode = mode;
  if (mode == 1) {
    noInterrupts();
    TIMSK3 |= (1 << OCIE3A);
    interrupts();
  } else {
    noInterrupts();
    TIMSK3 &= ~(1 << OCIE3A);
    interrupts();
  }
}

void Sequencer::calculateInterval() {
  // The frequency of the chosen bpm rate
  // Example frequencies
  // 100 = 250 * 24 / 60
  // 80 = 200 * 24 / 60
  // 24 = 60 * 24 / 60
  // 12 = 30 * 24 / 60
  // 3 = 8 * 24 / 60
  // 2 = 7 * 24 / 60
  // 2 = 6 * 24 / 60
  // 2 = 5 * 24 / 60
  // 1 = 4 * 24 / 60
  // 1 = 3 * 24 / 60
  _frequency = _bpm * _ppqn / 60;

  // Initialize the interval in milliseconds
  // CPU frequency / prescaler = prescaler space
  // 16000000 / 256 = 62500     62500 / 2Hz = 31250     62500 / 100Hz = 625
  // 16000000 / 128 = 125000    125000 / 2Hz = 62500    125000 / 100Hz = 1250
  // 16000000 / 64 = 250000     250000 / 2Hz = 125000   250000 / 100Hz = 2500
  // 16000000 / 32 = 500000     500000 / 2Hz = 250000   500000 / 100Hz = 5000
  _prescalerCompare = 62500 / _frequency;
}

void Sequencer::setTempo(unsigned int bpm) {
  _bpm = bpm;
  calculateInterval();
  noInterrupts();
  OCR3A = _prescalerCompare;
  interrupts();
}

void Sequencer::writeSerialMidi(uint8_t cmd, uint8_t note, uint8_t velocity) {
  Serial1.write(cmd);
  Serial1.write(note);
  Serial1.write(velocity);
  DEBUG_PRINT(cmd == NOTE_OFF ? "OFF " : "ON ");
  DEBUG_PRINT("cmd ");
  DEBUG_PRINTLN(cmd);
  DEBUG_PRINT("note ");
  DEBUG_PRINTLN(note);
  DEBUG_PRINT("velocity ");
  DEBUG_PRINTLN(velocity);
}
