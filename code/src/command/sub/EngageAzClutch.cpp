#include <SubCmdEngageAzClutch.h>
#include <Arduino.h>
#include <ClutchDirection.h>
#include <ClutchStatus.h>
#include <Pin.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <Constants.h>

SubCmdEngageAzClutch::SubCmdEngageAzClutch(TurntableState* state) : BaseTurntableSubCommand(state) {
    this->state = state;
    stepsBeforeStatusChange = 0;
    stepsSinceStatusChange = 0;
}

void SubCmdEngageAzClutch::doInitialize() {
    state->clutchStepper.setSpeed(CLUTCH_SPEED);
    Serial.println("made it here");
}

CommandResult SubCmdEngageAzClutch::doExecute() {
    state->clutchStepper.step(ClutchDirection::Engage);

    if(digitalRead(Pin::HorizontalClutchSwitch) == ClutchStatus::Engaged) {
        stepsSinceStatusChange++;
    } else {
        stepsBeforeStatusChange++;
    }

    if(stepsSinceStatusChange >= CLUTCH_ENGAGE_STEPS) {
        return CommandResult::Success;
    } else if(stepsBeforeStatusChange > CLUTCH_TIMEOUT_STEPS) {
        return CommandResult::ClutchFailedToEngage;
    } else {
        return CommandResult::Running;
    }
}

void SubCmdEngageAzClutch::doUninitialize() {
    state->clutchStepper.releaseMotorCurrent();
}