#include <SubCmdSetDownTonearm.h>
#include <Arduino.h>
#include <VerticalDirection.h>
#include <Pin.h>
#include <LiftStatus.h>
#include <CommandResult.h>
#include <BaseLiftSubCommand.h>
#include <Constants.h>
#include <TurntableState.h>

SubCmdSetDownTonearm::SubCmdSetDownTonearm(TurntableState* state, uint8_t speed) : BaseLiftSubCommand(state, speed) {
    // Do nothing.
}

void SubCmdSetDownTonearm::doInitialize() {
    baseInitialize();
}

CommandResult SubCmdSetDownTonearm::doExecute() {
    state->movementStepper.step(VerticalDirection::Down);
    int currentPosition = analogRead(Pin::VerticalPosition);

    bool stalled = checkVerticalStall(VerticalDirection::Down, currentPosition);

    CommandResult result = CommandResult::Running;

    if(stalled) {
        result = CommandResult::LiftStalledMovingUp;
    } else {
        if(currentPosition <= TEST_VERTICAL_LOWER_LIMIT || (isSetDown && setDownPosition - currentPosition >= TICKS_BELOW_RECORD)) {
            result = CommandResult::Success;
        } else if(!isSetDown && state->getLiftStatus() == LiftStatus::SetDown) {
            isSetDown = true;
            setDownPosition = currentPosition;
        }
    }

    return result;
}

void SubCmdSetDownTonearm::doUninitialize() {
    state->movementStepper.releaseMotorCurrent();
}