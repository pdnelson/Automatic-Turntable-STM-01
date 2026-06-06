#include <SubCmdMoveNSteps.h>
#include <Arduino.h>
#include <AzimuthDirection.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <MovementAxis.h>

SubCmdMoveNSteps::SubCmdMoveNSteps(TurntableState* state, uint8_t speed, int16_t steps) : BaseTurntableSubCommand(state) {
    this->state = state;
    this->speed = speed;
    this->steps = abs(steps);
    this->direction = (this->steps == steps) ? AzimuthDirection::Clockwise : AzimuthDirection::CounterClockwise;
    stepCount = 0;
}

void SubCmdMoveNSteps::doInitialize() {
    digitalWrite(Pin::MovementSelect, MovementAxis::Horizontal);
    state->movementStepper.setSpeed(this->speed);
}

CommandResult SubCmdMoveNSteps::doExecute() {
    state->movementStepper.step(this->direction);
    stepCount++;

    if(stepCount == steps) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running; 
    }
}

void SubCmdMoveNSteps::doUninitialize() {
    state->movementStepper.releaseMotorCurrent();
}