#include <SubCmdMoveNStepsH.h>
#include <Arduino.h>
#include <AzimuthDirection.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
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
    state->horizontalStepper.setMovementProfile(direction, speed, steps);
}

CommandResult SubCmdMoveNStepsH::doExecute() {
    if(state->horizontalStepper.stepTrapezoidally(state->clockMicros)) {
        stepCount++;
    }

    if(stepCount == steps) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running; 
    }
}

void SubCmdMoveNStepsH::doUninitialize() {
    // Do nothing
}

SubCommandId SubCmdMoveNStepsH::getSubCommandId() {
    return SubCommandId::SubMoveNStepsH;
}