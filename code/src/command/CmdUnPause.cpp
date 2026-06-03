#include <CmdUnPause.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <ActionCommand.h>
#include <BaseTurntableCommand.h>
#include <SubCmdSetDownTonearm.h>
#include <TurntableState.h>
#include <Constants.h>

CmdUnPause::CmdUnPause(TurntableState* state) : BaseTurntableCommand(state) {
    subcommands = std::make_unique<SubCmdSetDownTonearm>(state, SET_DOWN_SPEED);
}

ActionCommand CmdUnPause::getCommandId() {
    return ActionCommand::UnPause;
}

void CmdUnPause::doInitialize() {
    // do nothing
}

void CmdUnPause::doUninitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, false);
}