#include <SubCmdSetDownTonearm.h>
#include <Arduino.h>
#include <VerticalDirection.h>
#include <Pin.h>
#include <LiftStatus.h>
#include <CommandResult.h>
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

    CommandResult result = CommandResult::Running;

    if(stalled) {
        result = CommandResult::LiftStalledMovingUp;
    } else {
        if(currentPosition <= TEST_VERTICAL_LOWER_LIMIT || (this->isSetDown && setDownPosition - currentPosition >= TICKS_BELOW_RECORD)) {
            result = CommandResult::Success;
        } else if(!this->isSetDown && this->state->getLiftStatus() == LiftStatus::SetDown) {
            this->isSetDown = true;
            this->setDownPosition = currentPosition;
        }
    }

    return result;
}

void SubCmdSetDownTonearm::doUninitialize() {
    // Do nothing.
}