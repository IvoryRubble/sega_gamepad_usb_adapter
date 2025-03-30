#include <Arduino.h>
#include "ButtonDebounce.h"

const unsigned long debounceDelayMillis = 1000;
const uint8_t buttonPin = 2;
ButtonDebounce buttonDebounce(debounceDelayMillis);

void setup() {
  Serial.begin(115200);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  buttonDebounce.updateState(!digitalRead(buttonPin));
  digitalWrite(LED_BUILTIN, buttonDebounce.btnState);
}
