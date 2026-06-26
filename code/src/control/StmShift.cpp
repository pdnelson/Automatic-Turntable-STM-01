#include <StmShift.h>
#include <Wire.h>

StmShift::StmShift(uint8_t sda, uint8_t scl) {
    currentValue = 0x0000;
    nextValue = 0x0000;
    
    pinMode(sda, OUTPUT);
    pinMode(scl, OUTPUT);

    Wire2.begin();

    Wire2.beginTransmission(SHIFT_START_ADDR);
    Wire2.write(BANK_A_MODE);
    Wire2.write(OUTPUT_MODE);
    Wire2.endTransmission();

    Wire2.beginTransmission(SHIFT_START_ADDR);
    Wire2.write(BANK_B_MODE);
    Wire2.write(OUTPUT_MODE);
    Wire2.endTransmission();

    Wire2.beginTransmission(SHIFT_START_ADDR);
    Wire2.write(BANK_A_WRITE);
    Wire2.write(CLEAR_BANK);
    Wire2.endTransmission();

    Wire2.beginTransmission(SHIFT_START_ADDR);
    Wire2.write(BANK_B_WRITE);
    Wire2.write(CLEAR_BANK);
    Wire2.endTransmission();
}

void StmShift::monitor() {
  // If what's currently on the shift doesn't match what's "next," then update it.
  if(nextValue != currentValue) {

    uint8_t bankACurrValue = currentValue & 0x00FF;
    uint8_t bankANextValue = nextValue & 0x00FF;

    // Rewrite bank A if it has changed at all
    if(bankACurrValue != bankANextValue) {
      Wire2.beginTransmission(SHIFT_START_ADDR);
      Wire2.write(BANK_A_WRITE);
      Wire2.write(bankANextValue);
      Wire2.endTransmission();
    }

    uint8_t bankBCurrValue = (currentValue & 0xFF00) >> 8;
    uint8_t bankBNextValue = (nextValue & 0xFF00) >> 8;

    // Rewrite bank B if it has changed at all
    if(bankBCurrValue != bankBNextValue) {
      Wire2.beginTransmission(SHIFT_START_ADDR);
      Wire2.write(BANK_B_WRITE);
      Wire2.write(bankBNextValue);
      Wire2.endTransmission();
    }

    currentValue = nextValue;
  }
}

void StmShift::setValue(StmShiftPin pin, bool value) {
  if(getValue(pin) != value) {
    nextValue ^= 1 << pin;
  }
}

bool StmShift::getValue(StmShiftPin pin) {
  return (nextValue >> pin) & 1;
}

void StmShift::setValues(uint16_t newValues) {
  nextValue = newValues;
}

uint16_t StmShift::getValues() {
  return nextValue;
}