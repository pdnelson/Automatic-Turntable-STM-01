#include <StmHStepper.h>

StmHStepper::StmHStepper(StmEncoder &hEncoder, Pin enablePin, Pin stepPin, Pin directionPin) : hEncoder(hEncoder) {
    this->enablePin = enablePin;
    this->stepPin = stepPin;
    this->directionPin = directionPin;
    this->polarity = StmPolarity::Normal;
}

void StmHStepper::setPolarity(StmPolarity polarity) {
    this->polarity = polarity;
}