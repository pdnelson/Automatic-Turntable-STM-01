#include <BaseLiftSubCommand.h>
#include <Arduino.h>
#include <BaseTurntableSubCommand.h>
#include <Constants.h>
#include <Pin.h>
#include <MovementAxis.h>
#include <TurntableState.h>

BaseLiftSubCommand::BaseLiftSubCommand(TurntableState* state, uint8_t speed): BaseTurntableSubCommand(state) {
    this->speed = speed;
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
    digitalWrite(Pin::MovementSelect, MovementAxis::Vertical);
    verticalStallPosition = analogRead(Pin::VerticalPosition);
    state->movementStepper.setSpeed(speed);
}