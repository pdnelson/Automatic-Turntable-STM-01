#include <Arduino.h>
#include <StmEncoderPolarity.h>

#ifndef CalibrationValues_h
#define CalibrationValues_h

class CalibrationValues {
    public:
        CalibrationValues();

        void load();

        void persist();

        // Basic tonearm setup
        uint16_t home = 0;
        uint16_t verticalUpperLimit;
        uint16_t verticalLowerLimit;
        StmEncoderPolarity polarity;
        uint8_t verticalUp;
        uint8_t verticalDown;
        uint8_t horizontalClockwise;
        uint8_t horizontalCounterclockwise;

        // Record sizes
        uint16_t in7 = 0;
        uint16_t in10 = 0;
        uint16_t in12 = 0;
};

#endif