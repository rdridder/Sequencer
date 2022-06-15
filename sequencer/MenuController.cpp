#include "MenuController.h"

MenuController::MenuController(DisplayController* displayController) {
	_displayController = displayController;
}

void MenuController::startMenu() {
	_displayController->startDisplayOutput();
	for (uint8_t i = 0; i < _numberOfMenuItems; i++) {
		if (i == _activeItem) {
			_displayController->printActiveMenuLine(_menuItems[i]);
		}
		else {
			_displayController->printMenuLine(_menuItems[i]);
		}
	}
	_displayController->stopDisplayOutput();
}

void MenuController::cycleMenuPlus() {
	_activeItem++;
	if (_activeItem >= _numberOfMenuItems) {
		_activeItem = 0;
	}
	startMenu();
}

void MenuController::cycleMenuMin() {
	_activeItem--;
	if (_activeItem < 0) {
		_activeItem = _numberOfMenuItems - 1;
	}
	startMenu();
}

void MenuController::clickMenu() {
}