#include <Arduino.h>
#include <Keyboard.h>
#include <Joystick.h>
#include <EEPROM.h>
#include "SegaGamepad.h"
#include "ButtonDebounce.h"

bool serialPrintEnabled = false;
unsigned long previousKeyUpdateTime = 0;

const int outputModesCount = 2;
enum OutputMode {
  keyboard = 0,
  joystick = 1
};
const char* outputModeNames[outputModesCount] = { "keyboard", "joystick" };

OutputMode outputMode = keyboard;
int outputModeStorageAddress = 24;

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

void initSerialPrintEnableFlag();
void initOutputMode();
void printOutputModeInfo();
void printGamepadStatusOnSetup(SegaGamepad& segaGamepad, int gamepadIndex);
void handleGamepad(SegaGamepad& segaGamepad, int gamepadIndex, ButtonDebounce& modeButtonDebounce, const uint8_t keysKeyboard[], Joystick_& joystick);
void updateJoystick(SegaGamepad& segaGamepad, bool keys[], bool keysPrevious[], Joystick_& joystick);
void updateKeyboard(bool keys[], bool keysPrevious[], const uint8_t keysKeyboard[]);
void printGamepadStatus(SegaGamepad& segaGamepad, int gamepadIndex, bool keys[], bool keysPrevious[], bool isConnectedPrevious, bool isSixButtonsPrevious);
void initKeys(bool keys[], SegaGamepad& segaGamepad, ButtonDebounce& modeButtonDebounce);

void setup() {
  segaGamepad1.init();
  segaGamepad2.init();

  delay(2000);
  int gamepadReadigsToDiscard = 2;
  for (int i = 0; i < gamepadReadigsToDiscard + 1; i++) {
    segaGamepad1.update();
    segaGamepad2.update();
  }

  initSerialPrintEnableFlag();
  initOutputMode();

  if (serialPrintEnabled) {
    printOutputModeInfo();
  }

  switch (outputMode) {
    case OutputMode::keyboard:
      Keyboard.begin();
      break;
    case OutputMode::joystick:
      joystick1.begin();
      joystick2.begin();
      break;
  }

  if (serialPrintEnabled) {
    printGamepadStatusOnSetup(segaGamepad1, 1);
    printGamepadStatusOnSetup(segaGamepad2, 2);
  }
}

void loop() {
  handleGamepad(segaGamepad1, 1, modeButtonDebounce1, keysKeyboard1, joystick1);
  handleGamepad(segaGamepad2, 2, modeButtonDebounce2, keysKeyboard2, joystick2);
}

void printOutputModeInfo() {
  Serial.println("Press Start+A on first gamepad during startup to change output mode to keyboard");
  Serial.println("Press Start+B on first gamepad during startup to change output mode to joystick");
  Serial.print("Current output mode: ");
  Serial.println(outputModeNames[outputMode]);
  Serial.println();
}

void initOutputMode() {
  if (segaGamepad1.btnStart && (segaGamepad1.btnA || segaGamepad1.btnB)) {
    if (segaGamepad1.btnA) outputMode = OutputMode::keyboard;
    if (segaGamepad1.btnB) outputMode = OutputMode::joystick;
    EEPROM.put(outputModeStorageAddress, outputMode);
  } else {
    EEPROM.get(outputModeStorageAddress, outputMode);
    outputMode = (OutputMode)(outputMode % outputModesCount);
  }
}

void initSerialPrintEnableFlag() {
  if (segaGamepad1.btnStart) {
    serialPrintEnabled = true;
    Serial.begin(115200);
    delay(5000);
    Serial.println();
    Serial.println("Please stand by...");
    delay(1000);
    Serial.println();
    Serial.println("Enabled serial output by pressing Start on first gamepad during startup");
  } else {
    serialPrintEnabled = false;
  }
}

void printGamepadStatusOnSetup(SegaGamepad& segaGamepad, int gamepadIndex) {
  if (segaGamepad.isConnected) {
    Serial.print("Gamepad "); Serial.print(gamepadIndex); Serial.println(" connected"); 
  } else {
    Serial.print("Gamepad "); Serial.print(gamepadIndex); Serial.println(" disconnected"); 
  }

  if (segaGamepad.isConnected && segaGamepad.isSixBtns) {
    Serial.print("Gamepad "); Serial.print(gamepadIndex); Serial.print(" type: "); 
    if (segaGamepad.isSixBtns) {
      Serial.println("\"six buttons\"");
    } else {
      Serial.println("\"three buttons\"");
    }
  }
  Serial.println();
}

void handleGamepad(SegaGamepad& segaGamepad, int gamepadIndex, ButtonDebounce& modeButtonDebounce, const uint8_t keysKeyboard[], Joystick_& joystick) {
  bool keysPrevious[keysCount];
  initKeys(keysPrevious, segaGamepad, modeButtonDebounce);
  bool isConnectedPrevious = segaGamepad.isConnected;
  bool isSixButtonsPrevious = segaGamepad.isSixBtns;

  segaGamepad.update();
  modeButtonDebounce.updateState(segaGamepad.btnMode);

  bool keys[keysCount];
  initKeys(keys, segaGamepad, modeButtonDebounce);

  switch (outputMode) {
    case OutputMode::keyboard:
      updateKeyboard(keys, keysPrevious, keysKeyboard);
      break;
    case OutputMode::joystick:
      updateJoystick(segaGamepad, keys, keysPrevious, joystick);
      break;  
  }
  
  if (serialPrintEnabled) {
    printGamepadStatus(segaGamepad, gamepadIndex, keys, keysPrevious, isConnectedPrevious, isSixButtonsPrevious);
  }
}

void updateKeyboard(bool keys[], bool keysPrevious[], const uint8_t keysKeyboard[]) {
  for (int i = 0; i < keysCount; i++) {
    if (keys[i] && !keysPrevious[i]) {
      Keyboard.press(keysKeyboard[i]);
    }
    if (!keys[i] && keysPrevious[i]) {
      Keyboard.release(keysKeyboard[i]);
    }
  }
}

void updateJoystick(SegaGamepad& segaGamepad, bool keys[], bool keysPrevious[], Joystick_& joystick) {
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
}

void printGamepadStatus(SegaGamepad& segaGamepad, int gamepadIndex, bool keys[], bool keysPrevious[], bool isConnectedPrevious, bool isSixButtonsPrevious) {
  unsigned long currentTime = millis();

  for (int i = 0; i < keysCount; i++) {
    if (keys[i] && !keysPrevious[i]) {
      Serial.print("+ "); Serial.print(currentTime - previousKeyUpdateTime); Serial.print(" ms "); Serial.print(keysNames[i]); Serial.print(" pressed on gamepad "); Serial.println(gamepadIndex);
      previousKeyUpdateTime = currentTime;
    }
    if (!keys[i] && keysPrevious[i]) {
      Serial.print("+ "); Serial.print(currentTime - previousKeyUpdateTime); Serial.print(" ms "); Serial.print(keysNames[i]); Serial.print(" released on gamepad "); Serial.println(gamepadIndex);
      previousKeyUpdateTime = currentTime;
    }
  }

  if (segaGamepad.isConnected && !isConnectedPrevious) {
    Serial.print("Gamepad "); Serial.print(gamepadIndex); Serial.println(" connected"); 
  }
  if (!segaGamepad.isConnected && isConnectedPrevious) {
    Serial.print("Gamepad "); Serial.print(gamepadIndex); Serial.println(" disconnected"); 
  }

  if (segaGamepad.isConnected && segaGamepad.isSixBtns != isSixButtonsPrevious) {
    Serial.print("Gamepad "); Serial.print(gamepadIndex); Serial.print(" type changed to "); 
    if (segaGamepad.isSixBtns) {
      Serial.println("\"six buttons\"");
    } else {
      Serial.println("\"three buttons\"");
    }
  }
}

void initKeys(bool keys[], SegaGamepad& segaGamepad, ButtonDebounce& modeButtonDebounce) {
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
}

