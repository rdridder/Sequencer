#ifndef _DISPLAYCONTROLLER_h
#define _DISPLAYCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <Wire.h>
#include "Config.h"
#include <Adafruit_SSD1306.h>

class DisplayController
{
	public:
		DisplayController();
		void startDisplayOutput();
		void stopDisplayOutput();
		void printMenuLine(char menuLine[21]);
		void printActiveMenuLine(char menuLine[21]);
		void printEncoderValues(int encoderIndex, long encoderValue);
		void printButtonValues(unsigned long buttonValues);
		void setup();

	protected:

	private:
		Adafruit_SSD1306 *_display;
};

#endif

