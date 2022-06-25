#include "MenuController.h"

MenuController::MenuController(DisplayController* displayController, SequencerEngine* sequencerEngine) {
	_displayController = displayController;
	_sequencerEngine = sequencerEngine;
	manipulateMenuLine(_sequencerEngine->getTempo(), 1, 4);
	manipulateMenuLine(_sequencerEngine->getStep(), 2, 5);
}

void MenuController::startMenu() {
	_displayController->startDisplayOutput();
	for (uint8_t i = 0; i < _numberOfMainMenuItems; i++) {
		if (i == _activeIndex) {
			if (_itemSelected) {
				_displayController->printSelectedMenuLine(_mainMenuItems[i]);
			}
			else {
				_displayController->printActiveMenuLine(_mainMenuItems[i]);
			}
		}
		else {
			_displayController->printMenuLine(_mainMenuItems[i]);
		}
	}
	_displayController->stopDisplayOutput();
}

void MenuController::cycleMenuPlus() {
	if (_itemSelected) {
		cycleActiveMenuPlus();
	}
	else {
		_activeIndex++;
		if (_activeIndex >= _numberOfMainMenuItems) {
			_activeIndex = 0;
		}
		startMenu();
	}
}

void MenuController::cycleMenuMin() {
	if (_itemSelected) {
		cycleActiveMenuMin();
	}
	else {
		_activeIndex--;
		if (_activeIndex < 0) {
			_activeIndex = _numberOfMainMenuItems - 1;
		}
		startMenu();
	}
}

void MenuController::cycleActiveMenuMin() {
	if (_activeIndex == 0) {
		handleStartStop();
	}
	else if (_activeIndex == 1) {
		handleBPM(-1);
	}
}

void MenuController::cycleActiveMenuPlus() {
	if (_activeIndex == 0) {
		handleStartStop();
	}
	else if (_activeIndex == 1) {
		handleBPM(1);
	}
}

void MenuController::clickMenu() {
	_itemSelected = !_itemSelected;
	startMenu();
}

void MenuController::handleStartStop() {
	if (_isStarted) {
		manipulateStartStopLine(_stopped);
		_sequencerEngine->setMode(0);
	}
	else {
		manipulateStartStopLine(_started);
		_sequencerEngine->setMode(1);
	}
	_isStarted = !_isStarted;
	startMenu();
}

void MenuController::handleBPM(int direction ) {
	uint8_t bpm = _sequencerEngine->getTempo();
	if (direction < 0) {
		bpm -= 2;
	}
	else {
		bpm += 2;
	}
	_sequencerEngine->setTempo(bpm);
	manipulateMenuLine(bpm, 1, 4);
	startMenu();
}

void MenuController::handleStep() {
	uint8_t step = _sequencerEngine->getStep();
	manipulateMenuLine(step, 2, 5);
	startMenu();
}

void MenuController::manipulateMenuLine(uint8_t value, uint8_t menuIndex, uint8_t startIndex)
{
	char s[4];
	sprintf(s, "%i", value);
	_mainMenuItems[menuIndex][startIndex] = s[0];
	_mainMenuItems[menuIndex][startIndex+1] = s[1];
	_mainMenuItems[menuIndex][startIndex+2] = s[2];
}

void MenuController::manipulateStartStopLine(char characters[8]) {
	_mainMenuItems[0][0] = characters[0];
	_mainMenuItems[0][1] = characters[1];
	_mainMenuItems[0][2] = characters[2];
	_mainMenuItems[0][3] = characters[3];
	_mainMenuItems[0][4] = characters[4];
	_mainMenuItems[0][5] = characters[5];
	_mainMenuItems[0][6] = characters[6];
}

