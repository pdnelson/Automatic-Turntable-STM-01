#include <Arduino.h>
#include "StmShiftPin.h"

/**
 * 2026 Patrick Nelson (Statimatic Science)
 * 
 * This is a wrapper class to handle I2C transmissions for the STM-01 turntable. This allows us to set, and monitor
 * values easily, without bothering the actual shift register too much, since writing to it is expensive.
 * 
 * Eventually, it would be nice to eliminate Wire usage in favor of an implementation geared more toward the specific requirements
 * of this project. But, I doubt I'll do this any time soon (or at all).
 */

 #ifndef STMSHIFT_H
 #define STMSHIFT_H

 class StmShift {
    public:
        StmShift();

        void initialize();
        void monitor();

        void setValue(StmShiftPin pin, bool value);
        bool getValue(StmShiftPin pin);

    private:
        uint16_t currentValue;
        uint16_t nextValue;
 };

 #endif