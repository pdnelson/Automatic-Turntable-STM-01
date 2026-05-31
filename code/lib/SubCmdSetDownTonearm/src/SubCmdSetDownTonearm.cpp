#include "SubCmdSetDownTonearm.h"
#include <Arduino.h>
#include <VerticalDirection.h>
#include <Pin.h>
#include <LiftStatus.h>
#include <CommandResult.h>
#include <CommandError.h>
#include <BaseLiftSubCommand.h>
#include <Constants.h>

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

    bool completed = false;
    CommandError error = CommandError::NoError;

    if(stalled) {
        error = CommandError::LiftStalledMovingUp;
    } else {
        if(currentPosition <= TEST_VERTICAL_LOWER_LIMIT) {
            completed = true;
        } else if(!this->isSetDown && this->state->getLiftStatus() == LiftStatus::SetDown) {
            this->isSetDown = true;
            this->setDownPosition = currentPosition;
        } else if(this->isSetDown && setDownPosition - currentPosition >= TICKS_BELOW_RECORD) {
            completed = true;
        }
    }

    return { completed, error };
}

void SubCmdSetDownTonearm::doUninitialize() {
    // Do nothing.
}