#include <CmdUnPause.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <ActionCommand.h>
#include <BaseTurntableCommand.h>
#include <SubCmdSetDownTonearm.h>

CmdUnPause::CmdUnPause(TurntableState* state) : BaseTurntableCommand(state), setDownTonearm(state, 3) {
    // Do nothing.
}

ActionCommand CmdUnPause::getCommandId() {
    return ActionCommand::UnPause;
}

void CmdUnPause::doInitialize() {
    // do nothing
}

CommandResult CmdUnPause::doExecute() {
    return setDownTonearm.execute();
}

void CmdUnPause::doUninitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, false);
}