#include <Arduino.h>

const int pin1 = 0;
const int pin2 = 0;
const int pin3 = 0;
const int pin4 = 0;
const int pin6 = 0;
const int pin7 = 0;
const int pin9 = 0;

const int pinSelect = pin7;

const unsigned long delayBeforeReadMicros = 10;

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

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

  // state 1
  digitalWrite(pinSelect, HIGH);
  delayMicroseconds(delayBeforeReadMicros);

  // state 2
  digitalWrite(pinSelect, LOW);
  delayMicroseconds(delayBeforeReadMicros);
  bool isConnected = digitalRead(pin3) && digitalRead(pin4);

  // state 3
  digitalWrite(pinSelect, HIGH);
  delayMicroseconds(delayBeforeReadMicros);
  bool btnUp = !digitalRead(pin1);
  bool btnDown = !digitalRead(pin2);
  bool btnLeft = !digitalRead(pin3);
  bool btnRight = !digitalRead(pin4);
  bool btnA = !digitalRead(pin6);
  bool btnStart = !digitalRead(pin9);

  // state 4
  digitalWrite(pinSelect, LOW);
  delayMicroseconds(delayBeforeReadMicros);
  bool isSixBtns = digitalRead(pin1) && digitalRead(pin2);

  // state 5
  digitalWrite(pinSelect, HIGH);
  delayMicroseconds(delayBeforeReadMicros);
  bool btnZ = !digitalRead(pin1);
  bool btnY = !digitalRead(pin2);
  bool btnX = !digitalRead(pin3);
  bool btnMode = !digitalRead(pin4);

  // state 5
  digitalWrite(pinSelect, LOW);
  delayMicroseconds(delayBeforeReadMicros);

  // state 6
  digitalWrite(pinSelect, HIGH);
  delayMicroseconds(delayBeforeReadMicros);

  String outputString = String() + (int)isConnected + "\t" + (int)isSixBtns + "\t";
  Serial.println(outputString);

  delayMicroseconds(2000);

  digitalWrite(LED_BUILTIN, HIGH);
  delay(10);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
