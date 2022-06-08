#define DEBUG

#include "Debug.h"
#include "Arduino.h"
#include "AnalogButtonHandler.h"

AnalogButtonHandler::AnalogButtonHandler(uint8_t buttonPin, unsigned long debounceDelay)
{
	_buttonPin = buttonPin;
	_debounceDelay = debounceDelay;
	_lastRead = 0;
}

void AnalogButtonHandler::attachCallback(void (*callbackMethod)(int valueRead)) {
	callback = callbackMethod;
}

void AnalogButtonHandler::readValue(unsigned long millis) {
  if(_lastRead > millis - _debounceDelay) {
    return;  
  }
  int buttonValue = analogRead(_buttonPin);
  int buttonValueMin = _previousRead - 10;
  int buttonValueMax = _previousRead + 10;

  if(buttonValue == 0) {
    if(didCallback) {
      DEBUG_PRINTLN("reset callback");
      didCallback = false;
    }
    return;
  } else if(buttonValue >= buttonValueMin && buttonValue <= buttonValueMax) {
    // Proper reading
    DEBUG_PRINT("buttonValue press");
    DEBUG_PRINTLN(buttonValue);
    if(!didCallback) {
      callback(buttonValue);
      didCallback = true;
    }
  } else {
    DEBUG_PRINTLN("BOUNCE");  
    DEBUG_PRINT("buttonValue ");
    DEBUG_PRINTLN(buttonValue);
    DEBUG_PRINT("buttonValueMin ");
    DEBUG_PRINTLN(buttonValueMin);
    DEBUG_PRINT("buttonValueMax ");
    DEBUG_PRINTLN(buttonValueMax);
  }
  _lastRead = millis;
  _previousRead = buttonValue;
}
