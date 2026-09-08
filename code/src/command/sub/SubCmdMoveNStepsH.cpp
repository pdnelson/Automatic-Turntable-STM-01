#include <SubCmdMoveNStepsH.h>
#include <Arduino.h>
#include <AzimuthDirection.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <MovementAxis.h>
#include <SubCommandId.h>

SubCmdMoveNStepsH::SubCmdMoveNStepsH(TurntableState* state, int16_t steps, uint8_t speed, bool releaseCurrentAfterMovement) : BaseTurntableSubCommand(state) {
    this->state = state;
    this->speed = speed;
    this->steps = abs(steps);
    stepCount = 0;
    this->releaseCurrentAfterMovement = releaseCurrentAfterMovement;
    direction = this->steps == steps ? AzimuthDirection::Clockwise : AzimuthDirection::CounterClockwise;
}

void SubCmdMoveNStepsH::doInitialize() {
    digitalWrite(Pin::MovementSelect, MovementAxis::Horizontal);
    state->movementStepper.setDirection(direction);
    state->movementStepper.setSpeed(this->speed);
}

CommandResult SubCmdMoveNStepsH::doExecute() {
    if(state->movementStepper.stepBlind(state->clockMicros)) {
        stepCount++;
    }

    if(stepCount == steps) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running; 
    }
}

void SubCmdMoveNStepsH::doUninitialize() {
    if(releaseCurrentAfterMovement) {
        state->movementStepper.releaseMotorCurrent();
    }
}

SubCommandId SubCmdMoveNStepsH::getSubCommandId() {
    return SubCommandId::MoveNSteps;
}