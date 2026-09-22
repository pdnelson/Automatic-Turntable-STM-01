#include <Arduino.h>
#include <StmEncoder.h>
#include <Pin.h>
#include <StmPolarity.h>
#include <AzimuthDirection.h>

#ifndef STMHSTEPPER_H
#define STMHSTEPPER_H

#define PULSE_WIDTH_MICROS 25
#define STEPS_PER_REVOLUTION 6400 // 1/16 step

#define TRAPEZOIDAL_PROFILE_STEPS 50

class StmHStepper {
    public:
        StmHStepper(StmEncoder &hEncoder, Pin enablePin, Pin sleepPin, Pin stepPin, Pin directionPin);

        void setPolarity(StmPolarity polarity);
        void setDirection(AzimuthDirection direction);
        void setSpeed(float speed);
        void setMovementProfile(AzimuthDirection direction, float speed, uint16_t steps);

        bool stepBlind(unsigned long clockMicros);
        bool stepTrapezoidally(unsigned long clockMicros);
        void wake();
        void sleep();
        
    private:
        StmEncoder &hEncoder;
        Pin enablePin;
        Pin sleepPin;
        Pin stepPin;
        Pin directionPin;
        StmPolarity polarity = StmPolarity::Normal;
        AzimuthDirection direction = AzimuthDirection::Clockwise;
        bool takingStep = false;
        unsigned long lastStepMicros = 0;
        unsigned long topSpeedTimeBetweenStepsMicros = 0;
        unsigned long currSpeedBetweenStepsMicros = 0;
        uint16_t destinationMovementSteps = 0;
        uint16_t currentMovementSteps = 0;


        void performStep();
};

#endif