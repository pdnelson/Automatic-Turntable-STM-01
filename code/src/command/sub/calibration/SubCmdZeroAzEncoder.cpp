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
    state->azEncoder.zeroOutEncoder(HOME_ZERO_OFFSET);
    return CommandResult::Success;
}

void SubCmdZeroAzEncoder::doUninitialize() {
    // do nothing
}