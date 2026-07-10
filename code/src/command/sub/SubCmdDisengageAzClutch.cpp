#include <SubCmdDisengageAzClutch.h>
#include <Arduino.h>
#include <ClutchDirection.h>
#include <ClutchStatus.h>
#include <Pin.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <Constants.h>
#include <SubCommandId.h>

SubCmdDisengageAzClutch::SubCmdDisengageAzClutch(TurntableState* state) : BaseTurntableSubCommand(state) {
    this->state = state;
    totalSteps = 0;
}

void SubCmdDisengageAzClutch::doInitialize() {
    state->clutchStepper.setSpeed(CLUTCH_SPEED);
    state->clutchStepper.setDirection(ClutchDirection::Disengage);
}

CommandResult SubCmdDisengageAzClutch::doExecute() {
    if(state->clutchStepper.stepBlind(state->clockMicros)) {
        totalSteps++;
    }

    if(digitalRead(Pin::HorizontalClutchSwitch) == ClutchStatus::Disengaged) {
        return CommandResult::Success;
    } else if(totalSteps > (CLUTCH_ENGAGE_STEPS + CLUTCH_TIMEOUT_STEPS)) {
        return CommandResult::ClutchFailedToDisengage;
    } else {
        return CommandResult::Running;
    }
}

void SubCmdDisengageAzClutch::doUninitialize() {
    state->clutchStepper.releaseMotorCurrent();
}

SubCommandId SubCmdDisengageAzClutch::getSubCommandId() {
    return SubCommandId::DisengageAzClutch;
}