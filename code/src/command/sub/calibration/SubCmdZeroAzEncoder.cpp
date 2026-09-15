#include <SubCmdZeroAzEncoder.h>
#include <TurntableState.h>
#include <Constants.h>

SubCmdZeroAzEncoder::SubCmdZeroAzEncoder(TurntableState* state) : BaseTurntableSubCommand(state) {
    
}

SubCommandId SubCmdZeroAzEncoder::getSubCommandId() {
    return SubCommandId::CalibrateHorizontalPolarity;
}

void SubCmdZeroAzEncoder::doInitialize() {
    // do nothing
}

CommandResult SubCmdZeroAzEncoder::doExecute() {
    state->azEncoder.setZero(0);
    state->calibration.azEncoderOffset = state->azEncoder.getPosition() - HOME_ZERO_OFFSET;
    state->azEncoder.zeroOutEncoder(state->calibration.azEncoderOffset);
    return CommandResult::Success;
}

void SubCmdZeroAzEncoder::doUninitialize() {
    // do nothing
}