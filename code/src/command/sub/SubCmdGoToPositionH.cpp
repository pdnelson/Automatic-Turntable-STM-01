#include <SubCmdGoToPositionH.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <MovementAxis.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <StmStepperResult.h>
#include <CommandResult.h>

SubCmdGoToPositionH::SubCmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t tolerance, uint8_t speed, uint16_t rampDownTicks) : BaseTurntableSubCommand(state) {
    this->state = state;
    this->speed = speed;
    destinationEncoderPosition = position;
    this->rampDownEncoderTicks = rampDownTicks;
    encoderTolerance = tolerance;
}

void SubCmdGoToPositionH::doInitialize() {
    uint16_t currentPosition = state->azEncoder.getNormalizedPosition();

    // Immediately succeed the command if we're on the boundary
    if(state->movementStepper.onBoundary(currentPosition, destinationEncoderPosition, encoderTolerance)) {
        setCommandResult(CommandResult::Success);
    } 
    
    // Otherwise, carry out the movement
    else {
        digitalWrite(Pin::MovementSelect, MovementAxis::Horizontal);
        state->movementStepper.setSpeed(speed);
        state->movementStepper.setEncoderRange(currentPosition, destinationEncoderPosition, encoderTolerance);
        state->movementStepper.setRampDownEncoderTicks(rampDownEncoderTicks);
        state->movementStepper.calibrateDirection(AzimuthDirection::Clockwise, AzimuthDirection::CounterClockwise);
    }
}

CommandResult SubCmdGoToPositionH::doExecute() {
    StmStepperResult result = state->movementStepper.step(state->clockMicros, state->azEncoder.getNormalizedPosition());

    if(result.movementCompleted) {
        return CommandResult::Success;
    } else {
        // TO DO: Stall check
        return CommandResult::Running;
    }
}

void SubCmdGoToPositionH::doUninitialize() {
    state->movementStepper.releaseMotorCurrent();
}

SubCommandId SubCmdGoToPositionH::getSubCommandId() {
    return SubCommandId::SubGoToPosition;
}