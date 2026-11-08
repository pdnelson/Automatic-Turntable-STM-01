#include <SubCmdScanRecord.h>
#include <BaseHorizontalMovementSubCommand.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <CommandResult.h>

SubCmdScanRecord::SubCmdScanRecord(TurntableState* state, float speed) : BaseHorizontalMovementSubCommand(state, speed) {
}

void SubCmdScanRecord::doInitialize() {
    state->horizontalStepper.setDirection(AzimuthDirection::Clockwise);
    state->horizontalStepper.setSpeed(speed);
}

CommandResult SubCmdScanRecord::doExecute() {
    if(state->horizontalStepper.stepBlind(state->clockMicros)) {
        if(checkStall(AzimuthDirection::Clockwise)) {
            return stallResult(AzimuthDirection::Clockwise);
        } else {
            state->recordProfile.collectRawDataPoint();
        }
    }

    if(state->azEncoder.getNormalizedPosition() >= state->calibration.scanEndPosition) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running; 
    }
}

void SubCmdScanRecord::doUninitialize() {
    baseUninitialize();
    state->recordProfile.averageProfile();
}

SubCommandId SubCmdScanRecord::getSubCommandId() {
    return SubCommandId::HErrorCorrect;
}