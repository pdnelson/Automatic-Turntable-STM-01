#include <Arduino.h>
#include <Pin.h>
#include <StmStepperResult.h>

#ifndef STMSTEPPER_H
#define STMSTEPPER_H

#define STEPPER_STEPS_PER_REVOLUTION 2048

/**
 * The slowest the tonearm can move; this is a little less than 1 RPM.
 */
#define STEPPER_MAX_DELAY_BETWEEN_STEPS 30000

class StmStepper {
    public:
        StmStepper(Pin pin1, Pin pin2, Pin pin3, Pin pin4);

        void setSpeed(uint8_t speedRpm);

        void setRampUpEncoderTicks(uint16_t rampUp);
        void setRampDownEncoderTicks(uint16_t rampDown);

        // This automatically sets the direction
        void setEncoderRange(uint16_t start, uint16_t end, uint8_t tolerance);

        void setDirection(int8_t direction);

        // Step in accordance with the currentEncoderPosition.
        StmStepperResult step(unsigned long clockMicros, uint16_t currentEncoderPosition);

        // Step without acknowledging any encoder position. This will ignore any ramp up/ramp down logic.
        bool stepBlind(unsigned long clockMicros);

        void releaseMotorCurrent();

        // Methods we want to be able to unit test
        bool rampingDown(uint16_t currentEncoderPosition);
        bool rampingUp(uint16_t currentEncoderPosition);
        uint16_t rampDownSpeed(uint16_t currentEncoderPosition);
        uint16_t rampUpSpeed(uint16_t currentEncoderPosition);
        bool movementCompleted(uint16_t currentEncoderPosition);

    private:
        Pin pin1;
        Pin pin2;
        Pin pin3;
        Pin pin4;

        unsigned long topSpeedTimeBetweenStepsMicros = 0;

        uint16_t rampUpEncoderTicks = 0;
        uint16_t rampDownEncoderTicks = 0;
        
        uint16_t startEncoderPosition = 0;
        uint16_t destinationEncoderPosition = 0;
        uint8_t destinationEncoderPositionTolerance = 0;

        int8_t direction = 1;

        int8_t currentStep = 0;

        unsigned long lastStepMicros = 0;

        void performStep();
};

#endif