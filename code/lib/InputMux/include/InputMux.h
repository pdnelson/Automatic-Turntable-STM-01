#include <Arduino.h>
#include "ButtonResult.h"

#ifndef INPUTMUX_H
#define INPUTMUX_H

class InputMux {
    public:
        // 1-channel constructor
        InputMux(
            uint8_t muxA,
            uint8_t muxOutput, 
            uint8_t propagationDelayMicros,
            uint16_t holdInterval,
            uint8_t debounceInterval
        );

        // 2-channel constructor
        InputMux(
            uint8_t muxA, 
            uint8_t muxB, 
            uint8_t muxOutput, 
            uint8_t propagationDelayMicros,
            uint16_t holdInterval,
            uint8_t debounceInterval
        );

        // 3-channel constructor
        InputMux(
            uint8_t muxA, 
            uint8_t muxB, 
            uint8_t muxC, 
            uint8_t muxOutput, 
            uint8_t propagationDelayMicros,
            uint16_t holdIntervalMs,
            uint8_t debounceIntervalMs
        );

        // 4-channel constructor
        InputMux(
            uint8_t muxA, 
            uint8_t muxB, 
            uint8_t muxC, 
            uint8_t muxD, 
            uint8_t muxOutput, 
            uint8_t propagationDelayMicros,
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

        // All these intervals are in microseconds.
        uint8_t propagationDelay;
        unsigned long holdInterval;
        unsigned long debounceInterval;

        uint8_t arraySize;
        uint8_t selectorCount;

        void allocateMemory(uint8_t arraySize);
        void initializeBaseValues(uint8_t muxA, uint8_t muxB, uint8_t muxC, uint8_t muxD, uint8_t muxOutput, uint8_t selectorCount, uint8_t propagationDelay, uint16_t holdInterval, uint8_t debounceInterval);
        void setValue(uint8_t index, ButtonResult value);

        unsigned long* inputPressMicros;
        ButtonResult* currentValue;
        uint8_t currentIndex;
        unsigned long lastIterationClockMicros;
};

#endif