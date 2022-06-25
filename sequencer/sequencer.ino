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

volatile bool updateUi = false;
volatile uint8_t currentStep = 0;

void setup() {
	Serial.begin(115200);

	// Set I2C speed
	//Wire.setClock(I2C_SPEED);
	
	// Initialize controllers
	rotaryEncoderController = new RotaryEncoderController(&rotaryEncoderCallback);
	shiftRegisterController = new ShiftRegisterController(&buttonCallback);
	displayController = new DisplayController();
	ledController = new LedController();
	uint8_t bpm = 120;
	sequencerEngine = new SequencerEngine(bpm, &uiCallback);
	sequencerEngine->init();

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
	sequencerEngine->loop();

	unsigned long currentMillis = millis();
	rotaryEncoderController->loop(currentMillis);
	shiftRegisterController->loop(currentMillis);
	button.loop();
	if (button.isPressed()) {
		rotaryMainButtonCallback();
	}
	if (updateUi) {
		updateUi = false;
		uint8_t led = 0;
		led = currentStep;
		unsigned long state = 0;
		if (currentStep > 7) {
			led -= 7;
		}
		state |= (1 << led);
		ledController->setLedState(state);
		menuController->handleStep();
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

void uiCallback(uint8_t step) {
	updateUi = true;
	currentStep = step;
}