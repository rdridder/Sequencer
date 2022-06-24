#include "MenuController.h"

MenuController::MenuController(DisplayController* displayController, SequencerEngine* sequencerEngine) {
	_displayController = displayController;
	_sequencerEngine = sequencerEngine;
	manipulateBPMLine(_sequencerEngine->getTempo());
	manipulateStepLine(_sequencerEngine->getStep());
}

void MenuController::startMenu() {
	_displayController->startDisplayOutput();
	for (uint8_t i = 0; i < _numberOfMenuItems; i++) {
		if (i == _activeIndex) {
			if (_itemSelected) {
				_displayController->printSelectedMenuLine(_menuItems[i]);
			}
			else {
				_displayController->printActiveMenuLine(_menuItems[i]);
			}
		}
		else {
			_displayController->printMenuLine(_menuItems[i]);
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
		if (_activeIndex >= _numberOfMenuItems) {
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
			_activeIndex = _numberOfMenuItems - 1;
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
	manipulateBPMLine(bpm);
	startMenu();
}

void MenuController::handleStep() {
	uint8_t step = _sequencerEngine->getStep();
	////manipulateStepLine(step);
	////_displayController->startDisplayOutput();

	//_displayController->setRowColumn(3,5);
	//char s[4];
	//sprintf(s, "%u", ++step);
	//_displayController->print(s);
	//_displayController->stopDisplayOutput();
	manipulateStepLine(step);
	startMenu();
}

void MenuController::manipulateBPMLine(uint8_t bpm) {
	char s[4];
	sprintf(s, "%i", bpm);
	_menuItems[1][4] = s[0];
	_menuItems[1][5] = s[1];
	_menuItems[1][6] = s[2];
}

void MenuController::manipulateStepLine(uint8_t step) {
	char s[4];
	sprintf(s, "%i", ++step);
	_menuItems[2][5] = s[0];
	_menuItems[2][6] = s[1];
}

void MenuController::manipulateStartStopLine(char characters[8]) {
	_menuItems[0][0] = characters[0];
	_menuItems[0][1] = characters[1];
	_menuItems[0][2] = characters[2];
	_menuItems[0][3] = characters[3];
	_menuItems[0][4] = characters[4];
	_menuItems[0][5] = characters[5];
	_menuItems[0][6] = characters[6];
}

