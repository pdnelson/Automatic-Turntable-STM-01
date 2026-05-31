#include <CmdPause.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <ActionCommand.h>
#include <BaseTurntableCommand.h>
#include <SubCmdLiftTonearm.h>
#include <TurntableState.h>

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