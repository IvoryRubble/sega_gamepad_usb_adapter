#pragma once
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
        uint8_t pinSelect;
        uint8_t pin1;
        uint8_t pin2;
        uint8_t pin3;
        uint8_t pin4;
        uint8_t pin6;
        uint8_t pin9;
        unsigned int delayBeforeReadMicros;
        unsigned int delayBeforeNextUpdateMicros;

        unsigned long previousUpdateTime = 0;
};
