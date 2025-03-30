#include <Arduino.h>
#include <Keyboard.h>
#include "SegaGamepad.h"

const unsigned int delayBeforeReadMicros = 10; 
const unsigned int delayBeforeNextUpdateMicros = 2000;
SegaGamepad segaGamepad(A0, A1, A2, A3, 1, 0, 2, delayBeforeReadMicros, delayBeforeNextUpdateMicros);

const int keysCount = 12;
bool keys[keysCount];
bool keysPrevious[keysCount];

const char* keysNames[keysCount] = {
  "btnUp",
  "btnDown",
  "btnLeft",
  "btnRight",
  "btnA",
  "btnB",
  "btnC",
  "btnX",
  "btnY",
  "btnZ",
  "btnStart",
  "btnMode"
};

const uint8_t keysKeyboard[keysCount] = {
  'w',
  's',
  'a',
  'd',
  'j',
  'k',
  'l',
  'u',
  'i',
  'o',
  KEY_RETURN,
  '\\'
};

bool ledState = LOW;

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  segaGamepad.init();
}

void loop() {
  segaGamepad.update();

  keys[0] = segaGamepad.btnUp;
  keys[1] = segaGamepad.btnDown;
  keys[2] = segaGamepad.btnLeft;
  keys[3] = segaGamepad.btnRight;
  keys[4] = segaGamepad.btnA;
  keys[5] = segaGamepad.btnB;
  keys[6] = segaGamepad.btnC;
  keys[7] = segaGamepad.btnX;
  keys[8] = segaGamepad.btnY;
  keys[9] = segaGamepad.btnZ;
  keys[10] = segaGamepad.btnStart;
  keys[11] = segaGamepad.btnMode;

  for (int i = 0; i < keysCount; i++) {
    if (keys[i] && !keysPrevious[i]) {
      Serial.print(keysNames[i]); Serial.println(" pressed");
      Keyboard.press(keysKeyboard[i]);
    }

    if (!keys[i] && keysPrevious[i]) {
      Serial.print(keysNames[i]); Serial.println(" released");
      Keyboard.release(keysKeyboard[i]);
    }
  }

  memcpy(keysPrevious, keys, keysCount);
}
