#ifndef _MENUCONTROLLER_h
#define _MENUCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "DisplayController.h";

class MenuController
{
	public:
		MenuController(DisplayController *displayController);
		void startMenu();
		void clickMenu();
		void cycleMenuPlus();
		void cycleMenuMin();

	protected:

	private:
		DisplayController* _displayController;
		char* _menuItems[21] = { "start sequencer      ", "stop sequencer       "};
		uint8_t _numberOfMenuItems = 2;
		int8_t _activeItem = 0;
};





#endif

