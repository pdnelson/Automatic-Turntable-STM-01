#include <BaseHorizontalMovementSubCommand.h>
#include <Arduino.h>
#include <BaseTurntableSubCommand.h>
#include <Constants.h>
#include <Pin.h>
#include <TurntableState.h>
#include <AzimuthDirection.h>

BaseHorizontalMovementSubCommand::BaseHorizontalMovementSubCommand(TurntableState* state, uint8_t speed): BaseTurntableSubCommand(state) {
    this->speed = speed;
}

bool BaseHorizontalMovementSubCommand::checkStall(AzimuthDirection direction, int currentPosition) {

    return false;
}

AzimuthDirection BaseHorizontalMovementSubCommand::calculateDirection(uint16_t destination) {
    AzimuthDirection direction = AzimuthDirection::Clockwise;

    if(destination < state->azEncoder.getNormalizedPosition()) {
        direction = AzimuthDirection::CounterClockwise;
    }

    return direction;
}

uint16_t BaseHorizontalMovementSubCommand::calculateTravelDistance(uint16_t destination) {
    return abs((int32_t)destination - (int32_t)state->azEncoder.getNormalizedPosition());
}

void BaseHorizontalMovementSubCommand::baseUninitialize() {
    state->verticalStepper.releaseMotorCurrent();
}