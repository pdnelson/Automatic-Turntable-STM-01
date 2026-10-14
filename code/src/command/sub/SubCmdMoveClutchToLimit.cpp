#include <SubCmdMoveClutchToLimit.h>
#include <Arduino.h>
#include <ClutchDirection.h>
#include <ClutchHomeLimit.h>
#include <Pin.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <Constants.h>
#include <SubCommandId.h>

SubCmdMoveClutchToLimit::SubCmdMoveClutchToLimit(TurntableState* state) : BaseTurntableSubCommand(state) {
    this->state = state;
    totalSteps = 0;
}

void SubCmdMoveClutchToLimit::doInitialize() {
    state->clutchStepper.setSpeed(CLUTCH_SPEED);
    state->clutchStepper.setDirection(ClutchDirection::Disengage);
}

CommandResult SubCmdMoveClutchToLimit::doExecute() {
    if(digitalRead(Pin::HorizontalClutchSwitch) == ClutchHomeLimit::ClutchLimitClosed) {
        return CommandResult::Success;
    } else if(totalSteps > (CLUTCH_HOME_STEPS + CLUTCH_ENGAGE_DISENGAGE_STEPS + CLUTCH_TIMEOUT_STEPS)) {
        return CommandResult::ClutchFailedToHome;
    } else {
        if(state->clutchStepper.step(state->clockMicros)) {
            totalSteps++;
        }

        return CommandResult::Running;
    }
}

void SubCmdMoveClutchToLimit::doUninitialize() {
    state->clutchStepper.releaseMotorCurrent();
}

SubCommandId SubCmdMoveClutchToLimit::getSubCommandId() {
    return SubCommandId::MoveClutchToLimit;
}