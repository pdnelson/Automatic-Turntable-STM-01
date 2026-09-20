#include <Arduino.h>
#include <StmEncoder.h>
#include <Pin.h>
#include <StmPolarity.h>

#ifndef STMHSTEPPER_H
#define STMHSTEPPER_H

class StmHStepper {
    public:
        StmHStepper(StmEncoder &hEncoder, Pin enablePin, Pin stepPin, Pin directionPin);

        void setPolarity(StmPolarity polarity);
    private:
        StmEncoder &hEncoder;
        Pin enablePin;
        Pin stepPin;
        Pin directionPin;
        StmPolarity polarity;
};

#endif