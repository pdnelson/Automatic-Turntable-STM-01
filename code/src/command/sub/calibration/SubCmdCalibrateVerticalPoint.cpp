#include <SubCmdCalibrateVerticalPoint.h>
#include <TurntableState.h>
#include <CmdCalibration.h>
#include <Arduino.h>

SubCmdCalibrateVerticalPoint::SubCmdCalibrateVerticalPoint(TurntableState* state, uint16_t &destination) : BaseTurntableSubCommand(state), destination(destination) {
    
}

SubCommandId SubCmdCalibrateVerticalPoint::getSubCommandId() {
    return SubCommandId::CalibrateVerticalPoint;
}

void SubCmdCalibrateVerticalPoint::doInitialize() {
    // do nothing
}

CommandResult SubCmdCalibrateVerticalPoint::doExecute() {
    destination = state->getVerticalEncoderPos();
    return CommandResult::Success;
}

void SubCmdCalibrateVerticalPoint::doUninitialize() {
    // do nothing
}