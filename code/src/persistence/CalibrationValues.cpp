#include <CalibrationValues.h>
#include <Constants.h>
#include <VerticalDirection.h>
#include <AzimuthDirection.h>
#include <EEPROM.h>

CalibrationValues::CalibrationValues() {
    // Do nothing (For now...)
}

void CalibrationValues::load() {
    // to do: load values
}

void CalibrationValues::persist() {
    byte oldHomeByte1 = EEPROM.read(EEPROM_HOME_START_ADDR);
    if(oldHomeByte1 != (byte)(home & 0xFF)) {
        EEPROM.write(EEPROM_HOME_START_ADDR, home);
    }
}

void CalibrationValues::writeOneByteValueIfChanged(uint8_t address, uint8_t newValue) {
    uint8_t old = EEPROM.read(address);
    if(old != newValue) {
        EEPROM.write(address, newValue);
    }
}

void CalibrationValues::writeTwoByteValueIfChanged(uint8_t startAddress, uint16_t newValue) {
    writeOneByteValueIfChanged(startAddress, newValue & 0xFF);
    writeOneByteValueIfChanged(startAddress, (newValue >> 8) & 0xFF);
}

uint16_t CalibrationValues::readUInt16(uint8_t startAddress) {
    return 0;//EEPROM.read(startAddress) &
}