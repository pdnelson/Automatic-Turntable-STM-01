#include <SubCmdMoveNStepsV.h>
#include <Arduino.h>
#include <VerticalDirection.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <MovementAxis.h>
#include <SubCommandId.h>

SubCmdMoveNStepsV::SubCmdMoveNStepsV(TurntableState* state, int16_t steps, uint8_t speed, bool releaseCurrentAfterMovement) : BaseTurntableSubCommand(state) {
    this->state = state;
    this->speed = speed;
    this->steps = abs(steps);
    stepCount = 0;
    this->releaseCurrentAfterMovement = releaseCurrentAfterMovement;
    direction = this->steps == steps ? VerticalDirection::Up : VerticalDirection::Down;
}

void SubCmdMoveNStepsV::doInitialize() {
    digitalWrite(Pin::MovementSelect, MovementAxis::Vertical);
    state->movementStepper.setDirection(direction);
    state->movementStepper.calibrateDirection(VerticalDirection::Up, VerticalDirection::Down);
    state->movementStepper.setSpeed(this->speed);
}

CommandResult SubCmdMoveNStepsV::doExecute() {
    if(state->movementStepper.stepBlind(state->clockMicros)) {
        stepCount++;
    }

    if(stepCount == steps) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running; 
    }
}

void SubCmdMoveNStepsV::doUninitialize() {
    if(releaseCurrentAfterMovement) {
        state->movementStepper.releaseMotorCurrent();
    }
}

SubCommandId SubCmdMoveNStepsV::getSubCommandId() {
    return SubCommandId::SubMoveNStepsV;
}