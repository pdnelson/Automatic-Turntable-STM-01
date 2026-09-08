#include <Arduino.h>
#include <SubCmdCalibrateAzimuth.h>
#include <CmdCalibration.h>
#include <SubCommandId.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <ButtonResult.h>
#include <MuxPin.h>
#include <Constants.h>
#include <RecordSize.h>
#include <StmShiftPin.h>
#include <SubCommandId.h>

SubCmdCalibrateAzimuth::SubCmdCalibrateAzimuth(TurntableState* state, CmdCalibration* calCommand, SubCommandId subCommandId, StmShiftPin pinToFlash, uint16_t &destination) : 
    BaseTurntableSubCommand(state),
    destination(destination) {

    this->subCommandId = subCommandId;
    this->pinToFlash = pinToFlash;
}

SubCommandId SubCmdCalibrateAzimuth::getSubCommandId() {
    return subCommandId;
}

void SubCmdCalibrateAzimuth::doInitialize() {
    state->outputShift.setValues(0);
}

CommandResult SubCmdCalibrateAzimuth::doExecute() {
    if(state->clockMicros - lightBlinkIndicator > ONE_SECOND_MICROS) {
        lightBlinkIndicator = state->clockMicros;
        state->outputShift.setValue(pinToFlash, !state->outputShift.getValue(pinToFlash));
    }

    if(state->inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease) {
        return CommandResult::Success;
    } 
    else if(state->inputMux.getValue(MuxPin::BtnPlay) == ButtonResult::OnRelease) {
        destination = state->azEncoder.getNormalizedPosition();
        return CommandResult::Success;
    }
    else {
        return CommandResult::Running;
    }
}

void SubCmdCalibrateAzimuth::doUninitialize() {
    // do nothing
}