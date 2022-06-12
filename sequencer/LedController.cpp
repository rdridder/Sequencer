#include "LedController.h"

LedController::LedController() {
	pinMode(LED_SHIFT_LATCH_PIN, OUTPUT);
	pinMode(LED_SHIFT_DATA_PIN, OUTPUT);
	pinMode(LED_SHIFT_CLOCK_PIN, OUTPUT);
}

void LedController::setup() {
	setLedState(0);
}

void LedController::setLedState(unsigned long data) {
	int shift = data & ((1 << 8) - 1);
	digitalWrite(LED_SHIFT_LATCH_PIN, LOW);
	shiftOut(LED_SHIFT_DATA_PIN, LED_SHIFT_CLOCK_PIN, MSBFIRST, shift);
	digitalWrite(LED_SHIFT_LATCH_PIN, HIGH);
}

