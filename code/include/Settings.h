#include <Arduino.h>
#include <BasePersistence.h>
#include <RecordSize.h>

#ifndef Settings_h
#define Settings_h

#define EEPROM_SELECTED_SIZE_ADDR 16
#define EEPROM_SELECTED_SPEED_START_ADDR 17

class Settings : public BasePersistence {
    public:
        Settings();

        void load() override;

        void persist() override;

        RecordSize size = RecordSize::InAuto;
        float speed = 0.0;
};

#endif