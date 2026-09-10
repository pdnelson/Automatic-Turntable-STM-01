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

SubCmdCalibrateAzimuth::SubCmdCalibrateAzimuth(TurntableState* state, bool waitForUserInput, SubCommandId subCommandId, StmShiftPin pinToFlash, uint16_t &destination) : 
    BaseTurntableSubCommand(state),
    destination(destination) {

    this->waitForUserInput = waitForUserInput;
    this->subCommandId = subCommandId;
    this->pinToFlash = pinToFlash;
}

SubCommandId SubCmdCalibrateAzimuth::getSubCommandId() {
    return subCommandId;
}

void SubCmdCalibrateAzimuth::doInitialize() {
    if(waitForUserInput) {
        state->outputShift.setValues(0);
    }
}

CommandResult SubCmdCalibrateAzimuth::doExecute() {
    if(!waitForUserInput) {
        updateDestination();
        return CommandResult::Success;
    } else {
        if(state->clockMicros - lightBlinkIndicator > ONE_SECOND_MICROS) {
            lightBlinkIndicator = state->clockMicros;
            state->outputShift.setValue(pinToFlash, !state->outputShift.getValue(pinToFlash));
        }

        if(state->inputMux.getValue(MuxPin::BtnPause) == ButtonResult::OnRelease) {
            return CommandResult::Success;
        } 
        else if(state->inputMux.getValue(MuxPin::BtnPlay) == ButtonResult::OnRelease) {
            updateDestination();
            return CommandResult::Success;
        }
        else {
            return CommandResult::Running;
        }
    }
}

void SubCmdCalibrateAzimuth::doUninitialize() {
    // do nothing
}

void SubCmdCalibrateAzimuth::updateDestination() {
    destination = state->azEncoder.getNormalizedPosition();
}