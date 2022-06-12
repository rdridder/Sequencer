#include "LedController.h"
#include <splash.h>
#include "DisplayController.h"
#include "ShiftRegisterController.h"
#include "RotaryEncoderController.h"
#include <Wire.h>
#include "Config.h"

RotaryEncoderController *rotaryEncoderController;
ShiftRegisterController *shiftRegisterController;
DisplayController *displayController;
LedController *ledController;

unsigned long currentMillis;

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

	Serial.println("setup done");
}

void loop() {
	currentMillis = millis();
	rotaryEncoderController->tick(currentMillis);
	shiftRegisterController->tick(currentMillis);
}

void rotaryEncoderCallback(int encoderIndex, long encoderValue) {
	displayController->printEncoderValues(encoderIndex, encoderValue);
}

void buttonCallback(unsigned long buttonValues) {
	displayController->printButtonValues(buttonValues);
	ledController->setLedState(buttonValues);
}