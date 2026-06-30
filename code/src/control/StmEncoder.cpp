#include <StmEncoder.h>
#include <Wire.h>

StmEncoder::StmEncoder(uint8_t sda, uint8_t scl) {
    pinMode(sda, OUTPUT);
    pinMode(scl, OUTPUT);

    Wire2.begin();

    // Fill out encoder data
    data[0] = getPosition();
    data[1] = getPosition();
    data[2] = getPosition();
    data[3] = getPosition();
    data[4] = getPosition();
    data[5] = getPosition();
    data[6] = getPosition();
    data[7] = getPosition();
    data[8] = getPosition();
    data[9] = getPosition();
}

void StmEncoder::monitor(unsigned long clockMicros) {
    if(clockMicros - dataCollectCounter > ENCODER_DATA_POINT_INTERVAL_MICROS) {
        dataCollectCounter = clockMicros;
        logData();
    }

    if(clockMicros - dataAvgCounter > ENCODER_AVG_INTERVAL_MICROS) {
        dataAvgCounter = clockMicros;
        computeAverage();
    }
}

uint16_t StmEncoder::getNormalizedPosition() {
    return rollingDataAvg;
}

uint16_t StmEncoder::getPosition() {
    Wire2.beginTransmission(ENCODER_START_ADDR);
    Wire2.write(ENCODER_POSITION_START_ADDR);
    uint8_t result = Wire2.endTransmission(false);
    if(result > 0) {
        return 0;
    }

    Wire2.requestFrom(ENCODER_START_ADDR, 2);

    // Calculation adapted from sosandroid's AMS_AS5048B library
    uint16_t finalValue = ((uint16_t) Wire2.read()) << 6;
    finalValue += (Wire2.read() & 0x3F);

    return finalValue;
}

void StmEncoder::logData() {
    uint16_t newData = getPosition();

    // Only log the new data if it's not 0, because 0 is an erroneous value that we don't want to ruin the average.
    if(newData != 0) {
        // This is NOT looped through in order to avoid the overhead of C++ looping.
        data[0] = data[1];
        data[1] = data[2];
        data[2] = data[3];
        data[3] = data[4];
        data[4] = data[5];
        data[5] = data[6];
        data[6] = data[7];
        data[7] = data[8];
        data[8] = data[9];
        data[9] = newData;
    }
}

void StmEncoder::computeAverage() {
    rollingDataAvg = (data[0] + data[1] + data[2] + data[3] + data[4] + data[5] + data[6] + data[7] + data[8] + data[9]) / ENCODER_AVG_RESOLUTION;
}

void StmEncoder::zeroOutEncoder() {
    // Zero out the existing offset before applying a new one
    setZero(0);
    setZero(getPosition());
}

void StmEncoder::setZero(uint16_t offset) {
    // MSB value
    Wire2.beginTransmission(ENCODER_START_ADDR);
    Wire2.write(ENCODER_ZERO_MSB_ADDR);
    Wire2.write((uint8_t)(offset >> 6));
    Wire2.endTransmission();

    // LSB value
    Wire2.beginTransmission(ENCODER_START_ADDR);
    Wire2.write(ENCODER_ZERO_LSB_ADDR);
    Wire2.write((uint8_t)(offset & 0x3F));
    Wire2.endTransmission();
}