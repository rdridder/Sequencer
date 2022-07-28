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
		bool startMenu(uint8_t menuIndex);
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
		char *_menuItems[9][21] = { 
			{ "stopped              ", "bpm                  ", "step                 " }, 
			{ "note 1               ", "length               ", "velocity 100         " , "octave 0             ", "step                 " },
			{ "note 2               ", "length               ", "velocity 100         " , "octave 0             ", "step                 " },
			{ "note 3               ", "length               ", "velocity 100         " , "octave 0             ", "step                 " },
			{ "note 4               ", "length               ", "velocity 100         " , "octave 0             ", "step                 " },
			{ "note 5               ", "length               ", "velocity 100         " , "octave 0             ", "step                 " },
			{ "note 6               ", "length               ", "velocity 100         " , "octave 0             ", "step                 " },
			{ "note 7               ", "length               ", "velocity 100         " , "octave 0             ", "step                 " },
			{ "note 8               ", "length               ", "velocity 100         " , "octave 0             ", "step                 " }
		};
		uint8_t const _numberOfMenuItems[9] = {3, 5, 5, 5, 5, 5, 5, 5, 5};
		int8_t _activeMenuItemIndex[9] = { 0 };
		bool _isStarted = false;
		bool _itemSelected = true;
		void handleStartStop();
		void handleBPM(int direction);
		void manipulateStartStopLine(char characters[8]);
		void manipulateMenuLine(uint8_t value, uint8_t menuIndex, uint8_t startIndex);
};





#endif

