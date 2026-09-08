#include <CmdGoToPositionV.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdGoToPositionV.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>
#include <SubCmdDelay.h>
#include <SubCmdSetMovementVertical.h>

CmdGoToPositionV::CmdGoToPositionV(TurntableState* state, uint16_t position, uint8_t speed) : BaseTurntableCommand(state) {
    this->state = state;
    subCommands = std::make_shared<SubCmdGoToPositionV>(state, position, speed);
}

CommandId CmdGoToPositionV::getCommandId() {
    return CommandId::GoToPositionV;
}

void CmdGoToPositionV::doInitialize() {
    // do nothing
}

void CmdGoToPositionV::doUninitialize() {
    // do nothing
}