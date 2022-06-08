#ifndef AnalogButtonHandler_h
#define AnalogButtonHandler_h

#include "Arduino.h"

class AnalogButtonHandler
{
  public:
  AnalogButtonHandler(uint8_t buttonPin, unsigned long debounceDelay);
	void attachCallback(void (*callbackMethod)(int valueRead));
	void (*callback)(int valueRead);
	void readValue(unsigned long millis);

  private:
	uint8_t _buttonPin;
	unsigned long _debounceDelay;
  int _previousRead = 0;
	unsigned long _lastRead = 0;
  bool didCallback = false;
};

#endif
