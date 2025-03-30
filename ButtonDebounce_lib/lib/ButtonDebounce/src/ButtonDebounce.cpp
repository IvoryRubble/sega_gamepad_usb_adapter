#include "ButtonDebounce.h"

ButtonDebounce::ButtonDebounce(unsigned long debounceDelayMillis) : debounceDelayMillis(debounceDelayMillis) {}

void ButtonDebounce::updateState(bool btnStateInput) {
    bool previousStateInternal = btnStateInternal;
    btnStateInternal = btnStateInput;
    unsigned long currentTime = millis();
    isBtnPressed = false;
    isBtnReleased = false;

    if (btnStateInternal != previousStateInternal) {
        previousStateInternalChangeTime = currentTime;
        debounceDelayPassed = false;
    }

    if (!debounceDelayPassed && currentTime - previousStateInternalChangeTime >= debounceDelayMillis) {
        debounceDelayPassed = true;
    }

    if (debounceDelayPassed && btnStateInternal != btnState) {
        btnState = btnStateInternal;
        isBtnPressed = btnStateInternal;
        isBtnReleased = !btnStateInternal;
    }
}
