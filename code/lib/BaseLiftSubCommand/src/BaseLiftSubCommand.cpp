#include <BaseLiftSubCommand.h>
#include <Arduino.h>
#include <BaseTurntableSubCommand.h>
#include <Constants.h>
#include <Pin.h>
#include <MovementAxis.h>

BaseLiftSubCommand::BaseLiftSubCommand(TurntableState* state, uint8_t speed): BaseTurntableSubCommand(state) {
    this->speed = speed;
}

bool BaseLiftSubCommand::checkVerticalStall(VerticalDirection direction, int currentPosition) {
    this->verticalStallCounter++;

    // If going down, verticalStallPosition > currentPosition
    // If going up, currentPosition > verticalStallPosition
    int greaterThan = direction == VerticalDirection::Down ? this->verticalStallPosition : currentPosition;
    int lessThan = direction == VerticalDirection::Down ? currentPosition : this->verticalStallPosition;

    // Reset the stall counter when an encoder tick (in the proper direction) occurs
    if(greaterThan > lessThan) {
        this->verticalStallPosition = currentPosition;
        this->verticalStallCounter = 0;
    } 
    
    // We've stalled.
    else if(this->verticalStallCounter >= VERTICAL_STALL_STEPS) {
        return true;
    }

    return false;
}

void BaseLiftSubCommand::baseInitialize() {
    digitalWrite(Pin::MovementSelect, MovementAxis::Vertical);
    this->verticalStallPosition = analogRead(Pin::VerticalPosition);
    state->movementStepper.setSpeed(this->speed);
}