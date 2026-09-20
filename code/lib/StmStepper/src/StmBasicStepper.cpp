#include <StmBasicStepper.h>
#include <Arduino.h>

StmBasicStepper::StmBasicStepper(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->pin3 = pin3;
    this->pin4 = pin4;

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
    pinMode(pin4, OUTPUT);
}

void StmBasicStepper::setSpeed(float speedRpm) {
    this->topSpeedTimeBetweenStepsMicros = 60000000L / STEPPER_STEPS_PER_REVOLUTION / speedRpm;
}

void StmBasicStepper::setDirection(int8_t direction) {
    this->direction = direction;
}

bool StmBasicStepper::step(unsigned long clockMicros) {
    if(clockMicros - lastStepMicros > topSpeedTimeBetweenStepsMicros) {
        performStep();
        lastStepMicros = clockMicros;
        return true;
    }

    return false;
}

void StmBasicStepper::releaseMotorCurrent() {
    lastStepMicros = 0;
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, LOW);
}

void StmBasicStepper::performStep() {
    currentStep += direction;

    if(currentStep == 4) {
        currentStep = 0;
    } else if(currentStep == -1) {
        currentStep = 3;
    }

    switch (currentStep) {
        case 0:
            digitalWrite(pin1, HIGH);
            digitalWrite(pin2, LOW);
            digitalWrite(pin3, HIGH);
            digitalWrite(pin4, LOW);
            break;
        case 1:
            digitalWrite(pin1, LOW);
            digitalWrite(pin2, HIGH);
            digitalWrite(pin3, HIGH);
            digitalWrite(pin4, LOW);
            break;
        case 2:
            digitalWrite(pin1, LOW);
            digitalWrite(pin2, HIGH);
            digitalWrite(pin3, LOW);
            digitalWrite(pin4, HIGH);
            break;
        case 3:
            digitalWrite(pin1, HIGH);
            digitalWrite(pin2, LOW);
            digitalWrite(pin3, LOW);
            digitalWrite(pin4, HIGH);
            break;
    }
}