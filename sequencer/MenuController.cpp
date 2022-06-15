#include "MenuController.h"

MenuController::MenuController(DisplayController* displayController, SequencerEngine* sequencerEngine) {
	_displayController = displayController;
	_sequencerEngine = sequencerEngine;
}

void MenuController::startMenu() {
	_displayController->startDisplayOutput();
	for (uint8_t i = 0; i < _numberOfMenuItems; i++) {
		if (i == _activeIndex) {
			_displayController->printActiveMenuLine(_menuItems[i]);
		}
		else {
			_displayController->printMenuLine(_menuItems[i]);
		}
	}
	uint8_t bpm = _sequencerEngine->getTempo();
	_displayController->printBPM(bpm);
	_displayController->stopDisplayOutput();
}

void MenuController::cycleMenuPlus() {
	if (_itemActive) {
		cycleActiveMenuPlus();
	}
	else {
		_activeIndex++;
		if (_activeIndex >= _numberOfMenuItems) {
			_activeIndex = 0;
		}
		startMenu();
	}
}

void MenuController::cycleMenuMin() {
	if (_itemActive) {
		cycleActiveMenuMin();
	}
	else {
		_activeIndex--;
		if (_activeIndex < 0) {
			_activeIndex = _numberOfMenuItems - 1;
		}
		startMenu();
	}
}

void MenuController::cycleActiveMenuMin() {
	if (_activeIndex == 0) {
		handleStart();
	}
}

void MenuController::cycleActiveMenuPlus() {
	if (_activeIndex == 0) {
		handleStart();
	}
}

void MenuController::clickMenu() {
	_itemActive = !_itemActive;
}

void MenuController::handleStart() {
	if (_isStarted) {
		manipulateStartStopLine(_started);
		_sequencerEngine->setMode(0);
	}
	else {
		manipulateStartStopLine(_stopped);
		_sequencerEngine->setMode(1);
	}
	_isStarted = !_isStarted;
	startMenu();
}

void MenuController::manipulateStartStopLine(char characters[8]) {
	_menuItems[0][0] = characters[0];
	_menuItems[0][1] = characters[1];
	_menuItems[0][2] = characters[2];
	_menuItems[0][3] = characters[3];
	_menuItems[0][4] = characters[4];
	_menuItems[0][5] = characters[5];
	_menuItems[0][6] = characters[6];
	_menuItems[0][7] = characters[7];
}

