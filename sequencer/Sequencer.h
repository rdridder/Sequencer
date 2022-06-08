#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"

#define NOTE_ON 144
#define NOTE_OFF 128
#define VELOCITY_ON 100
#define VELOCITY_OFF 0

class Sequencer
{
  public:
    Sequencer(unsigned int bpm, bool *updateUI);
    void init();
    void setTempo(unsigned int bpm);
    void setMode(uint8_t mode);
    bool setMidiNote(uint8_t note);
    uint8_t getTempo();
    uint8_t getMode();
    uint8_t getNote();
    uint8_t getStep();
    void nextStep();
    void writeSerialMidi(uint8_t cmd, uint8_t note, uint8_t velocity);
    void loop();
    static void isrCallback();
    static Sequencer* instance;

  private:
    uint8_t bank1[16] = {32,0,32,0,32,0,32,0,32,0,32,0,32,0,32,0};
    bool *_updateUI;
    volatile bool _checkMidi;

    uint8_t _midiData[3] = {0, 0, 0};
    uint8_t _previousMidiData[3] = {0, 0, 0};

    uint8_t _step = 0;
    uint8_t _mode = 0;
    unsigned int _ppqn = 16;
    unsigned int _notePulse = _ppqn / 4;
    uint8_t _tick = 0;
    unsigned int _frequency;
    unsigned int _bpm;
    unsigned long _prescalerCompare;
    void callBack();
    void calculateInterval();
};

#endif
