#include "ShiftRegisterController.h"

ShiftRegisterController::ShiftRegisterController(void (*callbackMethodArg)(unsigned long buttonValues))
{
	callbackMethod = callbackMethodArg;
}

void ShiftRegisterController::setup() {
	pinMode(BUTT_SHIFT_LOAD_PIN, OUTPUT);
	pinMode(BUTT_SHIFT_CLK_ENABLE_PIN, OUTPUT);
	pinMode(BUTT_SHIFT_CLK_PIN, OUTPUT);
	pinMode(BUTT_SHIFT_DATA_PIN, INPUT);
	digitalWrite(BUTT_SHIFT_CLK_PIN, LOW);
	digitalWrite(BUTT_SHIFT_LOAD_PIN, HIGH);
}

void ShiftRegisterController::loop(unsigned long currentMillis) {
	if (currentMillis - _previousMillis < BUTT_SHIFT_POLL_INTERVAL_MS) {
		return;
	}
	_previousMillis = currentMillis;
	long bitVal;
	unsigned long bytesVal = 0;

	/* Trigger a parallel Load to latch the state of the data lines,
	*/
	digitalWrite(BUTT_SHIFT_CLK_ENABLE_PIN, HIGH);
	digitalWrite(BUTT_SHIFT_LOAD_PIN, LOW);
	digitalWrite(BUTT_SHIFT_LOAD_PIN, HIGH);
	digitalWrite(BUTT_SHIFT_CLK_ENABLE_PIN, LOW);

	/* Loop to read each bit value from the serial out line
		* of the SN74HC165N.
	*/
	for (int i = 0; i < BUTT_SHIFT_NUMBER_OF_BUTTONS; i++)
	{
		bitVal = digitalRead(BUTT_SHIFT_DATA_PIN);

		/* Set the corresponding bit in bytesVal.
		*/
		bytesVal |= (bitVal << ((BUTT_SHIFT_NUMBER_OF_BUTTONS - 1) - i));

		/* Pulse the Clock (rising edge shifts the next bit).
		*/
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


