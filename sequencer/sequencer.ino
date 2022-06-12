#include <MemoryUsage.h>
#include "ezButton.h"
#include "SequencerEngine.h"
#include "LedController.h"
#include "DisplayController.h"
#include "ShiftRegisterController.h"
#include "RotaryEncoderController.h"
#include <Wire.h>
#include "Config.h"


RotaryEncoderController *rotaryEncoderController;
ShiftRegisterController *shiftRegisterController;
DisplayController *displayController;
LedController *ledController;

ezButton button(4);


void setup() {
	Serial.begin(115200);

	// Set I2C speed
	Wire.setClock(I2C_SPEED);
	
	// Initialize controllers
	rotaryEncoderController = new RotaryEncoderController(&rotaryEncoderCallback);
	shiftRegisterController = new ShiftRegisterController(&buttonCallback);
	displayController = new DisplayController();
	ledController = new LedController();

	// Setup controllers
	shiftRegisterController->setup();
	displayController->setup();
	ledController->setup();

	// Start with the starting UI
	displayController->printStartMenu();
}

unsigned long _previousMillis = 0;

void loop() {
	unsigned long currentMillis = millis();
	rotaryEncoderController->tick(currentMillis);
	shiftRegisterController->tick(currentMillis);
	
	
	button.loop();

	if (button.isPressed()) {
		displayController->flipMenu();
	}



	
	if (currentMillis - _previousMillis >= 2000) {
		_previousMillis = currentMillis;
		FREERAM_PRINT;
	}
	

	

}

void rotaryEncoderCallback(int encoderIndex, long encoderValue) {
	displayController->printEncoderValues(encoderIndex, encoderValue);
}

void buttonCallback(unsigned long buttonValues) {
	displayController->printButtonValues(buttonValues);
	ledController->setLedState(buttonValues);
}