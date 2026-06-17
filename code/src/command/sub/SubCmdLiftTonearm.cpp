#include <SubCmdLiftTonearm.h>
#include <Arduino.h>
#include <VerticalDirection.h>
#include <Pin.h>
#include <LiftStatus.h>
#include <CommandResult.h>
#include <BaseLiftSubCommand.h>
#include <Constants.h>
#include <TurntableState.h>
#include <SubCommandId.h>

SubCmdLiftTonearm::SubCmdLiftTonearm(TurntableState* state, uint8_t speed) : BaseLiftSubCommand(state, speed) {
    // Do nothing.
}

void SubCmdLiftTonearm::doInitialize() {
    baseInitialize();
}

CommandResult SubCmdLiftTonearm::doExecute() {
    int currentPosition = analogRead(Pin::VerticalPosition);

    CommandResult result = CommandResult::Running;

    if(!reachedLimit && currentPosition >= TEST_VERTICAL_UPPER_LIMIT) {
        reachedLimit = true;
        timeLimitReached = state->clockMicros;
    } else if(reachedLimit) {
        if(state->getLiftStatus() == LiftStatus::Lifted) {
            result = CommandResult::Success;
        } else if(state->clockMicros - timeLimitReached > LIFT_BOUNCE_TIMEOUT_MICROS) {
            result = CommandResult::NotLifted;
        }
    } else {
        state->movementStepper.step(VerticalDirection::Up);
        bool stalled = checkVerticalStall(VerticalDirection::Up, currentPosition);

        if(stalled) {
            result = CommandResult::LiftStalledMovingUp;
        }
    }

    return result; 
}

void SubCmdLiftTonearm::doUninitialize() {
    state->movementStepper.releaseMotorCurrent();
}

SubCommandId SubCmdLiftTonearm::getSubCommandId() {
    return SubCommandId::LiftTonearm;
}