//
// SegaGamepad.cpp
//
// Author:
//       Yaroslav Metelev <yaroslav.metelev@outlook.com>
//
// Copyright (c) 2025 Yaroslav Metelev <https://github.com/IvoryRubble>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "SegaGamepad.h"

SegaGamepad::SegaGamepad(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin6, uint8_t pin7, uint8_t pin9, unsigned int delayBeforeReadMicros, unsigned int delayBeforeNextUpdateMicros) : 
    pin1(pin1),
    pin2(pin2),
    pin3(pin3),
    pin4(pin4),
    pin6(pin6),
    pinSelect(pin7),
    pin9(pin9),
    delayBeforeReadMicros(delayBeforeReadMicros),
    delayBeforeNextUpdateMicros(delayBeforeNextUpdateMicros) {}

void SegaGamepad::init() {
    pinMode(pin1, INPUT_PULLUP);
    pinMode(pin2, INPUT_PULLUP);
    pinMode(pin3, INPUT_PULLUP);
    pinMode(pin4, INPUT_PULLUP);
    pinMode(pin6, INPUT_PULLUP);
    pinMode(pin9, INPUT_PULLUP);

    pinMode(pinSelect, OUTPUT);
    digitalWrite(pinSelect, HIGH);
}

void SegaGamepad::update() {
    unsigned long timeAfterPreviousUpdate = micros() - previousUpdateTime;
    if (timeAfterPreviousUpdate < delayBeforeNextUpdateMicros) {
        delayMicroseconds(delayBeforeNextUpdateMicros - timeAfterPreviousUpdate);
    }

    // state 0
    digitalWrite(pinSelect, LOW);
    delayMicroseconds(delayBeforeReadMicros);
    isConnected = !digitalRead(pin3) && !digitalRead(pin4);
    
    if (isConnected) {
        btnA = !digitalRead(pin6);
        btnStart = !digitalRead(pin9);
    }
    
    // state 1
    digitalWrite(pinSelect, HIGH);
    delayMicroseconds(delayBeforeReadMicros);
    if (isConnected) {
        btnUp = !digitalRead(pin1);
        btnDown = !digitalRead(pin2);
        btnLeft = !digitalRead(pin3);
        btnRight = !digitalRead(pin4);
        btnB = !digitalRead(pin6);
        btnC = !digitalRead(pin9);
    } else {
        btnUp = false;
        btnDown = false;
        btnLeft = false;
        btnRight = false;
        btnB = false;
        btnC = false;
    }

    // state 2
    digitalWrite(pinSelect, LOW);
    delayMicroseconds(delayBeforeReadMicros);

    // state 3
    digitalWrite(pinSelect, HIGH);
    delayMicroseconds(delayBeforeReadMicros);
    
    // state 4
    digitalWrite(pinSelect, LOW);
    delayMicroseconds(delayBeforeReadMicros);
    if (isConnected) {
        isSixBtns = !digitalRead(pin1) && !digitalRead(pin2);
    } else {
        isSixBtns = false;
    }

    // state 5
    digitalWrite(pinSelect, HIGH);
    delayMicroseconds(delayBeforeReadMicros);
    if (isConnected && isSixBtns) {
        btnZ = !digitalRead(pin1);
        btnY = !digitalRead(pin2);
        btnX = !digitalRead(pin3);
        btnMode = !digitalRead(pin4);
    } else {
       btnZ = 0; 
       btnY = 0; 
       btnX = 0; 
       btnMode = 0; 
    }
    
    // state 6
    digitalWrite(pinSelect, LOW);
    delayMicroseconds(delayBeforeReadMicros);

    // state 7
    digitalWrite(pinSelect, HIGH);
    delayMicroseconds(delayBeforeReadMicros);

    previousUpdateTime = micros();
}
