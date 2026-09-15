#include <Arduino.h>

#ifndef BasePersistence_h
#define BasePersistence_h

class BasePersistence {
    public:
        BasePersistence();

        virtual void load() = 0;
        virtual void persist() = 0;

        void writeOneByteValueIfChanged(uint8_t address, uint8_t newValue);
        void writeTwoByteValueIfChanged(uint8_t startAddress, uint16_t newValue);
        void writeFloatIfChanged(uint8_t startAddress, float newValue);
        uint16_t readUInt16(uint8_t startAddress);
        float readFloat(uint8_t startAddress);
};

#endif