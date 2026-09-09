#include <SubCmdCalibrateVerticalPolarity.h>
#include <TurntableState.h>
#include <CmdCalibration.h>
#include <Arduino.h>

SubCmdCalibrateVerticalPolarity::SubCmdCalibrateVerticalPolarity(TurntableState* state, uint16_t &destination) : BaseTurntableSubCommand(state), destination(destination) {
    
}

SubCommandId SubCmdCalibrateVerticalPolarity::getSubCommandId() {
    return SubCommandId::CalibrateVerticalPolarity;
}

void SubCmdCalibrateVerticalPolarity::doInitialize() {
    // do nothing
}

CommandResult SubCmdCalibrateVerticalPolarity::doExecute() {
    destination = state->getVerticalEncoderPos();
    return CommandResult::Success;
}

void SubCmdCalibrateVerticalPolarity::doUninitialize() {
    // do nothing
}