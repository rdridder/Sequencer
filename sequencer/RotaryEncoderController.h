#ifndef _ROTARYENCODERCONTROLLER_h
#define _ROTARYENCODERCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <RotaryEncoder.h>
#include "Config.h"

class RotaryEncoderController
{
	public:
		RotaryEncoderController(void (*callbackMethodArg)(int encoderIndex, long encoderValue, int direction));
		void loop(unsigned long currentMillis);

	protected:

	private:
		RotaryEncoder* _encoders[NUM_ENCODERS];
		long _previousMillis = 0;
		long _positions[NUM_ENCODERS] = { 0 };
		void (*callbackMethod)(int encoderIndex, long encoderValue, int direction);
};

#endif