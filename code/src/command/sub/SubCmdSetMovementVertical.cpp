#include <SubCmdSetMovementVertical.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <CommandResult.h>

SubCmdSetMovementVertical::SubCmdSetMovementVertical(TurntableState* state) : BaseTurntableSubCommand(state) {
    this->state = state;
}

void SubCmdSetMovementVertical::doInitialize() {
}

CommandResult SubCmdSetMovementVertical::doExecute() {
    return CommandResult::Success;
}

void SubCmdSetMovementVertical::doUninitialize() {
    /* do nothing */
}

SubCommandId SubCmdSetMovementVertical::getSubCommandId() {
    return SubCommandId::SubSetMovementVertical;
}