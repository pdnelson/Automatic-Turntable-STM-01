#include <CmdPause.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdGoToPositionV.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>
#include <SubCmdSetAzClutch.h>

CmdPause::CmdPause(TurntableState* state) : BaseTurntableCommand(state) {
    // Engage and disengage the clutch around this routine because lifting might bounce the tonearm around a bit
    subCommands = std::make_shared<SubCmdSetAzClutch>(state, ClutchStatus::Engaged)
        ->next(std::make_shared<SubCmdGoToPositionV>(state, state->calibration.verticalUpperLimit, LIFT_UP_SPEED, false))
        ->next(std::make_shared<SubCmdSetAzClutch>(state, ClutchStatus::Disengaged));
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