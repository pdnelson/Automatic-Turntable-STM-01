#include <Arduino.h>
#include <StmPolarity.h>
#include <BasePersistence.h>

#ifndef CalibrationValues_h
#define CalibrationValues_h

#define EEPROM_HOME_START_ADDR 0
#define EEPROM_V_UPPER_LIMIT_START_ADDR 2
#define EEPROM_V_LOWER_LIMIT_START_ADDR 4
#define EEPROM_POLARITY_H_ADDR 6
#define EEPROM_POLARITY_V_ADDR 7
#define EEPROM_7_IN_START_ADDR 8
#define EEPROM_10_IN_START_ADDR 10
#define EEPROM_12_IN_START_ADDR 12
#define EEPROM_AZ_OFFSET 14

class CalibrationValues : public BasePersistence {
    public:
        CalibrationValues();

        void load() override;

        void persist() override;

        // Basic tonearm setup
        uint16_t home = 0;
        uint16_t verticalUpperLimit = 0;
        uint16_t verticalLowerLimit = 0;
        StmPolarity motorPolarity = StmPolarity::Normal;
        StmPolarity polarityH = StmPolarity::Normal;
        StmPolarity polarityV = StmPolarity::Normal;
        uint16_t azEncoderOffset = 0;

        // Record sizes
        uint16_t in7 = 0;
        uint16_t in10 = 0;
        uint16_t in12 = 0;
};

#endif