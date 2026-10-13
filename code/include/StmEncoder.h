#include <Arduino.h>
#include <StmPolarity.h>
#include <EncoderPollSpeed.h>

#ifndef STMENCODER_H
#define STMENCODER_H

/** Constants */
#define ENCODER_START_ADDR 0x40
#define ENCODER_POSITION_START_ADDR 0xFE
#define ENCODER_ZERO_MSB_ADDR 0x16
#define ENCODER_ZERO_LSB_ADDR 0x17

#define ENCODER_AVG_RESOLUTION 20
#define ENCODER_DATA_POINT_INTERVAL_MICROS_NORMAL 25000
#define ENCODER_DATA_POINT_INTERVAL_MICROS_H_MOVEMENT_STANDARD 20000
#define ENCODER_DATA_POINT_INTERVAL_MICROS_H_ERROR_CORRECT 10

#define H_ENCODER_MAX_VALUE 16384
#define V_ENCODER_MAX_VALUE 1023

class StmEncoder {
    public:
        StmEncoder(uint8_t sda, uint8_t scl);

        void monitor(unsigned long clockMicros);
        void zeroOutEncoder(uint16_t offset);
        uint16_t getNormalizedPosition();
        uint16_t getPosition();
        void setPolarity(StmPolarity polarity);
        /**
         * Sets the current position to "zero"
         */
        void setZero(uint16_t offset);

        void setPollSpeed(EncoderPollSpeed newSpeed);
    private:
        void logData();
        void computeAverage();

        unsigned long dataCollectCounter = 0;
        uint16_t rollingDataAvg = 0;
        uint16_t data[ENCODER_AVG_RESOLUTION] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        StmPolarity polarity = StmPolarity::Normal;
        uint32_t pollInterval = ENCODER_DATA_POINT_INTERVAL_MICROS_NORMAL;
};

#endif