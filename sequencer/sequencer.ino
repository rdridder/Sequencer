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
volatile bool updateMenu = false;
volatile uint8_t currentStep = 0;

void setup() {
	Serial.begin(115200);

	// Set I2C speed
	//Wire.setClock(I2C_SPEED);
	
	// Initialize controllers
	rotaryEncoderController = new RotaryEncoderController(&rotaryEncoderCallback);
	shiftRegisterController = new ShiftRegisterController(&buttonCallback, BUTT_SHIFT_NUMBER_OF_BUTTONS);
	displayController = new DisplayController(BUTT_SHIFT_NUMBER_OF_BUTTONS);
	ledController = new LedController();
	sequencerEngine = new SequencerEngine(SEQ_START_BPM, SEQ_NUMBER_OF_STEPS, &uiCallback);
	sequencerEngine->init();

	menuController = new MenuController(displayController, sequencerEngine);

	// Setup controllers
	shiftRegisterController->setup();
	displayController->setup();
	ledController->setup();

	// Start with the starting UI
	menuController->startMenu(0);
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
		menuController->setActiveMenuIndex(0);
		if (direction < 0) {
			menuController->cycleActiveMenuMin();
		}
		else {
			menuController->cycleActiveMenuPlus();
		}
	}
	else {
		menuController->startMenu(1);
	}
}

void rotaryMainButtonCallback() {
	if (!menuController->startMenu(0)) {
		menuController->cycleMenuPlus();
	}
}

void buttonCallback(unsigned long buttonValues) {
	if (buttonValues > 128) {
		if (!menuController->startMenu(1)) {
			menuController->cycleMenuPlus();
		}
	}
	else if (buttonValues <= 128 && buttonValues > 0) {
		displayController->printButtonValues(buttonValues);
		ledController->setLedState(buttonValues);
	}
}

void uiCallback(uint8_t step) {
	updateUi = true;
	currentStep = step;
}