#include <Arduino.h>

#ifndef STMBASICSTEPPER_H
#define STMBASICSTEPPER_H

#define STEPPER_STEPS_PER_REVOLUTION 2048

/**
 * The slowest the tonearm can move; this is a little less than 1 RPM.
 */
#define STEPPER_MAX_DELAY_BETWEEN_STEPS 60000 // 0.5 RPM

class StmBasicStepper {
    public:
        StmBasicStepper(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);

        void setSpeed(float speedRpm);

        void setDirection(int8_t direction);

        bool step(unsigned long clockMicros);

        void releaseMotorCurrent();

    private:
        uint8_t pin1;
        uint8_t pin2;
        uint8_t pin3;
        uint8_t pin4;

        int8_t direction = 1;

        int8_t currentStep = 0;

        unsigned long lastStepMicros = 0;
        unsigned long topSpeedTimeBetweenStepsMicros = 0;

        void performStep();
};

#endif