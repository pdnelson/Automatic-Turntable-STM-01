#include "CmdPause.h"
#include <state/TurntableState.h>
#include <StmShiftPin.h>
#include <command/base/CommandResult.h>
#include <command/base/ActionCommand.h>
#include <command/base/BaseTurntableCommand.h>
#include <command/sub/lift/up/SubCmdLiftTonearm.h>

CmdPause::CmdPause(TurntableState* state) : BaseTurntableCommand(state), liftTonearm(state, 10) {
    // Do nothing
}

ActionCommand CmdPause::getCommandId() {
    return ActionCommand::Pause;
}

void CmdPause::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, true);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, true);
}

CommandResult CmdPause::doExecute() {
    return liftTonearm.execute();
}

void CmdPause::doUninitialize() {
    // Do nothing.
}