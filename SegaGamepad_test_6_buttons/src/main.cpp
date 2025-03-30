#include <Arduino.h>

const int pin1 = 1;
const int pin2 = 2;
const int pin3 = 3;
const int pin4 = 4;
const int pin6 = 5;
const int pin7 = 6;
const int pin9 = 7;

const int pinSelect = pin7;

const unsigned long delayBeforeReadMicros = 10;

void setup() {
  Serial.begin(115200);

  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  pinMode(pin3, INPUT_PULLUP);
  pinMode(pin4, INPUT_PULLUP);
  pinMode(pin6, INPUT_PULLUP);
  pinMode(pin9, INPUT_PULLUP);

  pinMode(pinSelect, OUTPUT);
  digitalWrite(pinSelect, HIGH);
}

void loop() {
  // state 0
  digitalWrite(pinSelect, LOW);
  delayMicroseconds(delayBeforeReadMicros);
  bool isConnected = !digitalRead(pin3) && !digitalRead(pin4);
  bool btnA = !digitalRead(pin6);
  bool btnStart = !digitalRead(pin9);
  
  // state 1
  digitalWrite(pinSelect, HIGH);
  delayMicroseconds(delayBeforeReadMicros);
  bool btnUp = !digitalRead(pin1);
  bool btnDown = !digitalRead(pin2);
  bool btnLeft = !digitalRead(pin3);
  bool btnRight = !digitalRead(pin4);
  bool btnB = !digitalRead(pin6);
  bool btnC = !digitalRead(pin9);

  // state 2
  digitalWrite(pinSelect, LOW);
  delayMicroseconds(delayBeforeReadMicros);

  // state 3
  digitalWrite(pinSelect, HIGH);
  delayMicroseconds(delayBeforeReadMicros);
  
  // state 4
  digitalWrite(pinSelect, LOW);
  delayMicroseconds(delayBeforeReadMicros);
  bool isSixBtns = !digitalRead(pin1) && !digitalRead(pin2);

  // state 5
  digitalWrite(pinSelect, HIGH);
  delayMicroseconds(delayBeforeReadMicros);
  bool btnZ = !digitalRead(pin1);
  bool btnY = !digitalRead(pin2);
  bool btnX = !digitalRead(pin3);
  bool btnMode = !digitalRead(pin4);

  // state 6
  digitalWrite(pinSelect, LOW);
  delayMicroseconds(delayBeforeReadMicros);

  // state 7
  digitalWrite(pinSelect, HIGH);
  delayMicroseconds(delayBeforeReadMicros);

  String outputString = String() 
    + "isConnected:" + (int)isConnected + " isSixBtns:" + (int)isSixBtns
    + " Up:" + (int)btnUp + " Down:" + (int)btnDown + " Left:" + (int)btnLeft + " Right:" + (int)btnRight 
    + " A:" + (int)btnA + " B:" + (int)btnB + " C:" + (int)btnC 
    + " x:" + (int)btnX + " y:" + (int)btnY + " z:" + (int)btnZ + " Start:" + (int)btnStart + " Mode:" + (int)btnMode;
  Serial.println(outputString);

  delay(200);
}
