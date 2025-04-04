#include <Arduino.h>
#include <Keyboard.h>
#include "SegaGamepad.h"

const bool serialPrintEnabled = true;

const unsigned int delayBeforeReadMicros = 10; 
const unsigned int delayBeforeNextUpdateMicros = 2000;
SegaGamepad segaGamepad1(A0, A1, A2, A3, 1, 0, 2, delayBeforeReadMicros, delayBeforeNextUpdateMicros);
SegaGamepad segaGamepad2(9, 8, 7, 6, 5, 4, 3, delayBeforeReadMicros, delayBeforeNextUpdateMicros);

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

void printGamepadStatusOnSetup(SegaGamepad& segaGamepad, int gamepadIndex);
void handleGamepad(SegaGamepad& segaGamepad, int gamepadIndex, const uint8_t keysKeyboard[]);
void updateKeyboard(bool keys[], bool keysPrevious[], const uint8_t keysKeyboard[]);
void printGamepadStatus(SegaGamepad& segaGamepad, int gamepadIndex, bool keys[], bool keysPrevious[], bool isConnectedPrevious, bool isSixButtonsPrevious);
void initKeys(bool keys[], SegaGamepad& segaGamepad);

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  segaGamepad1.init();
  segaGamepad2.init();

  int gamepadReadigsToDiscard = 2;
  for (int i = 0; i < gamepadReadigsToDiscard + 1; i++) {
    segaGamepad1.update();
    segaGamepad2.update();
  }

  if (serialPrintEnabled) {
    delay(5000);
    printGamepadStatusOnSetup(segaGamepad1, 1);
    printGamepadStatusOnSetup(segaGamepad2, 2);
  }
}

void loop() {
  handleGamepad(segaGamepad1, 1, keysKeyboard1);
  handleGamepad(segaGamepad2, 2, keysKeyboard2);
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
}

void handleGamepad(SegaGamepad& segaGamepad, int gamepadIndex, const uint8_t keysKeyboard[]) {
  bool keysPrevious[keysCount];
  initKeys(keysPrevious, segaGamepad);
  bool isConnectedPrevious = segaGamepad.isConnected;
  bool isSixButtonsPrevious = segaGamepad.isSixBtns;

  segaGamepad.update();

  bool keys[keysCount];
  initKeys(keys, segaGamepad);

  updateKeyboard(keys, keysPrevious, keysKeyboard);

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

void printGamepadStatus(SegaGamepad& segaGamepad, int gamepadIndex, bool keys[], bool keysPrevious[], bool isConnectedPrevious, bool isSixButtonsPrevious) {
  for (int i = 0; i < keysCount; i++) {
    if (keys[i] && !keysPrevious[i]) {
      Serial.print(keysNames[i]); Serial.print(" pressed on gamepad "); Serial.println(gamepadIndex);
    }
    if (!keys[i] && keysPrevious[i]) {
      Serial.print(keysNames[i]); Serial.print(" released on gamepad "); Serial.println(gamepadIndex);
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

void initKeys(bool keys[], SegaGamepad& segaGamepad) {
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
}
