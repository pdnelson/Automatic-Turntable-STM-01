#include "CmdUnPause.h"
#include <state/TurntableState.h>
#include <StmShiftPin.h>
#include <command/base/CommandResult.h>
#include <command/base/ActionCommand.h>
#include <command/base/BaseTurntableCommand.h>
#include <command/sub/lift/down/SubCmdSetDownTonearm.h>

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