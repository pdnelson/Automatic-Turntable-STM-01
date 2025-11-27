#include <Arduino.h>
#include "ButtonResult.h"

#ifndef INPUTMUX_H
#define INPUTMUX_H

class InputMux {
    public:
        // 2-channel constructor
        InputMux(
            uint8_t muxA, 
            uint8_t muxB, 
            uint8_t muxOutput, 
            uint8_t propagationDelay,
            uint16_t holdInterval,
            uint8_t debounceInterval
        );

        // 3-channel constructor
        InputMux(
            uint8_t muxA, 
            uint8_t muxB, 
            uint8_t muxC, 
            uint8_t muxOutput, 
            uint8_t propagationDelay,
            uint16_t holdInterval,
            uint8_t debounceInterval
        );

        // 4-channel constructor
        InputMux(
            uint8_t muxA, 
            uint8_t muxB, 
            uint8_t muxC, 
            uint8_t muxD, 
            uint8_t muxOutput, 
            uint8_t propagationDelay,
            uint16_t holdInterval,
            uint8_t debounceInterval
        );

        void monitor(unsigned long clockMicros);

        ButtonResult getValue(uint8_t index);

        void releaseMemory();

    private:
        uint8_t muxA;
        uint8_t muxB;
        uint8_t muxC;
        uint8_t muxD;
        uint8_t muxOutput;
        uint8_t propagationDelay;
        uint16_t holdInterval;

        uint8_t arraySize;
        uint8_t selectorCount;

        void allocateMemory(uint8_t arraySize);

        unsigned long* inputLastMs;
        boolean* inputLast;
        boolean* inputCurrent;
        uint8_t currentIndex;
};

#endif