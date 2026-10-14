#include <SubCmdMoveClutchToHome.h>
#include <Arduino.h>
#include <ClutchDirection.h>
#include <ClutchStatus.h>
#include <Pin.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <Constants.h>
#include <SubCommandId.h>

SubCmdMoveClutchToHome::SubCmdMoveClutchToHome(TurntableState* state) : BaseTurntableSubCommand(state) {
    this->state = state;
    stepsBeforeStatusChange = 0;
    stepsSinceStatusChange = 0;
}

void SubCmdMoveClutchToHome::doInitialize() {
    state->clutchStepper.setSpeed(CLUTCH_SPEED);
    state->clutchStepper.setDirection(ClutchDirection::Engage);
}

CommandResult SubCmdMoveClutchToHome::doExecute() {
    if(state->clutchStepper.step(state->clockMicros)) {
        if(digitalRead(Pin::HorizontalClutchSwitch) == ClutchStatus::Engaged) {
            stepsSinceStatusChange++;
        } else {
            stepsBeforeStatusChange++;
        }

        if(stepsSinceStatusChange >= CLUTCH_HOME_STEPS) {
            return CommandResult::Success;
        } else if(stepsBeforeStatusChange > CLUTCH_TIMEOUT_STEPS) {
            return CommandResult::ClutchFailedToHome;
        } else {
            return CommandResult::Running;
        }
    } else {
        return CommandResult::Running;
    }
}

void SubCmdMoveClutchToHome::doUninitialize() {
    state->clutchStepper.releaseMotorCurrent();
}

SubCommandId SubCmdMoveClutchToHome::getSubCommandId() {
    return SubCommandId::MoveClutchToHome;
}