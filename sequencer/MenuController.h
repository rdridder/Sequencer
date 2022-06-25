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
		void handleStep();

	protected:

	private:
		DisplayController* _displayController;
		SequencerEngine* _sequencerEngine;
		char _started[8] = "started";
		char _stopped[8] = "stopped";
		char* _mainMenuItems[21] = { "stopped              ", "bpm                  ", "step                 " };
		char* _noteMenuItems[21] = { "note                 ", "length               ", "velocity 100         " , "octave 0             ", "step                 " };
		uint8_t const _numberOfMainMenuItems = 3;
		uint8_t const _numberOfNoteMenuItems = 5;
		int8_t _activeIndex = 0;
		bool _isStarted = false;
		bool _itemSelected = true;
		void handleStartStop();
		void handleBPM(int direction);
		void manipulateStartStopLine(char characters[8]);
		void manipulateMenuLine(uint8_t value, uint8_t menuIndex, uint8_t startIndex);
};





#endif

