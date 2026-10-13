#include <BaseHorizontalMovementSubCommand.h>
#include <Arduino.h>
#include <BaseTurntableSubCommand.h>
#include <Constants.h>
#include <Pin.h>
#include <TurntableState.h>
#include <AzimuthDirection.h>

BaseHorizontalMovementSubCommand::BaseHorizontalMovementSubCommand(TurntableState* state, uint8_t speed): BaseTurntableSubCommand(state) {
    this->speed = speed;

    stepsBetweenPolling = (STEPS_BETWEEN_POLLING_REF * speed) / SPEED_REF;
    ticksPerPoll = (TICKS_PER_POLL_REF * speed) / SPEED_REF;
}

bool BaseHorizontalMovementSubCommand::checkStall(AzimuthDirection direction) {
    stallCounter++;

    if(stallCounter > stepsBetweenPolling) {
        uint16_t stepsSinceLastPoll = calculateTravelDistance(lastPosition);

        if(stepsSinceLastPoll < ticksPerPoll) {
            return true;
        }

        stallCounter = 0;
        lastPosition = state->azEncoder.getNormalizedPosition();
    }

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

CommandResult BaseHorizontalMovementSubCommand::stallResult(AzimuthDirection direction) {
    if(direction == AzimuthDirection::Clockwise) {
        return CommandResult::StalledMovingClockwise;
    } else {
        return CommandResult::StalledMovingCounterClockwise;
    }
}

void BaseHorizontalMovementSubCommand::baseUninitialize() {
    state->verticalStepper.releaseMotorCurrent();
}