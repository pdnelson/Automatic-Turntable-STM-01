#include <SubCmdErrorCorrectHPosition.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <CommandResult.h>
#include <EncoderPollSpeed.h>

SubCmdErrorCorrectHPosition::SubCmdErrorCorrectHPosition(TurntableState* state, uint16_t position, uint8_t tolerance, float speed) : BaseTurntableSubCommand(state) {
    this->state = state;
    this->speed = speed;
    destinationEncoderPosition = position;
    encoderTolerance = tolerance;
    direction = AzimuthDirection::Clockwise; // This might get overwritten later depending on actual direction
}

void SubCmdErrorCorrectHPosition::doInitialize() {
    uint16_t distanceFromDestination = abs((int32_t)destinationEncoderPosition - (int32_t)state->azEncoder.getNormalizedPosition());

    // If we are already within tolerance, ignore this command entirely
    if(distanceFromDestination <= encoderTolerance) {
        result = CommandResult::Success;
    }

    // Otherwise set up the horizontal stepper's profile to do some non-profiled stepping
    if(destinationEncoderPosition < state->azEncoder.getNormalizedPosition()) {
        direction = AzimuthDirection::CounterClockwise;
    }

    state->horizontalStepper.setDirection(direction);
    state->horizontalStepper.setSpeed(speed);
}

CommandResult SubCmdErrorCorrectHPosition::doExecute() {
    state->horizontalStepper.stepBlind(state->clockMicros);

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
    state->verticalStepper.releaseMotorCurrent();
}

SubCommandId SubCmdErrorCorrectHPosition::getSubCommandId() {
    return SubCommandId::SubGoToPosition;
}