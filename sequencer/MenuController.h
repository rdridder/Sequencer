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
		void startMenu(uint8_t menuIndex);
		void setActiveMenuIndex(uint8_t menuIndex);
		void clickMenu();
		void cycleMenuPlus();
		void cycleMenuMin();
		void cycleActiveMenuMin();
		void cycleActiveMenuPlus();
		void handleStep();

	protected:

	private:
		DisplayController* _displayController;
		SequencerEngine* _sequencerEngine;
		char _started[8] = "started";
		char _stopped[8] = "stopped";
		uint8_t _activeMenuIndex = 0;		
		char *_menuItems[2][21] = { 
			{ "stopped              ", "bpm                  ", "step                 " }, 
			{ "note                 ", "length               ", "velocity 100         " , "octave 0             ", "step                 " } 
		};
		uint8_t const _numberOfMenuItems[2] = {3, 5};
		int8_t _activeMenuItemIndex = 0;
		bool _isStarted = false;
		bool _itemSelected = true;
		void handleStartStop();
		void handleBPM(int direction);
		void manipulateStartStopLine(char characters[8]);
		void manipulateMenuLine(uint8_t value, uint8_t menuIndex, uint8_t startIndex);
};





#endif

