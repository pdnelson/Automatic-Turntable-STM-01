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

        /**
         * Constructor to instantiate an instance of this class.
         */
        StmShift();

        /**
         * Initializes base values. Call this in your `setup` routine.
         */
        void initialize();

        /**
         * Monitors the values that should be written to the shift register, and writes them if a value needs to be changed. Place this at the end of your `loop`.
         */
        void monitor();

        /**
         * Schedules a shift register pin to be set to the `value` next time `monitor` is called.
         */
        void setValue(StmShiftPin pin, bool value);

        /**
         * Gets the requested shift register pin's value.
         */
        bool getValue(StmShiftPin pin);

        /**
         * Updates all the shift register's values.
         */
        void setValues(uint16_t newValues);

        /**
         * Gets all the values currently stored in the shift register.
         */
        uint16_t getValues();

    private:
        /**
         * The current pin values that the shift register has set. This always has the most up-to-date data that the shift register is displaying.
         */
        uint16_t currentValue;

        /**
         * Scheduled value changes to the shift register get stored in here, via the public `setValue` method.
         */
        uint16_t nextValue;
 };

 #endif