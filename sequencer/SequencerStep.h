// SequencerStep.h

#ifndef _SEQUENCERSTEP_h
#define _SEQUENCERSTEP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

typedef struct SequencerStep {
	uint8_t note;
	uint8_t length;
	uint8_t velocity;
	uint8_t transpose;
} sequencerStep;

#endif

