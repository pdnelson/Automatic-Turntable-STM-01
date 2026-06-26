#include <StmEncoder.h>
#include <Wire.h>

StmEncoder::StmEncoder(uint8_t sda, uint8_t scl) {
    pinMode(sda, OUTPUT);
    pinMode(scl, OUTPUT);

    Wire2.begin();
}

uint16_t StmEncoder::getPosition() {
    Wire2.beginTransmission(ENCODER_START_ADDR);
    Wire2.write(ENCODER_POSITION_START_ADDR);

    // Calculation adapted from sosandroid's AMS_AS5048B library
    uint16_t finalValue = ((uint16_t) Wire2.read()) << 6;
    finalValue += (Wire2.read() & 0x3F);

    Wire2.endTransmission();

    return finalValue;
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