#include "DisplayController.h"

DisplayController::DisplayController() {
	_display = new Adafruit_SSD1306(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, DISPLAY_RESET);
}

void DisplayController::setup() {
	_display->begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);
	_display->clearDisplay();
	_display->setTextSize(1);
	_display->setTextColor(SSD1306_WHITE);
	_display->setCursor(0, 0);
	_display->display();
}

void DisplayController::startDisplayOutput() {
	_display->clearDisplay();
	_display->setCursor(0, 0);
}

void DisplayController::stopDisplayOutput() {
	_display->display();
}

void DisplayController::printMenuLine(char menuLine[21]) {
	menuLine[20] = ' ';
	_display->println(menuLine);
}

void DisplayController::printActiveMenuLine(char menuLine[21]) {
	menuLine[20] = '<';
	//_display->setTextColor(BLACK, WHITE); // crashes after a while
	_display->println(menuLine);
	//_display->setTextColor(WHITE, BLACK);
}

void DisplayController::printEncoderValues(int encoderIndex, long encoderValue) {
	_display->clearDisplay();
	_display->setCursor(0, 0);
	_display->print("encoder");
	_display->print(' ');
	_display->print(encoderIndex + 1);
	_display->print(' ');
	_display->setCursor(0, 10);
	_display->print(encoderValue);
	_display->print(' ');
	_display->display();
}

void DisplayController::printButtonValues(unsigned long buttonValues) {
	_display->clearDisplay();
	_display->setCursor(0, 0);
	_display->println("buttons");
	for (int i = 0; i < BUTT_SHIFT_NUMBER_OF_BUTTONS; i++)
	{
		if (i % 8 == 0) {
			_display->println();
		}
		if ((buttonValues >> i) & 1) {
			_display->print(1);
		}
		else {
			_display->print(0);
		}
		_display->print(' ');
	}
	_display->display();
}
