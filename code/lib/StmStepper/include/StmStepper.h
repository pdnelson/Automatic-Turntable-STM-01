#include <Arduino.h>
#include <StmStepperResult.h>

#ifndef STMSTEPPER_H
#define STMSTEPPER_H

#define STEPPER_STEPS_PER_REVOLUTION 2048

/**
 * The slowest the tonearm can move; this is a little less than 1 RPM.
 */
#define STEPPER_MAX_DELAY_BETWEEN_STEPS 60000 // 0.5 RPM

class StmStepper {
    public:
        StmStepper(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);

        void setSpeed(float speedRpm);

        void setRampDownEncoderTicks(uint16_t rampDown);

        // This automatically sets the direction
        void setEncoderRange(uint16_t start, uint16_t end, uint8_t tolerance);

        void setDirection(int8_t direction);

        /**
         * Calibrates the direction so the `step` method can properly calculate which direction the the stepper needs to step
         * to elicit a positive increment from the encoder.
         */
        void calibrateDirection(int8_t positive, int8_t negative);

        /**
         * Step in accordance with the currentEncoderPosition.
         */
        StmStepperResult step(unsigned long clockMicros, uint16_t currentEncoderPosition);

        /**
         * Step without acknowledging any encoder position. This will ignore any ramp up/ramp down logic.
         */
        bool stepBlind(unsigned long clockMicros);

        void releaseMotorCurrent();

        // Methods we want to be able to unit test
        uint16_t rampDownSpeed(uint16_t currentEncoderPosition);
        uint16_t rampUpSpeed(uint16_t currentEncoderPosition);
        bool movementCompleted(uint16_t currentEncoderPosition);
        bool onBoundary(uint16_t currentEncoderPosition, uint16_t boundary, uint8_t tolerance);

        /**
         * How many ticks that have been made, so far, within the rampEncoderTicks-to-boundary range.
         */
        uint16_t ticksToBoundarySoFar(uint16_t currentEncoderPosition, uint16_t boundary, uint16_t rampEncoderTicks);
    
        unsigned long topSpeedTimeBetweenStepsMicros = 0;

    private:
        uint8_t pin1;
        uint8_t pin2;
        uint8_t pin3;
        uint8_t pin4;


        uint16_t rampDownEncoderTicks = 0;
        
        uint16_t startEncoderPosition = 0;
        uint16_t destinationEncoderPosition = 0;
        uint8_t destinationEncoderPositionTolerance = 0;

        int8_t direction = 1;

        int8_t positiveDirection = 1;
        int8_t negativeDirection = -1;

        int8_t currentStep = 0;

        unsigned long lastStepMicros = 0;

        void performStep();
};

#endif