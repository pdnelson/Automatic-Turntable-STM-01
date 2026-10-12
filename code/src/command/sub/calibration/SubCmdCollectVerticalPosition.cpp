#include <SubCmdCollectVerticalPosition.h>
#include <TurntableState.h>
#include <CmdCalibration.h>
#include <Arduino.h>

SubCmdCollectVerticalPosition::SubCmdCollectVerticalPosition(TurntableState* state, uint16_t &destination) : BaseTurntableSubCommand(state), destination(destination) {
    
}

SubCommandId SubCmdCollectVerticalPosition::getSubCommandId() {
    return SubCommandId::CalibrateVerticalPoint;
}

void SubCmdCollectVerticalPosition::doInitialize() {
    // do nothing
}

CommandResult SubCmdCollectVerticalPosition::doExecute() {
    destination = state->getVerticalEncoderPos();
    return CommandResult::Success;
}

void SubCmdCollectVerticalPosition::doUninitialize() {
    // do nothing
}