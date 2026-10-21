#include <BaseLiftSubCommand.h>
#include <Arduino.h>
#include <BaseTurntableSubCommand.h>
#include <Constants.h>
#include <Pin.h>
#include <TurntableState.h>

BaseLiftSubCommand::BaseLiftSubCommand(TurntableState* state, uint16_t position, uint8_t speed, bool skipLiftCheck): BaseTurntableSubCommand(state) {
    this->speed = speed;
    destinationEncoderPosition = position;
    this->skipLiftCheck = skipLiftCheck;
}

bool BaseLiftSubCommand::checkVerticalStall(VerticalDirection direction, int currentPosition) {
    verticalStallCounter++;

    // If going down, verticalStallPosition > currentPosition
    // If going up, currentPosition > verticalStallPosition
    int greaterThan = direction == VerticalDirection::Down ? verticalStallPosition : currentPosition;
    int lessThan = direction == VerticalDirection::Down ? currentPosition : verticalStallPosition;

    // Reset the stall counter when an encoder tick (in the proper direction) occurs
    if(greaterThan > lessThan) {
        verticalStallPosition = currentPosition;
        verticalStallCounter = 0;
    } 
    
    // We've stalled.
    else if(verticalStallCounter >= VERTICAL_STALL_STEPS) {
        return true;
    }

    return false;
}

void BaseLiftSubCommand::baseInitialize() {
    verticalStallPosition = state->getVerticalEncoderPos();
    state->verticalStepper.setSpeed(speed);

    int currentPosition = state->getVerticalEncoderPos();

    // If the destination is greater than the current position, move up. Otherwise, move down.
    if(destinationEncoderPosition > currentPosition) {
        direction = VerticalDirection::Up;
    } else {
        direction = VerticalDirection::Down;
    }

    state->verticalStepper.setDirection(direction);
}

CommandResult BaseLiftSubCommand::executeLift() {
    int currentPosition = state->getVerticalEncoderPos();

    CommandResult result = CommandResult::Running;

    // When we reach or overshoot the target position, log the time we did it, and set a status indicating that we reached it
    if(!reachedLimit && 
        ((direction == VerticalDirection::Up && currentPosition >= destinationEncoderPosition) ||
        (direction == VerticalDirection::Down && currentPosition <= destinationEncoderPosition))
    ) {
        reachedLimit = true;
        timeLimitReached = state->clockMicros;
    } else if(reachedLimit) {
        // Verify the tonearm is truly lifted once we've reached the limit. It can sometimes bounce a bit after lifting,
        // so allow some time for it to stop doing that.
        if(skipLiftCheck || direction == VerticalDirection::Down || state->getLiftStatus() == LiftStatus::Lifted) {
            result = CommandResult::Success;
        } else if(state->clockMicros - timeLimitReached > LIFT_BOUNCE_TIMEOUT_MICROS) {
            result = CommandResult::NotLifted;
        }
    } else {
        if(state->verticalStepper.step(state->clockMicros)) {
            bool stalled = checkVerticalStall(direction, currentPosition);

            if(stalled) {
                if(direction == VerticalDirection::Up) {
                    result = CommandResult::LiftStalledMovingUp;
                } else {
                    result = CommandResult::LiftStalledMovingDown;
                }
            }
        }
    }

    return result; 
}