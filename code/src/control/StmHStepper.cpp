#include <StmHStepper.h>
#include <HorizontalStepperEnableStatus.h>
#include <HorizontalStepperSleepStatus.h>

StmHStepper::StmHStepper(StmEncoder &hEncoder, Pin sleepPin, Pin enablePin, Pin stepPin, Pin directionPin) : hEncoder(hEncoder) {
    this->enablePin = enablePin;
    this->stepPin = stepPin;
    this->directionPin = directionPin;
    this->sleepPin = sleepPin;
    this->polarity = StmPolarity::Normal;

    pinMode(enablePin, OUTPUT);
    pinMode(stepPin, OUTPUT);
    pinMode(sleepPin, OUTPUT);
    pinMode(directionPin, OUTPUT);
}

void StmHStepper::setPolarity(StmPolarity polarity) {
    this->polarity = polarity;
}

void StmHStepper::setDirection(AzimuthDirection direction) {
    if(this->polarity == StmPolarity::Normal) {
        this->direction = direction;
    } else {
        this->direction = (AzimuthDirection)!direction;
    }

    digitalWrite(directionPin, this->direction);
}

void StmHStepper::setSpeed(float speed) {
    this->topSpeedTimeBetweenStepsMicros = (unsigned long)((float)60000000L / (float)STEPS_PER_REVOLUTION / speed);
}

bool StmHStepper::stepBlind(unsigned long clockMicros) {
    if(!takingStep && clockMicros - lastStepMicros >= topSpeedTimeBetweenStepsMicros) {
        performStep();
        lastStepMicros = clockMicros;
        return true;
    } else if(takingStep && clockMicros - lastStepMicros >= PULSE_WIDTH_MICROS) {
        takingStep = false;
        digitalWrite(stepPin, LOW);
    }

    return false;
}

void StmHStepper::wake() {
    digitalWrite(sleepPin, HorizontalStepperSleepStatus::Awake);
    digitalWrite(enablePin, HorizontalStepperEnableStatus::Enabled);
}

void StmHStepper::sleep() {
    digitalWrite(sleepPin, HorizontalStepperSleepStatus::Asleep);
    digitalWrite(enablePin, HorizontalStepperEnableStatus::Disabled);
}

void StmHStepper::performStep() {
    takingStep = true;
    digitalWrite(stepPin, HIGH);
}