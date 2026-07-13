#include <StmStepper.h>
#include <Arduino.h>
#include <StmStepperResult.h>

StmStepper::StmStepper(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) {
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

void StmStepper::calibrateDirection(uint8_t positive, uint8_t negative) {
    positiveDirection = positive;
    negativeDirection = negative;
}

void StmStepper::setRampUpEncoderTicks(uint16_t rampUp) {
    rampUpEncoderTicks = rampUp;
}

void StmStepper::setRampDownEncoderTicks(uint16_t rampDown) {
    rampDownEncoderTicks = rampDown;
}

void StmStepper::setEncoderRange(uint16_t start, uint16_t end, uint8_t tolerance) {
    startEncoderPosition = start;
    destinationEncoderPosition = end;
    destinationEncoderPositionTolerance = tolerance;
    direction = (start > end) ? positiveDirection : negativeDirection;
}

StmStepperResult StmStepper::step(unsigned long clockMicros, uint16_t currentEncoderPosition) {
    StmStepperResult result = StmStepperResult();

    // The movement has completed.
    if(onBoundary(currentEncoderPosition, destinationEncoderPositionTolerance)) {
        result.movementCompleted = true;
    } 
    
    // The movement has not completed, and we need to take a step.
    else {
        uint16_t speed = topSpeedTimeBetweenStepsMicros;

        // Ramp down calculation
        // prioritize ramp down since that's the functional one; ramp up just looks pretty
        if(onBoundary(currentEncoderPosition, rampDownEncoderTicks)) {
            speed = rampDownSpeed(currentEncoderPosition);
        }

        // Ramp up calculation
        else if(onBoundary(currentEncoderPosition, rampUpEncoderTicks)) {
            speed = rampUpSpeed(currentEncoderPosition);
        }

        if(clockMicros - lastStepMicros > speed) {
            performStep();
            lastStepMicros = clockMicros;
            result.stepTaken = true;
        }
    }


    return result;
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
    lastStepMicros = 0;
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, LOW);
}

uint16_t StmStepper::rampDownSpeed(uint16_t currentEncoderPosition) {
    uint16_t ticksSoFar = 0; // todo
    return ((STEPPER_MAX_DELAY_BETWEEN_STEPS - topSpeedTimeBetweenStepsMicros) / rampDownEncoderTicks) * ticksSoFar;
}

uint16_t StmStepper::rampUpSpeed(uint16_t currentEncoderPosition) {
    uint16_t ticksSoFar = 0; // todo
    return ((STEPPER_MAX_DELAY_BETWEEN_STEPS - topSpeedTimeBetweenStepsMicros) / rampDownEncoderTicks) * ticksSoFar;
}

bool StmStepper::onBoundary(uint16_t currentEncoderPosition, uint8_t tolerance) {
    uint16_t lowerToleranceBoundary = destinationEncoderPosition - tolerance;
    uint16_t upperToleranceBoundary = destinationEncoderPosition + tolerance;
    
    return currentEncoderPosition >= lowerToleranceBoundary && currentEncoderPosition <= upperToleranceBoundary;
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