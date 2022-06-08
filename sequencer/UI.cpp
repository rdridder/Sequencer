#include "Debug.h"
#include "Arduino.h"
#include "UI.h"

#define I2C_ADDRESS 0x3C
#define INCLUDE_SCROLLING 0
#define INITIAL_SCROLL_MODE SCROLL_MODE_OFF

UI::UI(uint8_t clockPin, uint8_t dataPin, uint8_t latchPin)
{ 
  _clockPin = clockPin;
  _dataPin = dataPin;
  _latchPin = latchPin;
}

void UI::init() {
  // Setup display
  Wire.begin();
  Wire.setClock(400000L);  
  _oled.begin(&Adafruit128x64, I2C_ADDRESS);
  _oled.setFont(X11fixed7x14B);  
  _oled.clear();
  printDisplay();
}

uint16_t UI::getMaskValueForNote(uint8_t noteValue) {
  if(noteValue == 34) {
    return 4096; 
  } else if(noteValue == 32) {
    return 2048;  
  } else if(noteValue == 30) {
    return 1024;  
  } else if(noteValue == 27) {
    return 512;  
  } else if(noteValue == 25) {
    return 256;  
  } else if(noteValue == 36) {
    return 128;  
  } else if(noteValue == 35) {
    return 64;  
  } else if(noteValue == 33) {
    return 32; 
  } else if(noteValue == 31) {
    return 16; 
  } else if(noteValue == 29) {
    return 8;  
  } else if(noteValue == 28) {
    return 4;  
  } else if(noteValue == 26) {
    return 2;  
  } else if(noteValue == 24) {
    return 1;  
  } else {
    return 0;  
  }
}

void UI::setNoteLed() {
  uint8_t whiteNoteByte = _noteMask & 255;
  uint8_t blackNoteByte = (_noteMask >> 8);

  DEBUG_PRINT("whiteNote ");
  DEBUG_PRINTLN(whiteNoteByte);
  DEBUG_PRINT("blackNoteByte ");
  DEBUG_PRINTLN(blackNoteByte);

  shiftOut(_dataPin, _clockPin, MSBFIRST, blackNoteByte);
  shiftOut(_dataPin, _clockPin, MSBFIRST, whiteNoteByte);
  digitalWrite(_latchPin, LOW);
  digitalWrite(_latchPin, HIGH); 
}

uint8_t UI::handleNoteSelect(uint8_t note) {
  uint16_t mask = getMaskValueForNote(note);
  uint8_t midiNote;  
  if(_noteMask == 0) {
    midiNote = note;  
  }
  _noteMask ^= mask;

  DEBUG_PRINT("note ");
  DEBUG_PRINTLN(note); 
  DEBUG_PRINT("_noteMask ");
  DEBUG_PRINTLN(_noteMask);   

  return midiNote;
}

void UI::setMaskValue(uint8_t noteValue) {
  uint16_t maskValue = getMaskValueForNote(noteValue);
  _noteMask = maskValue;

  DEBUG_PRINT("noteValue ");
  DEBUG_PRINTLN(noteValue);
  DEBUG_PRINT("maskValue ");
  DEBUG_PRINTLN(maskValue);    
}

void UI::updateDisplay(uint8_t mode, uint8_t step, uint8_t bpm) {
  _oled.setCursor(40, 0);
  _oled.print(mode);
  _oled.setCursor(40, 2);  
  _oled.print(step);
  _oled.print(" ");
  _oled.setCursor(40, 4);  
  _oled.print(bpm);
}

void UI::printDisplay() {
  _oled.setCursor(0, 0);
  _oled.println("mode");
  _oled.setCursor(0, 2);
  _oled.println("step");  
  _oled.setCursor(0, 4);
  _oled.println("bpm");  
}
