#include <StmShift.h>
#include <Wire.h> 
#include "StmShiftConstants.h"

StmShift::StmShift() {
    this->currentValue = 0x0000;
    this->nextValue = 0x0000;
}

void StmShift::initialize() {
  Wire2.begin();

  Wire2.beginTransmission(START_ADDR);
  Wire2.write(BANK_A_MODE);
  Wire2.write(OUTPUT_MODE);
  Wire2.endTransmission();

  Wire2.beginTransmission(START_ADDR);
  Wire2.write(BANK_B_MODE);
  Wire2.write(OUTPUT_MODE);
  Wire2.endTransmission();

  Wire2.beginTransmission(START_ADDR);
  Wire2.write(BANK_A_WRITE);
  Wire2.write(CLEAR_BANK);
  Wire2.endTransmission();

  Wire2.beginTransmission(START_ADDR);
  Wire2.write(BANK_B_WRITE);
  Wire2.write(CLEAR_BANK);
  Wire2.endTransmission();
}

void StmShift::monitor() {
  // If what's currently on the shift doesn't match what's "next," then update it.
  if(this->nextValue != this->currentValue) {

    uint8_t bankACurrValue = this->currentValue & 0x00FF;
    uint8_t bankANextValue = this->nextValue & 0x00FF;

    // Rewrite bank A if it has changed at all
    if(bankACurrValue != bankANextValue) {
      Wire2.beginTransmission(START_ADDR);
      Wire2.write(BANK_A_WRITE);
      Wire2.write(bankANextValue);
      Wire2.endTransmission();
    }

    uint8_t bankBCurrValue = (this->currentValue & 0xFF00) >> 8;
    uint8_t bankBNextValue = (this->nextValue & 0xFF00) >> 8;

    // Rewrite bank B if it has changed at all
    if(bankBCurrValue != bankBNextValue) {
      Wire2.beginTransmission(START_ADDR);
      Wire2.write(BANK_B_WRITE);
      Wire2.write(bankBNextValue);
      Wire2.endTransmission();
    }

    this->currentValue = this->nextValue;
  }
}

void StmShift::setValue(StmShiftPin pin, bool value) {
  if(this->getValue(pin) != value) {
    this->nextValue ^= 1 << pin;
  }
}

bool StmShift::getValue(StmShiftPin pin) {
  return (this->nextValue >> pin) & 1;
}