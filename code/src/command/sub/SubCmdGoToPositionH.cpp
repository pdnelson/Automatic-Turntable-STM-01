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

    uint16_t steps = 2000 * travelDistance / 1508;

    state->horizontalStepper.setMovementProfile(direction, speed, steps);
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