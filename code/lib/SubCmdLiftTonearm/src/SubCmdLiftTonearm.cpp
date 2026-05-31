#include "SubCmdLiftTonearm.h"
#include <Arduino.h>
#include <VerticalDirection.h>
#include <Pin.h>
#include <LiftStatus.h>
#include <CommandResult.h>
#include <CommandError.h>
#include <BaseLiftSubCommand.h>
#include <Constants.h>

SubCmdLiftTonearm::SubCmdLiftTonearm(TurntableState* state, uint8_t speed) : BaseLiftSubCommand(state, speed) {
    // Do nothing.
}

void SubCmdLiftTonearm::doInitialize() {
    baseInitialize();
}

CommandResult SubCmdLiftTonearm::doExecute() {
    state->movementStepper.step(VerticalDirection::Up);
    int currentPosition = analogRead(Pin::VerticalPosition);

    bool stalled = checkVerticalStall(VerticalDirection::Up, currentPosition);

    bool completed = false;
    CommandError error = CommandError::NoError;

    if(stalled) {
        error = CommandError::LiftStalledMovingUp;
    } else {
        if(!this->reachedLimit && currentPosition >= TEST_VERTICAL_UPPER_LIMIT) {
            this->reachedLimit = true;
            this->timeLimitReached = state->clockMicros;
        } else if(this->reachedLimit) {
            if(state->getLiftStatus() == LiftStatus::Lifted) {
                completed = true;
            } else if(state->clockMicros - this->timeLimitReached > LIFT_BOUNCE_TIMEOUT_MICROS) {
                error = CommandError::NotLifted;
            }
        }
    }

    return { completed, error }; 
}

void SubCmdLiftTonearm::doUninitialize() {
    // Do nothing.
}