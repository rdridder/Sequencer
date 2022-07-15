// LedController.h

#ifndef _LEDCONTROLLER_h
#define _LEDCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Config.h"
class LedController
{
public:
	LedController();
	void setLedState(unsigned long data);
	void setup();

protected:

private:
};

#endif

