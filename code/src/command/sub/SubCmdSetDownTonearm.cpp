#include <SubCmdSetDownTonearm.h>
#include <Arduino.h>
#include <VerticalDirection.h>
#include <Pin.h>
#include <LiftStatus.h>
#include <CommandResult.h>
#include <BaseLiftSubCommand.h>
#include <Constants.h>
#include <TurntableState.h>
#include <SubCommandId.h>

SubCmdSetDownTonearm::SubCmdSetDownTonearm(TurntableState* state, uint8_t speed) : BaseLiftSubCommand(state, speed) {
    // Do nothing.
}

void SubCmdSetDownTonearm::doInitialize() {
    baseInitialize();
    state->movementStepper.setDirection(VerticalDirection::Down);
    state->movementStepper.calibrateDirection(VerticalDirection::Up, VerticalDirection::Down);
}

CommandResult SubCmdSetDownTonearm::doExecute() {
    int currentPosition = analogRead(Pin::VerticalPosition);

    CommandResult result = CommandResult::Running;

    if(currentPosition <= TEST_VERTICAL_LOWER_LIMIT || (isSetDown && setDownPosition - currentPosition >= TICKS_BELOW_RECORD)) {
        result = CommandResult::Success;
    } else if(!isSetDown && state->getLiftStatus() == LiftStatus::SetDown) {
        isSetDown = true;
        setDownPosition = currentPosition;
    } else {
        if(state->movementStepper.stepBlind(state->clockMicros)) {
            bool stalled = checkVerticalStall(VerticalDirection::Down, currentPosition);
        
            if(stalled) {
                result = CommandResult::LiftStalledMovingDown;
            }
        }
    }

    return result;
}

void SubCmdSetDownTonearm::doUninitialize() {
    state->movementStepper.releaseMotorCurrent();
}

SubCommandId SubCmdSetDownTonearm::getSubCommandId() {
    return SubCommandId::SetDownTonearm;
}