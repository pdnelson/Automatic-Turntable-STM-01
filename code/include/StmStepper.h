#include <Arduino.h>
#include <Pin.h>

#ifndef STMSTEPPER_H
#define STMSTEPPER_H

#define STEPPER_STEPS_PER_REVOLUTION 2048

class StmStepper {
    public:
        StmStepper(Pin pin1, Pin pin2, Pin pin3, Pin pin4);

        void setSpeed(uint8_t speedRpm);

        void setRampUpEncoderTicks(uint16_t rampUp);
        void setRampDownEncoderTicks(uint16_t rampDown);

        void setDestinationEncoderPosition(uint16_t destinationEncoderPosition);
        void setDestinationEncoderPositionDelta(uint16_t destinationEncoderPositionDelta);

        void setDirection(int8_t direction);

        bool step(unsigned long clockMicros);

        void releaseMotorCurrent();

    private:
        Pin pin1;
        Pin pin2;
        Pin pin3;
        Pin pin4;

        uint16_t topSpeedTimeBetweenStepsMicros = 0;
        uint16_t currentTimeBetweenStepsMicros = 0;

        uint16_t rampUpEncoderTicks = 0;
        uint16_t rampDownEncoderTicks = 0;

        uint16_t destinationEncoderPosition = 0;
        uint8_t destinationEncoderPositionDelta = 0;

        int8_t direction = 1;

        uint8_t currentStep = 0;

        bool rampingUp = false;

        unsigned long lastStepMicros = 0;

        void performStep();
};

#endif