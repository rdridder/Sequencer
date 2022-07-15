#ifndef _SEQUENCERENGINE_h
#define _SEQUENCERENGINE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Config.h"
#include "SequencerStep.h"

#define NOTE_ON 144
#define NOTE_OFF 128
#define VELOCITY_ON 100
#define VELOCITY_OFF 0

class SequencerEngine
{
public:
    SequencerEngine(uint8_t bpm, uint8_t numberOfSteps, void (*uiCallbackMethodArg)(uint8_t step));
    void init();
    void setTempo(uint8_t bpm);
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
    static SequencerEngine* instance;

private:
    sequencerStep* bank1[SEQ_NUMBER_OF_STEPS];
    volatile bool* _updateUI;
    volatile bool _checkMidi;
    uint8_t _midiData[3] = { 0, 0, 0 };
    uint8_t _previousMidiData[3] = { 0, 0, 0 };
    uint8_t _step = 0;
    uint8_t _mode = 0;
    uint8_t _numberOfSteps;
    uint8_t _stepCompareValue;
    unsigned int _ppqn = 16;
    unsigned int _notePulse = _ppqn / 4;
    uint8_t _tick = 0;
    unsigned int _frequency;
    uint8_t _bpm;
    unsigned long _prescalerCompare;
    void (*uiCallbackMethod)(uint8_t step);
    void callBack();
    void calculateInterval();
};

#endif

