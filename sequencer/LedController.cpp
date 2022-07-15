#include "LedController.h"

LedController::LedController() {
}

void LedController::setup() {
	pinMode(LED_SHIFT_LATCH_PIN, OUTPUT);
	pinMode(LED_SHIFT_DATA_PIN, OUTPUT);
	pinMode(LED_SHIFT_CLOCK_PIN, OUTPUT);
	setLedState(0);
}

void LedController::setLedState(unsigned long data) {
	// Set latchpin low
	digitalWrite(LED_SHIFT_LATCH_PIN, LOW);

	// Get a reference to the bytes
	byte* arr = (byte*)&data;

	// Write the bytes to the shift register
	shiftOut(LED_SHIFT_DATA_PIN, LED_SHIFT_CLOCK_PIN, MSBFIRST, arr[0]);

#if NUMBOARDS == 2
	shiftOut(LED_SHIFT_DATA_PIN, LED_SHIFT_CLOCK_PIN, MSBFIRST, arr[1]);
#endif
	// Set latchpin high
	digitalWrite(LED_SHIFT_LATCH_PIN, HIGH);	
}

