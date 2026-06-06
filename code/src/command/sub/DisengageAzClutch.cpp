#include <SubCmdDisengageAzClutch.h>
#include <Arduino.h>
#include <ClutchDirection.h>
#include <ClutchStatus.h>
#include <Pin.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <Constants.h>

SubCmdDisengageAzClutch::SubCmdDisengageAzClutch(TurntableState* state) : BaseTurntableSubCommand(state) {
    this->state = state;
    totalSteps = 0;
}

void SubCmdDisengageAzClutch::doInitialize() {
    state->clutchStepper.setSpeed(CLUTCH_SPEED);
}

CommandResult SubCmdDisengageAzClutch::doExecute() {
    state->clutchStepper.step(ClutchDirection::Disengage);
    totalSteps++;

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