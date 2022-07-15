#include "ShiftRegisterController.h"

ShiftRegisterController::ShiftRegisterController(void (*callbackMethodArg)(unsigned long buttonValues), int numberOfButtons)
{
	callbackMethod = callbackMethodArg;
	_numberOfButtons = numberOfButtons;
}

void ShiftRegisterController::setup() {
	pinMode(BUTT_SHIFT_LOAD_PIN, OUTPUT);
	pinMode(BUTT_SHIFT_CLK_ENABLE_PIN, OUTPUT);
	pinMode(BUTT_SHIFT_CLK_PIN, OUTPUT);
	pinMode(BUTT_SHIFT_DATA_PIN, INPUT);
}

void ShiftRegisterController::loop(unsigned long currentMillis) {
	if (currentMillis - _previousMillis < BUTT_SHIFT_POLL_INTERVAL_MS) {
		return;
	}
	_previousMillis = currentMillis;
	long bitVal;
	unsigned long bytesVal = 0;

	// 6 clk/inh BUTT_SHIFT_CLK_ENABLE_PIN	enablePin
	// 7 clk BUTT_SHIFT_CLK_PIN				clockPin
	// 5 ser_in BUTT_SHIFT_DATA_PIN			dataPin
	// 8 sh/ld BUTT_SHIFT_LOAD_PIN			LoadPin

	/* Trigger a parallel Load to latch the state of the data lines,
	*/
	digitalWrite(BUTT_SHIFT_CLK_ENABLE_PIN, HIGH);
	digitalWrite(BUTT_SHIFT_LOAD_PIN, LOW);
	digitalWrite(BUTT_SHIFT_LOAD_PIN, HIGH);
	digitalWrite(BUTT_SHIFT_CLK_ENABLE_PIN, LOW);

	for (int i = 0; i < _numberOfButtons; i++)
	{
		bitVal = digitalRead(BUTT_SHIFT_DATA_PIN);
		bytesVal |= (bitVal << ((_numberOfButtons - 1) - i));

		digitalWrite(BUTT_SHIFT_CLK_PIN, HIGH);
		digitalWrite(BUTT_SHIFT_CLK_PIN, LOW);
	}
	_pinValues = bytesVal;

	if (_pinValues != _oldPinValues)
	{
		_oldPinValues = _pinValues;
		callbackMethod(_pinValues);
	}
}


