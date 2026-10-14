#include <CmdHomeAzClutch.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdMoveClutchToLimit.h>
#include <SubCmdMoveClutchToHome.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>

CmdHomeAzClutch::CmdHomeAzClutch(TurntableState* state) : BaseTurntableCommand(state) {
    subCommands = std::make_shared<SubCmdMoveClutchToLimit>(state)
        ->next(std::make_shared<SubCmdMoveClutchToHome>(state));
}

CommandId CmdHomeAzClutch::getCommandId() {
    return CommandId::HomeClutch;
}

void CmdHomeAzClutch::doInitialize() {
    // DO NOT A THING!
}

void CmdHomeAzClutch::doUninitialize() {
    // Do nothing.
}