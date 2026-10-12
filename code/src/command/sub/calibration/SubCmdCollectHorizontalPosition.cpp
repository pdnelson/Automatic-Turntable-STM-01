#include <Arduino.h>
#include <SubCmdCollectHorizontalPosition.h>
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

SubCmdCollectHorizontalPosition::SubCmdCollectHorizontalPosition(TurntableState* state, bool waitForUserInput, SubCommandId subCommandId, StmShiftPin pinToFlash, uint16_t &destination) : 
    BaseTurntableSubCommand(state),
    destination(destination) {

    this->waitForUserInput = waitForUserInput;
    this->subCommandId = subCommandId;
    this->pinToFlash = pinToFlash;
}

SubCommandId SubCmdCollectHorizontalPosition::getSubCommandId() {
    return subCommandId;
}

void SubCmdCollectHorizontalPosition::doInitialize() {
    if(waitForUserInput) {
        state->outputShift.setValues(0);
    }
}

CommandResult SubCmdCollectHorizontalPosition::doExecute() {
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

void SubCmdCollectHorizontalPosition::doUninitialize() {
    // do nothing
}

void SubCmdCollectHorizontalPosition::updateDestination() {
    destination = state->azEncoder.getNormalizedPosition();
}