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
    currSpeedBetweenStepsMicros = this->topSpeedTimeBetweenStepsMicros;
}

void StmHStepper::setMovementProfile(AzimuthDirection direction, float speed, uint16_t steps) {
    setSpeed(speed);
    setDirection(direction);
    destinationMovementSteps = steps;
    currentMovementSteps = 0;
    profileSteps = (PROFILE_STEPS_MAX * steps) / 1500;
    if(profileSteps > PROFILE_STEPS_MAX) {
        profileSteps = PROFILE_STEPS_MAX;
    }
    currSpeedBetweenStepsMicros = topSpeedTimeBetweenStepsMicros + profileSteps;
}

bool StmHStepper::stepTrapezoidally(unsigned long clockMicros) {
    bool didStep = stepBlind(clockMicros);

    if(didStep) {
        // Ramp up
        if(currentMovementSteps <= profileSteps && destinationMovementSteps > (profileSteps * 2)) {
            currSpeedBetweenStepsMicros -= 1;
        } 
        
        // Ramp down
        else if(currentMovementSteps + profileSteps > destinationMovementSteps) {
            currSpeedBetweenStepsMicros += 4;
        }

        currentMovementSteps++;
    }

    return didStep;
}

bool StmHStepper::stepBlind(unsigned long clockMicros) {
    if(!takingStep && clockMicros - lastStepMicros >= currSpeedBetweenStepsMicros) {
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