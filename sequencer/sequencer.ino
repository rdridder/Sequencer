#include "InputController.h"
#include <Wire.h>
#include "Config.h"

InputController *inputController;
unsigned long currentMillis;

void setup() {
	Serial.begin(115200);

	// Set I2C speed
	Wire.setClock(I2C_SPEED);

	inputController = new InputController(&callback);

}

void loop() {
	currentMillis = millis();
	inputController->tick(currentMillis);
}

void callback(int encoderIndex, long encoderValue) {
	Serial.print(encoderIndex);
	Serial.print(" - ");
	Serial.println(encoderValue);
}