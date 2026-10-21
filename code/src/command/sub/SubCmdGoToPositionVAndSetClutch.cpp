#include <SubCmdGoToPositionVAndSetClutch.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <CommandResult.h>
#include <ClutchDirection.h>
#include <LiftStatus.h>
#include <Pin.h>

SubCmdGoToPositionVAndSetClutch::SubCmdGoToPositionVAndSetClutch(TurntableState* state, uint16_t position, uint8_t speed, bool skipLiftCheck, ClutchStatus clutchStatus, bool waitUntilLiftedBeforeSettingClutch) : BaseLiftSubCommand(state, position, speed, skipLiftCheck) {
    this->clutchStatus = clutchStatus;
    this->waitUntilLiftedBeforeSettingClutch = waitUntilLiftedBeforeSettingClutch;
}

void SubCmdGoToPositionVAndSetClutch::doInitialize() {
    baseInitialize();

    state->clutchStepper.setSpeed(CLUTCH_SPEED);

    if(clutchStatus == ClutchStatus::Engaged) {
        state->clutchStepper.setDirection(ClutchDirection::Engage);
        state->horizontalStepper.wake();
    } else {
        state->clutchStepper.setDirection(ClutchDirection::Disengage);
    }
}

CommandResult SubCmdGoToPositionVAndSetClutch::executeClutch() {
    if(state->clutchStatus == clutchStatus) {
        result = CommandResult::Success;
    }
    else if(state->clutchStepper.step(state->clockMicros)) {
        stepCount++;

        if(stepCount >= CLUTCH_ENGAGE_DISENGAGE_STEPS) {
            return CommandResult::Success;
        }
    }
    
    return CommandResult::Running;
}

CommandResult SubCmdGoToPositionVAndSetClutch::doExecute() {
    if(!liftHasChanged) {
        if(direction == VerticalDirection::Up) {
            liftHasChanged = digitalRead(Pin::Lift) == LiftStatus::Lifted;
        } else {
            liftHasChanged = state->getLiftStatus() == LiftStatus::SetDown;
        }
    }

    if(clutchResult == CommandResult::Running && (!waitUntilLiftedBeforeSettingClutch || liftHasChanged)) {
        clutchResult = executeClutch();
    }

    if(liftResult == CommandResult::Running) {
        liftResult = executeLift();
    }

    if(clutchResult > CommandResult::Success) {
        return clutchResult;
    }

    if(liftResult > CommandResult::Success) {
        return liftResult;
    }

    if(clutchResult == CommandResult::Success && liftResult == CommandResult::Success) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running;
    }
}

void SubCmdGoToPositionVAndSetClutch::doUninitialize() {
    state->verticalStepper.releaseMotorCurrent();
    state->clutchStepper.releaseMotorCurrent();

    if(clutchStatus == ClutchStatus::Disengaged) {
        state->horizontalStepper.sleep();
    }

    state->clutchStatus = clutchStatus;
}

SubCommandId SubCmdGoToPositionVAndSetClutch::getSubCommandId() {
    return SubCommandId::SubGoToPositionV;
}