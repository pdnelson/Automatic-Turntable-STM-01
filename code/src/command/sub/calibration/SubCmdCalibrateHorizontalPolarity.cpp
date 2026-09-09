#include <SubCmdCalibrateHorizontalPolarity.h>
#include <TurntableState.h>
#include <CmdCalibration.h>
#include <Arduino.h>

SubCmdCalibrateHorizontalPolarity::SubCmdCalibrateHorizontalPolarity(TurntableState* state, uint16_t &destination) : BaseTurntableSubCommand(state), destination(destination) {
    
}

SubCommandId SubCmdCalibrateHorizontalPolarity::getSubCommandId() {
    return SubCommandId::CalibrateHorizontalPolarity;
}

void SubCmdCalibrateHorizontalPolarity::doInitialize() {
    // do nothing
}

CommandResult SubCmdCalibrateHorizontalPolarity::doExecute() {
    destination = state->azEncoder.getNormalizedPosition();
    return CommandResult::Success;
}

void SubCmdCalibrateHorizontalPolarity::doUninitialize() {
    // do nothing
}