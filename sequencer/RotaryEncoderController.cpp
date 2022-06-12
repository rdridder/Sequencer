#include "RotaryEncoderController.h"

RotaryEncoderController::RotaryEncoderController(void (*callbackMethodArg)(int encoderIndex, long encoderValue))
{
	_encoders[0] = &_encoder1;
	_encoders[1] = &_encoder2;
	_encoders[2] = &_encoder3;
	_encoders[3] = &_encoder4;
	_encoders[4] = &_encoder5;
	_encoders[5] = &_encoder6;
	_encoders[6] = &_encoder7;
	_encoders[7] = &_encoder8;
	_encoders[8] = &_encoder9;
	callbackMethod = callbackMethodArg;
}

void RotaryEncoderController::tick(long currentMillis) {
	if (currentMillis - _previousMillis >= ENCODER_POLL_INTERVAL_MS) {	
		_previousMillis = currentMillis;
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
