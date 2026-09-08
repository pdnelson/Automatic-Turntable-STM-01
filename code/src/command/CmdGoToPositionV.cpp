#include <CmdGoToPositionV.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdGoToPositionV.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>
#include <SubCmdDelay.h>
#include <SubCmdSetMovementVertical.h>

CmdGoToPositionV::CmdGoToPositionV(TurntableState* state, uint16_t position, uint8_t speed) : BaseTurntableCommand(state) {
    this->state = state;
    subCommands = std::make_shared<SubCmdGoToPositionV>(state, position, speed);
}

CommandId CmdGoToPositionV::getCommandId() {
    return CommandId::GoToPosition;
}

void CmdGoToPositionV::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, true);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, true);
}

void CmdGoToPositionV::doUninitialize() {
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, false);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, false);
}