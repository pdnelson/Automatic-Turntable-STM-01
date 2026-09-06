#include <Arduino.h>
#include <StmEncoderPolarity.h>

#ifndef CalibrationValues_h
#define CalibrationValues_h

class CalibrationValues {
    public:
        CalibrationValues();

        void load();

        void persist();

        uint16_t home = 0;
        uint16_t in7 = 0;
        uint16_t in10 = 0;
        uint16_t in12 = 0;
        uint16_t verticalUpperLimit;
        uint16_t verticalLowerLimit;
        StmEncoderPolarity polarity;
};

#endif