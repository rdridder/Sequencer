#ifndef _INPUTCONTROLLER_h
#define _INPUTCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <RotaryEncoder.h>
#include "Config.h"

class InputController
{
	public:
		InputController(void (*callbackMethod)(int encoderIndex, long encoderValue));
		void tick(long currentMillis);

	protected:

	private:
		RotaryEncoder* _encoders[NUM_ENCODERS];
		long _previousMillis;
		long _positions[NUM_ENCODERS] = { 0 };
		void (*callbackMethod)(int encoderIndex, long encoderValue);
};

#endif