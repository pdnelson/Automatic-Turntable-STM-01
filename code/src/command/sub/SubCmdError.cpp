#include <SubCmdError.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <MovementAxis.h>
#include <MuxPin.h>
#include <SubCommandId.h>

SubCmdError::SubCmdError(TurntableState* state) : BaseTurntableSubCommand(state) {
    this->state = state;
}

void SubCmdError::doInitialize() {
    timeInitialized = state->clockMicros;
}

CommandResult SubCmdError::doExecute() {
    if(state->clockMicros - powerBlinkCounter > ONE_SECOND_MICROS) {
        powerBlinkCounter = state->clockMicros;
        state->outputShift.setValue(StmShiftPin::LedPower, !state->outputShift.getValue(StmShiftPin::LedPower));
    }

    if(state->clockMicros - timeInitialized > ONE_SECOND_MICROS && (state->inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease || state->inputMux.getValue(MuxPin::BtnPlay) == ButtonResult::OnRelease)) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running;
    }
}

void SubCmdError::doUninitialize() {
    // Do nothing.
}

SubCommandId SubCmdError::getSubCommandId() {
    return SubCommandId::ErrorSubCommand;
}