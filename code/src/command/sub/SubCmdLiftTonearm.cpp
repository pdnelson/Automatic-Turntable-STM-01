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
    state->movementStepper.step(VerticalDirection::Up);
    int currentPosition = analogRead(Pin::VerticalPosition);

    bool stalled = checkVerticalStall(VerticalDirection::Up, currentPosition);

    CommandResult result = CommandResult::Running;

    if(stalled) {
        result = CommandResult::LiftStalledMovingUp;
    } else {
        if(!reachedLimit && currentPosition >= TEST_VERTICAL_UPPER_LIMIT) {
            reachedLimit = true;
            timeLimitReached = state->clockMicros;
        } else if(reachedLimit) {
            if(state->getLiftStatus() == LiftStatus::Lifted) {
                result = CommandResult::Success;
            } else if(state->clockMicros - timeLimitReached > LIFT_BOUNCE_TIMEOUT_MICROS) {
                result = CommandResult::NotLifted;
            }
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