#include <Arduino.h>
#include <Joystick.h>
#include "SegaGamepad.h"
#include "ButtonDebounce.h"

const bool serialPrintEnabled = false;

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

void handleGamepad(SegaGamepad& segaGamepad, bool keys[], bool keysPrevious[], int gamepadIndex, ButtonDebounce& modeButtonDebounce, Joystick_& joystick);

void setup() {
  Serial.begin(115200);
  joystick1.begin();
  joystick2.begin();
  segaGamepad1.init();
  segaGamepad2.init();
}

void loop() {
  handleGamepad(segaGamepad1, keys1, keysPrevious1, 1, modeButtonDebounce1, joystick1);
  handleGamepad(segaGamepad2, keys2, keysPrevious2, 2, modeButtonDebounce2, joystick2);
}

void handleGamepad(SegaGamepad& segaGamepad, bool keys[], bool keysPrevious[], int gamepadIndex, ButtonDebounce& modeButtonDebounce, Joystick_& joystick) {
  segaGamepad.update();

  modeButtonDebounce.updateState(segaGamepad.btnMode);

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
  keys[11] = modeButtonDebounce.btnState;

  for (int i = 0; i < keysCount; i++) {
    if (keys[i] && !keysPrevious[i]) {
      if (i >= 4) {
        joystick.pressButton(keysJoystick[i]);
      }
    }

    if (!keys[i] && keysPrevious[i]) {
      if (i >= 4) {
        joystick.releaseButton(keysJoystick[i]);
      }
    }
  }

  bool isArrowChanged = false;
  for (int i = 0; i < 4; i++) {
    isArrowChanged = isArrowChanged || (keys[i] != keysPrevious[i]);
  }
  if (isArrowChanged) {
    if (segaGamepad.btnUp && segaGamepad.btnRight) {
      joystick.setHatSwitch(0, 45);
    } else if (segaGamepad.btnRight && segaGamepad.btnDown) {
      joystick.setHatSwitch(0, 135);
    } else if (segaGamepad.btnDown && segaGamepad.btnLeft) {
      joystick.setHatSwitch(0, 225);
    } else if (segaGamepad.btnLeft && segaGamepad.btnUp) {
      joystick.setHatSwitch(0, 315);
    } else if (segaGamepad.btnUp) {
      joystick.setHatSwitch(0, 0);
    } else if (segaGamepad.btnRight) {
      joystick.setHatSwitch(0, 90);
    } else if (segaGamepad.btnDown) {
      joystick.setHatSwitch(0, 180);
    } else if (segaGamepad.btnLeft) {
      joystick.setHatSwitch(0, 270);
    } else {
      joystick.setHatSwitch(0, -1);
    }
  }

  if (serialPrintEnabled) {
    for (int i = 0; i < keysCount; i++) {
      if (keys[i] && !keysPrevious[i]) {
        Serial.print(keysNames[i]); Serial.print(" pressed on gamepad "); Serial.println(gamepadIndex);
      }

      if (!keys[i] && keysPrevious[i]) {
        Serial.print(keysNames[i]); Serial.print(" released on gamepad "); Serial.println(gamepadIndex);
      }
    }
  }

  memcpy(keysPrevious, keys, keysCount);
}
