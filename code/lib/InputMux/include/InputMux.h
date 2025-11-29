#include <Arduino.h>
#include "ButtonResult.h"

#ifndef INPUTMUX_H
#define INPUTMUX_H

class InputMux {
    public:
        /**
         * Constructor for 1-channel multiplexers.
         * 
         * @param muxA The multiplexer's 'A' select pin.
         * @param muxOutput The pin connected to the multiplexer's output pin.
         * @param propagationDelayMicros The propagation delay, in microseconds, between when the select pin is set, and the mux output can be read.
         * This can also be used to force the routine to not read the multiplexer's values as often, allowing the CPU to dedicate more resources to other
         * operations.
         * @param holdInterval The amount of time, in milliseconds, that a button must be held in order to trigger the `ButtonResult::Held` status.
         * @param debounceIntervalMs The amount of time, in milliseconds, that must elapse in between button presses and releases for a change to be registered.
         */
        InputMux(
            uint8_t muxA,
            uint8_t muxOutput, 
            uint8_t propagationDelayMicros,
            uint16_t holdIntervalMs,
            uint8_t debounceIntervalMs
        );

        /**
         * Constructor for 2-channel multiplexers.
         * 
         * @param muxA The multiplexer's 'A' select pin.
         * @param muxB The multiplexer's 'B' select pin.
         * @param muxOutput The pin connected to the multiplexer's output pin.
         * @param propagationDelayMicros The propagation delay, in microseconds, between when the select pins are set, and the mux output can be read.
         * This can also be used to force the routine to not read the multiplexer's values as often, allowing the CPU to dedicate more resources to other
         * operations.
         * @param holdInterval The amount of time, in milliseconds, that a button must be held in order to trigger the `ButtonResult::Held` status.
         * @param debounceIntervalMs The amount of time, in milliseconds, that must elapse in between button presses and releases for a change to be registered.
         */
        InputMux(
            uint8_t muxA, 
            uint8_t muxB, 
            uint8_t muxOutput, 
            uint8_t propagationDelayMicros,
            uint16_t holdIntervalMs,
            uint8_t debounceIntervalMs
        );

        /**
         * Constructor for 3-channel multiplexers.
         * 
         * @param muxA The multiplexer's 'A' select pin.
         * @param muxB The multiplexer's 'B' select pin.
         * @param muxC The multiplexer's 'C' select pin.
         * @param muxOutput The pin connected to the multiplexer's output pin.
         * @param propagationDelayMicros The propagation delay, in microseconds, between when the select pins are set, and the mux output can be read.
         * This can also be used to force the routine to not read the multiplexer's values as often, allowing the CPU to dedicate more resources to other
         * operations.
         * @param holdInterval The amount of time, in milliseconds, that a button must be held in order to trigger the `ButtonResult::Held` status.
         * @param debounceIntervalMs The amount of time, in milliseconds, that must elapse in between button presses and releases for a change to be registered.
         */
        InputMux(
            uint8_t muxA, 
            uint8_t muxB, 
            uint8_t muxC, 
            uint8_t muxOutput, 
            uint8_t propagationDelayMicros,
            uint16_t holdIntervalMs,
            uint8_t debounceIntervalMs
        );

        /**
         * Constructor for 4-channel multiplexers.
         * 
         * @param muxA The multiplexer's 'A' select pin.
         * @param muxB The multiplexer's 'B' select pin.
         * @param muxC The multiplexer's 'C' select pin.
         * @param muxD The multiplexer's 'D' select pin.
         * @param muxOutput The pin connected to the multiplexer's output pin.
         * @param propagationDelayMicros The propagation delay, in microseconds, between when the select pins are set, and the mux output can be read.
         * This can also be used to force the routine to not read the multiplexer's values as often, allowing the CPU to dedicate more resources to other
         * operations.
         * @param holdInterval The amount of time, in milliseconds, that a button must be held in order to trigger the `ButtonResult::Held` status.
         * @param debounceIntervalMs The amount of time, in milliseconds, that must elapse in between button presses and releases for a change to be registered.
         */
        InputMux(
            uint8_t muxA, 
            uint8_t muxB, 
            uint8_t muxC, 
            uint8_t muxD, 
            uint8_t muxOutput, 
            uint8_t propagationDelayMicros,
            uint16_t holdIntervalMs,
            uint8_t debounceIntervalMs
        );

        /**
         * This method monitors a different mux pin on every iteration. This should be dropped in your `loop()` method.
         * 
         * @param clockMicros The current clock's microseconds. Overflow is automatically handled in this routine.
         */
        void monitor(unsigned long clockMicros);

        /**
         * Get a button value corresponding to a particular mux index.
         * 
         * @param index The index of the button you want the value of.
         */
        ButtonResult getValue(uint8_t index);

        /**
         * Release memory used by this object. Only use this if the multiplexer is no longer needed for the rest of the runtime of the program.
         */
        void releaseMemory();

    private:

        /**
         * The multiplexer's 'A' select pin.
         */
        uint8_t muxA;

        /**
         * The multiplexer's 'B' select pin.
         */
        uint8_t muxB;

        /**
         * The multiplexer's 'C' select pin.
         */
        uint8_t muxC;

        /**
         * The multiplexer's 'D' select pin.
         */
        uint8_t muxD;

        /**
         * The multiplexer's output pin.
         */
        uint8_t muxOutput;

        /**
         * The amount of time, in microseconds to wait between setting select pins and reading the output value of the mux.
         */
        uint8_t propagationDelay;

        /**
         * The amount of time, in microseconds, that a button must be held to transition into the `ButtonResult::Held` status.
         */
        unsigned long holdInterval;

        /**
         * The amount of time, in microseconds, that must elapse in between button presses and releases for a change to be registered.
         */
        unsigned long debounceInterval;

        /**
         * The number of multiplexer input pins.
         */
        uint8_t arraySize;

        /**
         * The number of multiplexer select pins.
         */
        uint8_t selectorCount;

        /**
         * Allocates memory based on the number of multiplexer's input pins.
         */
        void allocateMemory(uint8_t arraySize);

        /**
         * Initialize base values that are common between all constructors.
         */
        void initializeBaseValues(uint8_t muxA, uint8_t muxB, uint8_t muxC, uint8_t muxD, uint8_t muxOutput, uint8_t selectorCount, uint8_t propagationDelay, uint16_t holdInterval, uint8_t debounceInterval);
        
        /**
         * Sets an output index based on the current value.
         */
        void setValue(uint8_t index, ButtonResult value);

        /**
         * The time at which a button press began. This is an array, so the current relevant value will correspond to the index in `currentIndex`.
         */
        unsigned long* inputPressMicros;

        /**
         * The current button's status. This is an array, so the current relevant value will correspond to the index in `currentIndex`.
         */
        ButtonResult* currentValue;

        /**
         * The current index of the button being read and updated from the multiplexer.
         */
        uint8_t currentIndex;

        /**
         * The last time the monitor routine was allowed to run. This is used, in conjunction with the `propagationDelay` value, to only read multiplexer
         * data at a certain interval.
         */
        unsigned long lastIterationClockMicros;
};

#endif