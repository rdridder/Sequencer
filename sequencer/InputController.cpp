#include "InputController.h"

InputController::InputController(void (*callbackMethodArg)(int encoderIndex, long encoderValue))
{
	_encoders[0] = new RotaryEncoder(52, 50, RotaryEncoder::LatchMode::FOUR0);
	_encoders[1] = new RotaryEncoder(48, 46, RotaryEncoder::LatchMode::FOUR0);
	_encoders[2] = new RotaryEncoder(44, 42, RotaryEncoder::LatchMode::FOUR0);
	_encoders[3] = new RotaryEncoder(40, 38, RotaryEncoder::LatchMode::FOUR0);
	_encoders[4] = new RotaryEncoder(36, 34, RotaryEncoder::LatchMode::FOUR0);
	_encoders[5] = new RotaryEncoder(32, 30, RotaryEncoder::LatchMode::FOUR0);
	_encoders[6] = new RotaryEncoder(28, 26, RotaryEncoder::LatchMode::FOUR0);
	_encoders[7] = new RotaryEncoder(24, 22, RotaryEncoder::LatchMode::FOUR0);
	_encoders[8] = new RotaryEncoder(2, 3, RotaryEncoder::LatchMode::FOUR0);
	callbackMethod = callbackMethodArg;
}

void InputController::tick(long currentMillis) {
	if (currentMillis - _previousMillis >= ENCODER_POLL_INTERVAL_MS) {
		for (int i = 0; i < NUM_ENCODERS; i++) {
			_encoders[i]->tick();
			long newPos = _encoders[i]->getPosition();
			if (_positions[i] != newPos) {
				_positions[i] = newPos;
				callbackMethod(i, newPos);
			}
		}
	}
}
