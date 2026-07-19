#include <SubCmdSetMovementVertical.h>
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

SubCmdSetMovementVertical::SubCmdSetMovementVertical(TurntableState* state) : BaseTurntableSubCommand(state) {
    this->state = state;
}

void SubCmdSetMovementVertical::doInitialize() {
    digitalWrite(Pin::MovementSelect, MovementAxis::Vertical);
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