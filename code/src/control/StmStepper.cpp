#include <StmStepper.h>
#include <Arduino.h>
#include <Pin.h>

StmStepper::StmStepper(Pin pin1, Pin pin2, Pin pin3, Pin pin4) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->pin3 = pin3;
    this->pin4 = pin4;

    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    pinMode(pin3, OUTPUT);
    pinMode(pin4, OUTPUT);
}

void StmStepper::setSpeed(uint8_t speedRpm) {
    this->topSpeedTimeBetweenStepsMicros = 60000000L / STEPPER_STEPS_PER_REVOLUTION / speedRpm;
}

void StmStepper::setDirection(int8_t direction) {
    this->direction = direction;
}

void StmStepper::setRampUpEncoderTicks(uint16_t rampUp) {
    rampUpEncoderTicks = rampUp;
}

void StmStepper::setRampDownEncoderTicks(uint16_t rampDown) {
    rampDownEncoderTicks = rampDown;
}

void StmStepper::setDestinationEncoderPosition(uint16_t destinationEncoderPosition) {
    this->destinationEncoderPosition = destinationEncoderPosition;
}

void StmStepper::setDestinationEncoderPositionDelta(uint16_t destinationEncoderPositionDelta) {
    this->destinationEncoderPositionDelta = destinationEncoderPositionDelta;
}

bool StmStepper::step(unsigned long clockMicros, uint16_t currentEncoderPosition) {
    if(clockMicros - lastStepMicros > currentTimeBetweenStepsMicros) {
        performStep();
        lastStepMicros = clockMicros;
        return true;
    }

    return false;
}

bool StmStepper::stepBlind(unsigned long clockMicros) {
    if(clockMicros - lastStepMicros > topSpeedTimeBetweenStepsMicros) {
        performStep();
        lastStepMicros = clockMicros;
        return true;
    }

    return false;
}

void StmStepper::releaseMotorCurrent() {
    rampingUp = true;
    lastStepMicros = 0;
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, LOW);
}

void StmStepper::performStep() {
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