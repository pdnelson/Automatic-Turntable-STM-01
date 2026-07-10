#include <SubCmdMoveNSteps.h>
#include <Arduino.h>
#include <AzimuthDirection.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <MovementAxis.h>
#include <SubCommandId.h>

SubCmdMoveNSteps::SubCmdMoveNSteps(TurntableState* state, int16_t steps, uint8_t speed, bool releaseCurrentAfterMovement) : BaseTurntableSubCommand(state) {
    this->state = state;
    this->speed = speed;
    this->steps = abs(steps);
    stepCount = 0;
    this->releaseCurrentAfterMovement = releaseCurrentAfterMovement;
    direction = this->steps == steps ? AzimuthDirection::Clockwise : AzimuthDirection::CounterClockwise;
}

void SubCmdMoveNSteps::doInitialize() {
    digitalWrite(Pin::MovementSelect, MovementAxis::Horizontal);
    state->movementStepper.setDirection(direction);
    state->movementStepper.setSpeed(this->speed);
}

CommandResult SubCmdMoveNSteps::doExecute() {
    if(state->movementStepper.stepBlind(state->clockMicros)) {
        stepCount++;
    }

    if(stepCount == steps) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running; 
    }
}

void SubCmdMoveNSteps::doUninitialize() {
    if(releaseCurrentAfterMovement) {
        state->movementStepper.releaseMotorCurrent();
    }
}

SubCommandId SubCmdMoveNSteps::getSubCommandId() {
    return SubCommandId::MoveNSteps;
}