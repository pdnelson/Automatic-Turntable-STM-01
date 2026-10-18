#include <SubCmdGoToPositionH.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <CommandResult.h>
#include <BaseHorizontalMovementSubCommand.h>

SubCmdGoToPositionH::SubCmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t tolerance, uint8_t speed) : BaseHorizontalMovementSubCommand(state, speed) {
    destinationEncoderPosition = position;
    encoderTolerance = tolerance;
}

void SubCmdGoToPositionH::doInitialize() {

    // If the destination position is less than current, we are going CCW. Otherwise, go CW.
    direction = calculateDirection(destinationEncoderPosition);

    // Calculate the steps to take relative to some values I determined through trial & error.
    // Approximately 1508 encoder ticks are traveled when 2000 steps are taken, which can be 
    // scaled to any encoder position range.
    uint16_t travelDistance = calculateTravelDistance(destinationEncoderPosition);

    if(travelDistance < MIN_TRAVEL_DISTANCE) {
        result = CommandResult::Success;
        return;
    }

    uint16_t steps = H_STEPS_REF * travelDistance / H_TICKS_REF;

    uint8_t normalizedSpeed = speed;

    // If the speed is the "automatic" speed, then determine the speed based on how 
    if(speed == SPEED_AUTOMATIC) {
        if(travelDistance > 1300) normalizedSpeed = H_MOVEMENT_FAST;
        else if(travelDistance > 700) normalizedSpeed = H_MOVEMENT_MEDIUM;
        else normalizedSpeed = H_MOVEMENT_SLOW;
    }

    state->horizontalStepper.setMovementProfile(direction, normalizedSpeed, steps);
    stepsToTake = steps;
}

CommandResult SubCmdGoToPositionH::doExecute() {
    if(state->horizontalStepper.stepTrapezoidally(state->clockMicros)) {
        stepCount++;

        if(checkStall(direction)) {
            return stallResult(direction);
        }
    }

    if(stepCount == stepsToTake) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running; 
    }
}

void SubCmdGoToPositionH::doUninitialize() {
    baseUninitialize();
}

SubCommandId SubCmdGoToPositionH::getSubCommandId() {
    return SubCommandId::SubGoToPosition;
}