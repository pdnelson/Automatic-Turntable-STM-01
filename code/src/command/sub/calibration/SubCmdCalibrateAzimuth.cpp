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
    BaseCalibrationSubCommand(state, calCommand),
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
    return baseExecute(pinToFlash, destination, state->azEncoder.getNormalizedPosition());
}

void SubCmdCalibrateAzimuth::doUninitialize() {
    // do nothing
}