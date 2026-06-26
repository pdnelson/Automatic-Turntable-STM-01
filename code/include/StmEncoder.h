#include <Arduino.h>

#ifndef STMENCODER_H
#define STMENCODER_H

/** Constants */
#define ENCODER_START_ADDR 0x40
#define ENCODER_POSITION_START_ADDR 0xFE
#define ENCODER_ZERO_MSB_ADDR 0x16
#define ENCODER_ZERO_LSB_ADDR 0x17

class StmEncoder {
    public:
        StmEncoder(uint8_t sda, uint8_t scl);

        uint16_t getPosition();

        void zeroOutEncoder();
    private:
        /**
         * Sets the current position to "zero"
         */
        void setZero(uint16_t offset);
};

#endif