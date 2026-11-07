#include <Arduino.h>
#include <StmPolarity.h>
#include <BasePersistence.h>

#ifndef CalibrationValues_h
#define CalibrationValues_h

class CalibrationValues : public BasePersistence {
    public:
        CalibrationValues();

        void load() override;

        void persist() override;

        // Basic tonearm setup
        uint16_t home = 0;
        uint16_t verticalUpperLimit = 0;
        uint16_t verticalLowerLimit = 0;
        StmPolarity hStepperPolarity = StmPolarity::Normal;
        StmPolarity polarityH = StmPolarity::Normal;
        StmPolarity polarityV = StmPolarity::Normal;
        uint16_t azEncoderOffset = 0;

        // Record sizes
        uint16_t in7 = 0;
        uint16_t in10 = 0;
        uint16_t in12 = 0;
};

#endif