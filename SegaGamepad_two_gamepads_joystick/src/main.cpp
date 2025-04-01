#include <Arduino.h>
#include <Joystick.h>
#include "SegaGamepad.h"
#include "ButtonDebounce.h"

Joystick_ joystick1(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 8, 1, false, false, false, false, false, false, false, false, false, false, false);
Joystick_ joystick2(JOYSTICK_DEFAULT_REPORT_ID + 1, JOYSTICK_TYPE_GAMEPAD, 8, 1, false, false, false, false, false, false, false, false, false, false, false);

const unsigned int delayBeforeReadMicros = 10; 
const unsigned int delayBeforeNextUpdateMicros = 2000;
SegaGamepad segaGamepad1(A0, A1, A2, A3, 1, 0, 2, delayBeforeReadMicros, delayBeforeNextUpdateMicros);
SegaGamepad segaGamepad2(9, 8, 7, 6, 5, 4, 3, delayBeforeReadMicros, delayBeforeNextUpdateMicros);

const unsigned long debounceDelayMillis = 50;
ButtonDebounce modeButtonDebounce1(debounceDelayMillis);
ButtonDebounce modeButtonDebounce2(debounceDelayMillis);

const int keysCount = 12;
bool keys1[keysCount];
bool keys2[keysCount];
bool keysPrevious1[keysCount];
bool keysPrevious2[keysCount];

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

const uint8_t keysJoystick[keysCount] = {
  0,
  0,
  0,
  0,
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7
};

void setup() {
  Serial.begin(115200);
  joystick1.begin();
  joystick2.begin();
  segaGamepad1.init();
  segaGamepad2.init();
}

void loop() {
  segaGamepad1.update();
  segaGamepad2.update();

  modeButtonDebounce1.updateState(segaGamepad1.btnMode);
  modeButtonDebounce2.updateState(segaGamepad2.btnMode);

  keys1[0] = segaGamepad1.btnUp;
  keys1[1] = segaGamepad1.btnDown;
  keys1[2] = segaGamepad1.btnLeft;
  keys1[3] = segaGamepad1.btnRight;
  keys1[4] = segaGamepad1.btnA;
  keys1[5] = segaGamepad1.btnB;
  keys1[6] = segaGamepad1.btnC;
  keys1[7] = segaGamepad1.btnX;
  keys1[8] = segaGamepad1.btnY;
  keys1[9] = segaGamepad1.btnZ;
  keys1[10] = segaGamepad1.btnStart;
  keys1[11] = modeButtonDebounce1.btnState;

  keys2[0] = segaGamepad2.btnUp;
  keys2[1] = segaGamepad2.btnDown;
  keys2[2] = segaGamepad2.btnLeft;
  keys2[3] = segaGamepad2.btnRight;
  keys2[4] = segaGamepad2.btnA;
  keys2[5] = segaGamepad2.btnB;
  keys2[6] = segaGamepad2.btnC;
  keys2[7] = segaGamepad2.btnX;
  keys2[8] = segaGamepad2.btnY;
  keys2[9] = segaGamepad2.btnZ;
  keys2[10] = segaGamepad2.btnStart;
  keys2[11] = modeButtonDebounce2.btnState;

  for (int i = 0; i < keysCount; i++) {
    if (keys1[i] && !keysPrevious1[i]) {
      Serial.print(keysNames[i]); Serial.println(" pressed on gamepad 1");
      if (i >= 4) {
        joystick1.pressButton(keysJoystick[i]);
      }
    }

    if (!keys1[i] && keysPrevious1[i]) {
      Serial.print(keysNames[i]); Serial.println(" released on gamepad 1");
      if (i >= 4) {
        joystick1.releaseButton(keysJoystick[i]);
      }
    }

    if (keys2[i] && !keysPrevious2[i]) {
      Serial.print(keysNames[i]); Serial.println(" pressed on gamepad 2");
      if (i >= 4) {
        joystick2.pressButton(keysJoystick[i]);
      }
    }

    if (!keys2[i] && keysPrevious2[i]) {
      Serial.print(keysNames[i]); Serial.println(" released on gamepad 2");
      if (i >= 4) {
        joystick2.releaseButton(keysJoystick[i]);
      }
    }
  }

  bool isArrowChanged = false;
  for (int i = 0; i < 4; i++) {
    isArrowChanged = isArrowChanged || (keys1[i] != keysPrevious1[i]);
  }
  if (isArrowChanged) {
    if (segaGamepad1.btnUp && segaGamepad1.btnRight) {
      joystick1.setHatSwitch(0, 45);
    } else if (segaGamepad1.btnRight && segaGamepad1.btnDown) {
      joystick1.setHatSwitch(0, 135);
    } else if (segaGamepad1.btnDown && segaGamepad1.btnLeft) {
      joystick1.setHatSwitch(0, 225);
    } else if (segaGamepad1.btnLeft && segaGamepad1.btnUp) {
      joystick1.setHatSwitch(0, 315);
    } else if (segaGamepad1.btnUp) {
      joystick1.setHatSwitch(0, 0);
    } else if (segaGamepad1.btnRight) {
      joystick1.setHatSwitch(0, 90);
    } else if (segaGamepad1.btnDown) {
      joystick1.setHatSwitch(0, 180);
    } else if (segaGamepad1.btnLeft) {
      joystick1.setHatSwitch(0, 270);
    } else {
      joystick1.setHatSwitch(0, -1);
    }
  }

  memcpy(keysPrevious1, keys1, keysCount);
  memcpy(keysPrevious2, keys2, keysCount);
}
