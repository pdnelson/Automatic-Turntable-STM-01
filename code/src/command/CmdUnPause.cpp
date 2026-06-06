#include <CmdUnPause.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdSetDownTonearm.h>
#include <TurntableState.h>
#include <Constants.h>

CmdUnPause::CmdUnPause(TurntableState* state) : BaseTurntableCommand(state) {
    subCommands = std::make_unique<SubCmdSetDownTonearm>(state, SET_DOWN_SPEED);
}

CommandId CmdUnPause::getCommandId() {
    return CommandId::UnPause;
}

void CmdUnPause::doInitialize() {
    // do nothing
}

void CmdUnPause::doUninitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, false);
}