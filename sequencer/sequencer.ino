#include "MenuController.h"
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
SequencerEngine *sequencerEngine;
MenuController *menuController;

ezButton button(4);

volatile bool updateUI = false;

void setup() {
	Serial.begin(115200);

	// Set I2C speed
	Wire.setClock(I2C_SPEED);
	
	// Initialize controllers
	rotaryEncoderController = new RotaryEncoderController(&rotaryEncoderCallback);
	shiftRegisterController = new ShiftRegisterController(&buttonCallback);
	displayController = new DisplayController();
	ledController = new LedController();
	sequencerEngine = new SequencerEngine(120, &updateUI);
	menuController = new MenuController(displayController, sequencerEngine);

	// Setup controllers
	shiftRegisterController->setup();
	displayController->setup();
	ledController->setup();

	// Start with the starting UI
	menuController->startMenu();
}

unsigned long _previousMillis = 0;

void loop() {
	unsigned long currentMillis = millis();
	rotaryEncoderController->tick(currentMillis);
	shiftRegisterController->tick(currentMillis);
	
	button.loop();
	if (button.isPressed()) {
		rotaryMainButtonCallback();
	}

	//if (currentMillis - _previousMillis >= 2000) {
	//	_previousMillis = currentMillis;
	//	FREERAM_PRINT;
	//}
}

void rotaryEncoderCallback(int encoderIndex, long encoderValue, int direction) {
	if (NUM_ENCODERS - 1 == encoderIndex) {
		if (direction < 0) {
			menuController->cycleMenuMin();
		}
		else {
			menuController->cycleMenuPlus();
		}
	}
	else {
		displayController->printEncoderValues(encoderIndex, encoderValue);
	}
}

void rotaryMainButtonCallback() {
	menuController->clickMenu();
}

void buttonCallback(unsigned long buttonValues) {
	displayController->printButtonValues(buttonValues);
	ledController->setLedState(buttonValues);
}