#include <Arduino.h>
#include <Keyboard.h>
#include "SegaGamepad.h"

SegaGamepad segaGamepad(1, 2, 3, 4, 5, 6, 7);

void setup() {
  Serial.begin(115200);
  segaGamepad.init();
}

void loop() {
  segaGamepad.update();

  String outputString = String() 
    + "isConnected:" + (int)segaGamepad.isConnected + " isSixBtns:" + (int)segaGamepad.isSixBtns
    + " Up:" + (int)segaGamepad.btnUp + " Down:" + (int)segaGamepad.btnDown + " Left:" + (int)segaGamepad.btnLeft + " Right:" + (int)segaGamepad.btnRight 
    + " A:" + (int)segaGamepad.btnA + " B:" + (int)segaGamepad.btnB + " C:" + (int)segaGamepad.btnC 
    + " x:" + (int)segaGamepad.btnX + " y:" + (int)segaGamepad.btnY + " z:" + (int)segaGamepad.btnZ + " Start:" + (int)segaGamepad.btnStart + " Mode:" + (int)segaGamepad.btnMode;
  Serial.println(outputString);

  delay(200);
}
