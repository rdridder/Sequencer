#ifndef _SHIFTREGISTERCONTROLLER_h
#define _SHIFTREGISTERCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Config.h"

class ShiftRegisterController
{
	public:
		ShiftRegisterController(void (*callbackMethodArg)(unsigned long buttonValues), int numberOfButtons);
		void loop(unsigned long currentMillis);
		void setup();

	protected:

	private:
		long _previousMillis = 0;
		unsigned long _oldPinValues = 0;
		unsigned long _pinValues = 0;
		int _numberOfButtons;
		byte* _readValues = { 0 };
		void (*callbackMethod)(unsigned long buttonValues);
};

#endif

