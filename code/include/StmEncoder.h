#include <Arduino.h>
#include <StmEncoderPolarity.h>

#ifndef STMENCODER_H
#define STMENCODER_H

/** Constants */
#define ENCODER_START_ADDR 0x40
#define ENCODER_POSITION_START_ADDR 0xFE
#define ENCODER_ZERO_MSB_ADDR 0x16
#define ENCODER_ZERO_LSB_ADDR 0x17

#define ENCODER_AVG_RESOLUTION 10
#define ENCODER_DATA_POINT_INTERVAL_MICROS 1000
#define ENCODER_AVG_INTERVAL_MICROS 5000

#define ENCODER_MAX_VALUE 16384

class StmEncoder {
    public:
        StmEncoder(uint8_t sda, uint8_t scl);

        void monitor(unsigned long clockMicros);
        void zeroOutEncoder();
        uint16_t getNormalizedPosition();
        void setPolarity(StmEncoderPolarity polarity);
    private:
        /**
         * Sets the current position to "zero"
         */
        void setZero(uint16_t offset);
        uint16_t getPosition();
        void logData();
        void computeAverage();

        unsigned long dataCollectCounter = 0;
        unsigned long dataAvgCounter = 0;
        uint16_t rollingDataAvg = 0;
        uint16_t data[ENCODER_AVG_RESOLUTION] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        StmEncoderPolarity polarity = StmEncoderPolarity::NORMAL;
};

#endif