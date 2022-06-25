#include "MenuController.h"

MenuController::MenuController(DisplayController* displayController, SequencerEngine* sequencerEngine) {
	_displayController = displayController;
	_sequencerEngine = sequencerEngine;
	manipulateMenuLine(_sequencerEngine->getTempo(), 1, 4);
	manipulateMenuLine(_sequencerEngine->getStep(), 2, 5);
}

void MenuController::startMenu(uint8_t menuIndex) {
	_activeMenuIndex = menuIndex;
	_displayController->startDisplayOutput();
	for (uint8_t i = 0; i < _numberOfMenuItems[_activeMenuIndex]; i++) {
		if (i == _activeMenuItemIndex) {
			if (_itemSelected) {
				_displayController->printSelectedMenuLine(_menuItems[_activeMenuIndex][i]);
			}
			else {
				_displayController->printActiveMenuLine(_menuItems[_activeMenuIndex][i]);
			}
		}
		else {
			_displayController->printMenuLine(_menuItems[_activeMenuIndex][i]);
		}
	}
	_displayController->stopDisplayOutput();
}

void MenuController::setActiveMenuIndex(uint8_t menuIndex)
{
	_activeMenuIndex = menuIndex;
}

void MenuController::cycleMenuPlus() {
	if (_itemSelected) {
		cycleActiveMenuPlus();
	}
	else {
		_activeMenuItemIndex++;
		if (_activeMenuItemIndex >= _numberOfMenuItems[_activeMenuIndex]) {
			_activeMenuItemIndex = 0;
		}
		startMenu(_activeMenuIndex);
	}
}

void MenuController::cycleMenuMin() {
	if (_itemSelected) {
		cycleActiveMenuMin();
	}
	else {
		_activeMenuItemIndex--;
		if (_activeMenuItemIndex < 0) {
			_activeMenuItemIndex = _numberOfMenuItems[_activeMenuIndex] - 1;
		}
		startMenu(_activeMenuIndex);
	}
}

void MenuController::cycleActiveMenuMin() {
	if (_activeMenuItemIndex == 0) {
		handleStartStop();
	}
	else if (_activeMenuItemIndex == 1) {
		handleBPM(-1);
	}
}

void MenuController::cycleActiveMenuPlus() {
	if (_activeMenuItemIndex == 0) {
		handleStartStop();
	}
	else if (_activeMenuItemIndex == 1) {
		handleBPM(1);
	}
}

void MenuController::clickMenu() {
	_itemSelected = !_itemSelected;
	startMenu(_activeMenuIndex);
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
	startMenu(_activeMenuIndex);
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
	startMenu(_activeMenuIndex);
}

void MenuController::handleStep() {
	uint8_t step = _sequencerEngine->getStep();
	manipulateMenuLine(step, 2, 5);
	startMenu(_activeMenuIndex);
}

void MenuController::manipulateMenuLine(uint8_t value, uint8_t menuIndex, uint8_t startIndex)
{
	char s[4];
	sprintf(s, "%i", value);
	_menuItems[0][menuIndex][startIndex] = s[0];
	_menuItems[0][menuIndex][startIndex+1] = s[1];
	_menuItems[0][menuIndex][startIndex+2] = s[2];
}

void MenuController::manipulateStartStopLine(char characters[8]) {
	_menuItems[0][0][0] = characters[0];
	_menuItems[0][0][1] = characters[1];
	_menuItems[0][0][2] = characters[2];
	_menuItems[0][0][3] = characters[3];
	_menuItems[0][0][4] = characters[4];
	_menuItems[0][0][5] = characters[5];
	_menuItems[0][0][6] = characters[6];
}

