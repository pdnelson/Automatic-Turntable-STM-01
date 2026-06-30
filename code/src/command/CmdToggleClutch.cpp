#include <CmdToggleClutch.h>
#include <memory>
#include <CommandResult.h>
#include <CommandId.h>
#include <TurntableState.h>
#include <BaseTurntableCommand.h>
#include <ClutchStatus.h>
#include <SubCmdDisengageAzClutch.h>
#include <SubCmdEngageAzClutch.h>

CmdToggleClutch::CmdToggleClutch(TurntableState* state) : BaseTurntableCommand(state) {
    if(state->clutchEngaged() == ClutchStatus::Engaged) {
        subCommands = std::make_unique<SubCmdDisengageAzClutch>(state);
    } else {
        subCommands = std::make_unique<SubCmdEngageAzClutch>(state);
    }
}

CommandId CmdToggleClutch::getCommandId() {
    return CommandId::ToggleClutch;
}

void CmdToggleClutch::doInitialize() {
    // Do nothing.
}

void CmdToggleClutch::doUninitialize() {
    // Do nothing.
}