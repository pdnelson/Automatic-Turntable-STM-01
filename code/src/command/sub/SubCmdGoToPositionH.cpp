#include <SubCmdGoToPositionH.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <MovementAxis.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>

SubCmdGoToPositionH::SubCmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t tolerance, uint8_t speed) : BaseTurntableSubCommand(state) {
    this->state = state;
    this->speed = speed;
    destinationEncoderPosition = position;
    encoderTolerance = tolerance;
}

void SubCmdGoToPositionH::doInitialize() {
    digitalWrite(Pin::MovementSelect, MovementAxis::Horizontal);
    state->movementStepper.setSpeed(speed);
    state->movementStepper.setEncoderRange(state->azEncoder.getNormalizedPosition(), destinationEncoderPosition, encoderTolerance);
    state->movementStepper.calibrateDirection(AzimuthDirection::Clockwise, AzimuthDirection::CounterClockwise);
}

CommandResult SubCmdGoToPositionH::doExecute() {
    return CommandResult::Success;
}

void SubCmdGoToPositionH::doUninitialize() {
    state->movementStepper.releaseMotorCurrent();
}

SubCommandId SubCmdGoToPositionH::getSubCommandId() {
    return SubCommandId::SubGoToPosition;
}