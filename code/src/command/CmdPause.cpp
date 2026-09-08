#include <CmdPause.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdGoToPositionV.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>

CmdPause::CmdPause(TurntableState* state) : BaseTurntableCommand(state) {
    subCommands = std::make_unique<SubCmdGoToPositionV>(state, state->calibration.verticalUpperLimit, LIFT_UP_SPEED);
}

CommandId CmdPause::getCommandId() {
    return CommandId::Pause;
}

void CmdPause::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, true);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, true);
}

void CmdPause::doUninitialize() {
    // Do nothing.
}