#include <Arduino.h>
#include <StmEncoderPolarity.h>

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

class CalibrationValues {
    public:
        CalibrationValues();

        void load();

        void persist();

        /**
         * Calibration values
         */

        // Basic tonearm setup
        uint16_t home = 0;
        uint16_t verticalUpperLimit = 0;
        uint16_t verticalLowerLimit = 0;
        StmEncoderPolarity polarityH = StmEncoderPolarity::NORMAL;
        StmEncoderPolarity polarityV = StmEncoderPolarity::NORMAL;
        uint16_t azEncoderOffset = 0;

        // Record sizes
        uint16_t in7 = 0;
        uint16_t in10 = 0;
        uint16_t in12 = 0;

    private:
        void writeOneByteValueIfChanged(uint8_t address, uint8_t newValue);
        void writeTwoByteValueIfChanged(uint8_t startAddress, uint16_t newValue);
        uint16_t readUInt16(uint8_t startAddress);
};

#endif