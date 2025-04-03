#include <Arduino.h>
#include <Keyboard.h>
#include <Joystick.h>
#include <EEPROM.h>
#include "SegaGamepad.h"
#include "ButtonDebounce.h"

bool serialPrintEnabled = false;

const int modesCount = 2;
enum Mode {
  keyboard = 0,
  joystick = 1
};
const char* modeNames[modesCount] = { "keyboard", "joystick" };

Mode mode = keyboard;
int modeStorageAddress = 24;

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

const uint8_t keysKeyboard1[keysCount] = {
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

const uint8_t keysKeyboard2[keysCount] = {
  't',
  'g',
  'f',
  'h',
  'z',
  'x',
  'c',
  'v',
  'b',
  'n',
  'm',
  ','
};

void handleGamepad(SegaGamepad& segaGamepad, bool keys[], bool keysPrevious[], const uint8_t keysKeyboard[], int gamepadIndex, ButtonDebounce& modeButtonDebounce);

void setup() {
  Keyboard.begin();
  segaGamepad1.init();
  segaGamepad2.init();

  EEPROM.get(modeStorageAddress, mode);
  mode = (Mode)(mode % modesCount); 

  delay(2000);
  for (int i = 0; i < 3; i++) {
    segaGamepad1.update();
  }

  if (segaGamepad1.btnStart) {
    serialPrintEnabled = true;
    Serial.begin(115200);
    delay(5000);
    Serial.println(); Serial.println("Please stand by..."); 
    delay(1000);
    Serial.println(); Serial.println("Enabled serial output by pressing Start+Up on first gamepad during startup");
  } else {
    serialPrintEnabled = false;
  }

  if (segaGamepad1.btnA && segaGamepad1.btnStart) {
    mode = (Mode)((mode + 1) % modesCount);   
    EEPROM.put(modeStorageAddress, mode); 
    if (serialPrintEnabled) Serial.println("Output mode changed by pressing Start+A on first gamepad during startup");
  } else {
    if (serialPrintEnabled) Serial.println("Press Start+A on first gamepad during startup to change output mode");
  }

  if (serialPrintEnabled) { Serial.print("Current output mode: "); Serial.println(modeNames[mode]); Serial.println(); }
}

void loop() {
  handleGamepad(segaGamepad1, keys1, keysPrevious1, keysKeyboard1, 1, modeButtonDebounce1);
  handleGamepad(segaGamepad2, keys2, keysPrevious2, keysKeyboard2, 2, modeButtonDebounce2);
}

void handleGamepad(SegaGamepad& segaGamepad, bool keys[], bool keysPrevious[], const uint8_t keysKeyboard[], int gamepadIndex, ButtonDebounce& modeButtonDebounce) {
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
      Keyboard.press(keysKeyboard[i]);
    }

    if (!keys[i] && keysPrevious[i]) {
      Keyboard.release(keysKeyboard[i]);
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
