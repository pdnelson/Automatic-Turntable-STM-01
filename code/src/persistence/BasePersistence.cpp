#include <BasePersistence.h>
#include <EEPROM.h>

BasePersistence::BasePersistence() {
    /* do nothin' */
}

void BasePersistence::writeOneByteValueIfChanged(uint8_t address, uint8_t newValue) {
    uint8_t old = EEPROM.read(address);
    if(old != newValue) {
        EEPROM.write(address, newValue);
    }
}

void BasePersistence::writeTwoByteValueIfChanged(uint8_t startAddress, uint16_t newValue) {
    writeOneByteValueIfChanged(startAddress, newValue & 0xFF);
    writeOneByteValueIfChanged(startAddress + 1, (newValue >> 8) & 0xFF);
}

void BasePersistence::writeFloatIfChanged(uint8_t startAddress, float newValue) {
    byte const* data = reinterpret_cast<byte const*>(&newValue);

    writeOneByteValueIfChanged(startAddress, data[0]);
    writeOneByteValueIfChanged(startAddress + 1, data[1]);
    writeOneByteValueIfChanged(startAddress + 2, data[2]);
    writeOneByteValueIfChanged(startAddress + 3, data[3]);
}

uint16_t BasePersistence::readUInt16(uint8_t startAddress) {
    return (EEPROM.read(startAddress) & 0x00FF) | ((EEPROM.read(startAddress + 1) << 8) & 0xFF00);
}

float BasePersistence::readFloat(uint8_t startAddress) {

    uint32_t dataCombined = (EEPROM.read(startAddress) & 0x000000FF) |
        ((EEPROM.read(startAddress + 1) << 8) & 0x0000FF00) |
        ((EEPROM.read(startAddress + 2) << 16) & 0x00FF0000) |
        ((EEPROM.read(startAddress + 3) << 24) & 0xFF000000);

    float* finalSpeed = reinterpret_cast<float*>((void*) &dataCombined);

    return *finalSpeed;
}