#include <SubCmdGoToPositionV.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <MovementAxis.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <StmStepperResult.h>
#include <CommandResult.h>

SubCmdGoToPositionV::SubCmdGoToPositionV(TurntableState* state, uint16_t position, uint8_t speed) : BaseLiftSubCommand(state, speed) {
    this->state = state;
    this->speed = speed;
    destinationEncoderPosition = position;
}

void SubCmdGoToPositionV::doInitialize() {
    baseInitialize();

    int currentPosition = analogRead(Pin::VerticalPosition);

    // If the destination is greater than the current position, move up. Otherwise, move down.
    if(destinationEncoderPosition > currentPosition) {
        direction = VerticalDirection::Up;
    } else {
        direction = VerticalDirection::Down;
    }

    state->movementStepper.setDirection(direction);

    state->movementStepper.calibrateDirection(state->calibration.verticalUp, state->calibration.verticalDown);
}

CommandResult SubCmdGoToPositionV::doExecute() {
    int currentPosition = analogRead(Pin::VerticalPosition);

    CommandResult result = CommandResult::Running;

    // When we reach or overshoot the target position, log the time we did it, and set a status indicating that we reached it
    if(!reachedLimit && 
        (
            (direction == VerticalDirection::Up && currentPosition >= destinationEncoderPosition) ||
        (direction == VerticalDirection::Down && currentPosition <= destinationEncoderPosition))
    ) {
        reachedLimit = true;
        timeLimitReached = state->clockMicros;
    } else if(reachedLimit) {
        
        // Verify the tonearm is truly lifted once we've reached the limit. It can sometimes bounce a bit after lifting,
        // so allow some time for it to stop doing that.
        if(state->getLiftStatus() == LiftStatus::Lifted) {
            result = CommandResult::Success;
        } else if(state->clockMicros - timeLimitReached > LIFT_BOUNCE_TIMEOUT_MICROS) {
            result = CommandResult::NotLifted;
        }
    } else {
        if(state->movementStepper.stepBlind(state->clockMicros)) {
            bool stalled = checkVerticalStall(direction, currentPosition);

            if(stalled) {
                result = CommandResult::LiftStalledMovingUp;
            }
        }
    }

    return result; 
}

void SubCmdGoToPositionV::doUninitialize() {
    state->movementStepper.releaseMotorCurrent();
}

SubCommandId SubCmdGoToPositionV::getSubCommandId() {
    return SubCommandId::SubGoToPositionV;
}