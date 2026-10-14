#include <CmdSetAzClutch.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdSetAzClutch.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>

CmdSetAzClutch::CmdSetAzClutch(TurntableState* state, ClutchStatus clutchStatus) : BaseTurntableCommand(state) {
    subCommands = std::make_shared<SubCmdSetAzClutch>(state, clutchStatus);
}

CommandId CmdSetAzClutch::getCommandId() {
    return CommandId::SetClutch;
}

void CmdSetAzClutch::doInitialize() {
    // Do nothin'
}

void CmdSetAzClutch::doUninitialize() {
    // Do nothing.
}