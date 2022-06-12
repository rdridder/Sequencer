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
		RotaryEncoderController(void (*callbackMethodArg)(int encoderIndex, long encoderValue));
		void tick(unsigned long currentMillis);

	protected:

	private:
		RotaryEncoder* _encoders[NUM_ENCODERS];
		long _previousMillis = 0;
		long _positions[NUM_ENCODERS] = { 0 };
		void (*callbackMethod)(int encoderIndex, long encoderValue);
		RotaryEncoder _encoder1 = RotaryEncoder(52, 50, RotaryEncoder::LatchMode::FOUR0);
		RotaryEncoder _encoder2 = RotaryEncoder(48, 46, RotaryEncoder::LatchMode::FOUR0);
		RotaryEncoder _encoder3 = RotaryEncoder(44, 42, RotaryEncoder::LatchMode::FOUR0);
		RotaryEncoder _encoder4 = RotaryEncoder(40, 38, RotaryEncoder::LatchMode::FOUR0);
		RotaryEncoder _encoder5 = RotaryEncoder(36, 34, RotaryEncoder::LatchMode::FOUR0);
		RotaryEncoder _encoder6 = RotaryEncoder(32, 30, RotaryEncoder::LatchMode::FOUR0);
		RotaryEncoder _encoder7 = RotaryEncoder(28, 26, RotaryEncoder::LatchMode::FOUR0);
		RotaryEncoder _encoder8 = RotaryEncoder(24, 22, RotaryEncoder::LatchMode::FOUR0);
		RotaryEncoder _encoder9 = RotaryEncoder(2, 3, RotaryEncoder::LatchMode::FOUR0);
};

#endif