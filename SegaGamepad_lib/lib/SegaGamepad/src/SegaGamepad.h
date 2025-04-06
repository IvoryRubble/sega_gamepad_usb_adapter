//
// SegaGamepad.h
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
// -----------------------------------------------------------------------------
// Yet another library to read Sega Mega Drive (aka Sega Genesis) gamepad.
// Supports both three and six buttons controllers.
//
// -----------------------------------------------------------------------------
// Sources:
//
// Gamepad protocol description:
// http://web.archive.org/web/20171229105419/http://www.cs.cmu.edu/~chuck/infopg/segasix.txt
// https://github.com/jonthysell/SegaController/wiki/How-To-Read-Sega-Controllers
// Gamepad protocol reverse engineering:
// https://www.raspberryfield.life/2019/03/25/sega-mega-drive-genesis-6-button-xyz-controller/#SMD6-protocol-overview
// Another good library:
// https://github.com/jonthysell/SegaController
//
// -----------------------------------------------------------------------------
// Sega Mega Drive gamepad connector pinout, female DB9 plug:
//
//    +----------- pin5, +5V
//    | +--------- pin4, set by gamepad            
//    | | +------- pin3, set by gamepad  
//    | | | +----- pin2, set by gamepad   
//    | | | | +--- pin1, set by gamepad
//    | | | | |            
// +-------------+             
//  \ O O O O O /                
//   \ O O O O /                  
//    +-------+                 
//     | | | |
//     | | | +---- pin6, set by gamepad
//     | | +------ pin7, Select pin, set to 0 or 1 by microcontroller        
//     | +-------- pin8, GND
//     +---------- pin9, set by gamepad         
//
// -----------------------------------------------------------------------------
// Connection:                                 
//
// Connect pin8 and pin5 to GND and +5V.
// Connect pin1, pin2, pin3, pin4, pin6, pin7, pin9 to microcontroller I/O pins.            
//
// ----------------------------------------------------------------------------- 
// Usage:
//
// Set correcponding pin1, pin2, pin3, pin4, pin6, pin7, pin9 numbers in SegaGamepad::SegaGamepad() constructor.
// Optionally set custom delayBeforeReadMicros and delayBeforeNextUpdateMicros delays.
// Example:
//      SegaGamepad segaGamepad(1, 2, 3, 4, 5, 6, 7);
//
// Call SegaGamepad::init() method in setup() procedure to initialize microcontroller ports for gamepad. 
// Example: 
//      segaGamepad.init();
//
// Call SegaGamepad::update() method to read new values from controller.
// Then you can keys get values from segaGamepad object fields. True value means corresponding key is pressed.
// You also can check if controller is connected by reading isConnected flag.
// And you can check type of controller (three or six buttons) by reading isSixBtns flag.
// When controller is not connected then isConnected flag is set to False, 
//      all buttons values are set to False, is isSixBtns flag is set to False.
// Example: 
//      segaGamepad.update();
//      Serial.println(segaGamepad.isConnected);
//      Serial.println(segaGamepad.btnUp);
//      Serial.println(segaGamepad.btnA);
//      Serial.println(segaGamepad.btnStart);
//

#ifndef SegaGamepad_h
#define SegaGamepad_h

#include <Arduino.h>

class SegaGamepad {
    public:
        SegaGamepad(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, uint8_t pin6, uint8_t pin7, uint8_t pin9, unsigned int delayBeforeReadMicros = 10, unsigned int delayBeforeNextUpdateMicros = 2000);
        void init();
        void update();

        bool isConnected = false;
        bool isSixBtns = false;

        bool btnUp = false;
        bool btnDown = false;
        bool btnLeft = false;
        bool btnRight = false;
        bool btnA = false;
        bool btnB = false;
        bool btnC = false;
        bool btnX = false;
        bool btnY = false;
        bool btnZ = false;
        bool btnStart = false;
        bool btnMode = false;

    private:
        uint8_t pin1;
        uint8_t pin2;
        uint8_t pin3;
        uint8_t pin4;
        uint8_t pin6;
        uint8_t pinSelect;
        uint8_t pin9;
        unsigned int delayBeforeReadMicros;
        unsigned int delayBeforeNextUpdateMicros;

        unsigned long previousUpdateTime = 0;
};

#endif
