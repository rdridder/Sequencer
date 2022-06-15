#ifndef _MENUCONTROLLER_h
#define _MENUCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "DisplayController.h";
#include "SequencerEngine.h";

class MenuController
{
	public:
		MenuController(DisplayController *displayController, SequencerEngine *sequencerEngine);
		void startMenu();
		void clickMenu();
		void cycleMenuPlus();
		void cycleMenuMin();
		void cycleActiveMenuMin();
		void cycleActiveMenuPlus();

	protected:

	private:
		DisplayController* _displayController;
		SequencerEngine* _sequencerEngine;
		char _started[8] = "started";
		char _stopped[8] = "stopped";
		char* _menuItems[21] = { "started              ", "set bpm              "};
		uint8_t _numberOfMenuItems = 2;
		int8_t _activeIndex = 0;
		bool _isStarted = false;
		bool _itemActive = true;
		void handleStart();
		void manipulateStartStopLine(char characters[8]);
};





#endif

