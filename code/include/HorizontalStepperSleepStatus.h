#include <Arduino.h>

#ifndef HORIZONTALSTEPPERSLEEPSTATUS_H
#define HORIZONTALSTEPPERSLEEPSTATUS_H

enum HorizontalStepperSleepStatus {
    Awake = HIGH,
    Asleep = LOW
};

#endif