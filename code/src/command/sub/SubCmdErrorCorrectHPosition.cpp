#include <SubCmdErrorCorrectHPosition.h>
#include <BaseHorizontalMovementSubCommand.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <CommandResult.h>
#include <EncoderPollSpeed.h>

SubCmdErrorCorrectHPosition::SubCmdErrorCorrectHPosition(TurntableState* state, uint16_t position, uint8_t tolerance, float speed) : BaseHorizontalMovementSubCommand(state, speed) {
    destinationEncoderPosition = position;
    encoderTolerance = tolerance;
    direction = AzimuthDirection::Clockwise; // This might get overwritten later depending on actual direction
}

void SubCmdErrorCorrectHPosition::doInitialize() {
    uint16_t distanceFromDestination = calculateTravelDistance(destinationEncoderPosition);

    // If we are already within tolerance, ignore this command entirely
    if(distanceFromDestination <= encoderTolerance) {
        result = CommandResult::Success;
    }

    direction = calculateDirection(destinationEncoderPosition);

    state->horizontalStepper.setDirection(direction);
    state->horizontalStepper.setSpeed(speed);
}

CommandResult SubCmdErrorCorrectHPosition::doExecute() {
    if(state->horizontalStepper.stepBlind(state->clockMicros) && checkStall(direction)) {
        return stallResult(direction);
    }

    if(
        (direction == AzimuthDirection::Clockwise && state->azEncoder.getNormalizedPosition() >= destinationEncoderPosition) ||
        (direction == AzimuthDirection::CounterClockwise && state->azEncoder.getNormalizedPosition() <= destinationEncoderPosition)
    ) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running; 
    }
}

void SubCmdErrorCorrectHPosition::doUninitialize() {
    baseUninitialize();
}

SubCommandId SubCmdErrorCorrectHPosition::getSubCommandId() {
    return SubCommandId::SubGoToPosition;
}