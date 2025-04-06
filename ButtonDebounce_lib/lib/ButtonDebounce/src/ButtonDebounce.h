#ifndef ButtonDebounce_h
#define ButtonDebounce_h

#include <Arduino.h>

class ButtonDebounce {
  public:
    ButtonDebounce(unsigned long debounceDelayMillis = 100);

    bool isBtnPressed = false;
    bool isBtnReleased = false;
    bool btnState = false;

    void updateState(bool btnStateInput);

  private:
    bool debounceDelayPassed = false;
    unsigned long debounceDelayMillis;
    unsigned long previousStateInternalChangeTime = 0;    
    bool btnStateInternal = false;

};

#endif