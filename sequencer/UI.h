#ifndef UI_h
#define UI_h

#include "Arduino.h"
#include <Wire.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiWire.h>

class UI
{
  public:
    UI(uint8_t clockPin, uint8_t dataPin, uint8_t latchPin);
    void setMaskValue(uint8_t noteValue);
    uint8_t handleNoteSelect(uint8_t note);
    void setNoteLed();
    void updateDisplay(uint8_t mode, uint8_t step, uint8_t bpm);
    void init();
    
  private:
    uint8_t _clockPin;
    uint8_t _dataPin;
    uint8_t _latchPin;
    uint16_t _noteMask = 0;
    SSD1306AsciiWire _oled;
    uint16_t getMaskValueForNote(uint8_t noteValue);
    void printDisplay();
};

#endif
