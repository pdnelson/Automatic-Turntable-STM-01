#include <Settings.h>
#include <EEPROM.h>
#include <RecordSize.h>

Settings::Settings() : BasePersistence() {
    // Do nothing (For now...)
}

void Settings::load() {
    size = (RecordSize)EEPROM.read(EEPROM_SELECTED_SIZE_ADDR);
    speed = readFloat(EEPROM_SELECTED_SPEED_START_ADDR);
}

void Settings::persist() {
    writeOneByteValueIfChanged(EEPROM_SELECTED_SIZE_ADDR, size);
    writeFloatIfChanged(EEPROM_SELECTED_SPEED_START_ADDR, speed);
}