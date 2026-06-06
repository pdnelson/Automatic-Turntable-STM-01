#include <CmdError.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdError.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>

CmdError::CmdError(TurntableState* state, CommandResult error) : BaseTurntableCommand(state) {
    this->state = state;
    this->error = error;
    subCommands = std::make_unique<SubCmdError>(state);
}

CommandId CmdError::getCommandId() {
    return CommandId::Error;
}

void CmdError::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, false);
    outputShiftValues = state->outputShift.getValues();

    // Size and speed lights become a binary code, representing the current error
    state->outputShift.setValue(StmShiftPin::LedAutoSize,   error  & 1    );
    state->outputShift.setValue(StmShiftPin::Led78Rpm,      error >> 1 & 1);
    state->outputShift.setValue(StmShiftPin::Led45Rpm,      error >> 2 & 1);
    state->outputShift.setValue(StmShiftPin::Led33Rpm,      error >> 3 & 1);
    state->outputShift.setValue(StmShiftPin::LedAutoSpeed,  error >> 4 & 1);
    state->outputShift.setValue(StmShiftPin::Led12In,       error >> 5 & 1);
    state->outputShift.setValue(StmShiftPin::Led10In,       error >> 6 & 1);
    state->outputShift.setValue(StmShiftPin::Led7In,        error >> 7 & 1);
}

void CmdError::doUninitialize() {
    state->outputShift.setValues(outputShiftValues);
}