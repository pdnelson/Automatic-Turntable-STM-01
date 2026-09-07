#include <BaseCalibrationSubCommand.h>
#include <Constants.h>
#include <TurntableState.h>
#include <MuxPin.h>

BaseCalibrationSubCommand::BaseCalibrationSubCommand(TurntableState* state, CmdCalibration* calCommand) : BaseTurntableSubCommand(state) {
    this->calCommand = calCommand;
}

CommandResult BaseCalibrationSubCommand::baseExecute(StmShiftPin pin, uint16_t &destination, uint16_t content) {
    if(state->clockMicros - lightBlinkIndicator > ONE_SECOND_MICROS) {
        lightBlinkIndicator = state->clockMicros;
        state->outputShift.setValue(pin, !state->outputShift.getValue(pin));
    }

    if(state->inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease) {
        return CommandResult::Success;
    } 
    else if(state->inputMux.getValue(MuxPin::BtnPlay) == ButtonResult::OnRelease) {
        destination = content;
        return CommandResult::Success;
    }
    else {
        return CommandResult::Running;
    }
}