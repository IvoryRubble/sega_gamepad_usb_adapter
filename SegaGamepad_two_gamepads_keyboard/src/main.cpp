#include <Arduino.h>
#include <Keyboard.h>
#include "SegaGamepad.h"

const unsigned int delayBeforeReadMicros = 10; 
const unsigned int delayBeforeNextUpdateMicros = 2000;
SegaGamepad segaGamepad(6, 1, 2, 3, 4, 5, 7, delayBeforeReadMicros, delayBeforeNextUpdateMicros);

const int keysCount = 12;
bool keys[keysCount];
bool keysPrevious[keysCount];

bool keysPressed[keysCount];
bool keysReleased[keysCount];

const char* keysPressedMessages[keysCount] = {
  "segaGamepad.btnUp pressed",
  "segaGamepad.btnDown pressed",
  "segaGamepad.btnLeft pressed",
  "segaGamepad.btnRight pressed",
  "segaGamepad.btnA pressed",
  "segaGamepad.btnB pressed",
  "segaGamepad.btnC pressed",
  "segaGamepad.btnX pressed",
  "segaGamepad.btnY pressed",
  "segaGamepad.btnZ pressed",
  "segaGamepad.btnStart pressed",
  "segaGamepad.btnMode pressed"
};

const char* keysReleasedMessages[keysCount] = {
  "segaGamepad.btnUp released",
  "segaGamepad.btnDown released",
  "segaGamepad.btnLeft released",
  "segaGamepad.btnRight released",
  "segaGamepad.btnA released",
  "segaGamepad.btnB released",
  "segaGamepad.btnC released",
  "segaGamepad.btnX released",
  "segaGamepad.btnY released",
  "segaGamepad.btnZ released",
  "segaGamepad.btnStart released",
  "segaGamepad.btnMode released"
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
      keysPressed[i] = true;
    } else {
      keysPressed[i] = false;
    }

    if (!keys[i] && keysPrevious[i]) {
      keysReleased[i] = true;
    } else {
      keysReleased[i] = false;
    }
  }

  for (int i = 0; i < keysCount; i++) {
    if (keysPressed[i]) {
      Serial.println(keysPressedMessages[i]);
      Keyboard.press(keysKeyboard[i]);
    }
    if (keysReleased[i]) {
      Serial.println(keysReleasedMessages[i]);
      Keyboard.release(keysKeyboard[i]);
    }
  }

  memcpy(keysPrevious, keys, keysCount);
}
