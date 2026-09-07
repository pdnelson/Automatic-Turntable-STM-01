#include <SubCmdCalibrate7In.h>
#include <CmdCalibration.h>
#include <SubCommandId.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <ButtonResult.h>
#include <MuxPin.h>
#include <Constants.h>

SubCmdCalibrate7In::SubCmdCalibrate7In(TurntableState* state, CmdCalibration* calCommand) : BaseCalibrationSubCommand(state, calCommand) {
    // do nothing
}

SubCommandId SubCmdCalibrate7In::getSubCommandId() {
    return SubCommandId::Calibrate7In;
}

void SubCmdCalibrate7In::doInitialize() {
    state->outputShift.setValues(0);
}

CommandResult SubCmdCalibrate7In::doExecute() {

    // to do: move this to the base class
    if(state->clockMicros - lightBlinkIndicator > ONE_SECOND_MICROS) {
        lightBlinkIndicator = state->clockMicros;
        state->outputShift.setValue(StmShiftPin::Led7In, !state->outputShift.getValue(StmShiftPin::Led7In));
    }

    if(state->inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease) {
        return CommandResult::Success;
    } 
    else if(state->inputMux.getValue(MuxPin::BtnPlay) == ButtonResult::OnRelease) {
        state->calibration.in7 = state->azEncoder.getNormalizedPosition();
        return CommandResult::Success;
    }
    else {
        return CommandResult::Running;
    }
}

void SubCmdCalibrate7In::doUninitialize() {
    // do nothing
}