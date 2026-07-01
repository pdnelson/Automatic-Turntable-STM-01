#include <SubCmdGoToPositionH.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <MovementAxis.h>
#include <SubCommandId.h>

SubCmdGoToPositionH::SubCmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t speed) : BaseTurntableSubCommand(state) {
    this->state = state;
}

void SubCmdGoToPositionH::doInitialize() {
    digitalWrite(Pin::MovementSelect, MovementAxis::Horizontal);
}

CommandResult SubCmdGoToPositionH::doExecute() {
    return CommandResult::Success;
}

void SubCmdGoToPositionH::doUninitialize() {
    // Do nothing.
}

SubCommandId SubCmdGoToPositionH::getSubCommandId() {
    return SubCommandId::SubGoToPosition;
}