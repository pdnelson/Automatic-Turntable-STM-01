#include <CalibrationValues.h>
#include <Constants.h>
#include <VerticalDirection.h>
#include <AzimuthDirection.h>
#include <EEPROM.h>
#include <StmEncoderPolarity.h>
#include <BasePersistence.h>

CalibrationValues::CalibrationValues() : BasePersistence() {
    // Do nothing (For now...)
}

void CalibrationValues::load() {
    home = readUInt16(EEPROM_HOME_START_ADDR);
    verticalUpperLimit = readUInt16(EEPROM_V_UPPER_LIMIT_START_ADDR);
    verticalLowerLimit = readUInt16(EEPROM_V_LOWER_LIMIT_START_ADDR);
    polarityH = (StmEncoderPolarity)EEPROM.read(EEPROM_POLARITY_H_ADDR);
    polarityV = (StmEncoderPolarity)EEPROM.read(EEPROM_POLARITY_V_ADDR);
    azEncoderOffset = readUInt16(EEPROM_AZ_OFFSET);
    in7 = readUInt16(EEPROM_7_IN_START_ADDR);
    in10 = readUInt16(EEPROM_10_IN_START_ADDR);
    in12 = readUInt16(EEPROM_12_IN_START_ADDR);
}

void CalibrationValues::persist() {
    writeTwoByteValueIfChanged(EEPROM_HOME_START_ADDR, home);
    writeTwoByteValueIfChanged(EEPROM_V_UPPER_LIMIT_START_ADDR, verticalUpperLimit);
    writeTwoByteValueIfChanged(EEPROM_V_LOWER_LIMIT_START_ADDR, verticalLowerLimit);
    writeOneByteValueIfChanged(EEPROM_POLARITY_H_ADDR, polarityH);
    writeOneByteValueIfChanged(EEPROM_POLARITY_V_ADDR, polarityV);
    writeTwoByteValueIfChanged(EEPROM_AZ_OFFSET, azEncoderOffset);
    writeTwoByteValueIfChanged(EEPROM_7_IN_START_ADDR, in7);
    writeTwoByteValueIfChanged(EEPROM_10_IN_START_ADDR, in10);
    writeTwoByteValueIfChanged(EEPROM_12_IN_START_ADDR, in12);
}