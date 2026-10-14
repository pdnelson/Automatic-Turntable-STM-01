#include <SubCmdSetAzClutch.h>
#include <Arduino.h>
#include <ClutchDirection.h>
#include <ClutchStatus.h>
#include <Pin.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <Constants.h>
#include <SubCommandId.h>

SubCmdSetAzClutch::SubCmdSetAzClutch(TurntableState* state, ClutchStatus clutchStatus) : BaseTurntableSubCommand(state) {
    this->state = state;
    this->clutchStatus = clutchStatus;
}

void SubCmdSetAzClutch::doInitialize() {
    // If the clutch is already where we want it to be, do nothing.
    if(state->clutchStatus == clutchStatus) {
        result = CommandResult::Success;
    }

    state->clutchStepper.setSpeed(CLUTCH_SPEED);

    if(clutchStatus == ClutchStatus::Engaged) {
        state->clutchStepper.setDirection(ClutchDirection::Engage);
        state->horizontalStepper.wake();
    } else {
        state->clutchStepper.setDirection(ClutchDirection::Disengage);
    }
}

CommandResult SubCmdSetAzClutch::doExecute() {
    if(state->clutchStepper.step(state->clockMicros)) {
        stepCount++;

        if(stepCount >= CLUTCH_ENGAGE_DISENGAGE_STEPS) {
            return CommandResult::Success;
        }
    }
    
    return CommandResult::Running;
}

void SubCmdSetAzClutch::doUninitialize() {
    state->clutchStepper.releaseMotorCurrent();

    if(clutchStatus == ClutchStatus::Disengaged) {
        state->horizontalStepper.sleep();
    }

    state->clutchStatus = clutchStatus;
}

SubCommandId SubCmdSetAzClutch::getSubCommandId() {
    return SubCommandId::SetAzClutch;
}