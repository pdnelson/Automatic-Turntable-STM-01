#include <SubCmdSetHMotorPolarity.h>
#include <SubCommandId.h>
#include <CommandResult.h>
#include <CmdCalibration.h>
#include <StmPolarity.h>
#include <TurntableState.h>

SubCmdSetHMotorPolarity::SubCmdSetHMotorPolarity(TurntableState* state, uint16_t &lowerReferencePoint, uint16_t &upperReferencePoint, StmPolarity &destination) : BaseTurntableSubCommand(state), lowerReferencePoint(lowerReferencePoint), upperReferencePoint(upperReferencePoint), destination(destination) {

}

SubCommandId SubCmdSetHMotorPolarity::getSubCommandId() {
    return SubCommandId::SetHMotorPolarity;
}


void SubCmdSetHMotorPolarity::doInitialize() {
    // Do nothing
}

CommandResult SubCmdSetHMotorPolarity::doExecute() {
    int32_t difference = abs((int32_t)lowerReferencePoint - (int32_t)upperReferencePoint);

    // If the tonearm moved at all, then the polarity is normal. Otherwise, it is reversed.
    if(difference < 100) {
        state->calibration.hStepperPolarity = StmPolarity::Reversed;
        state->horizontalStepper.setPolarity(state->calibration.hStepperPolarity);
    }

    return CommandResult::Success;
}

void SubCmdSetHMotorPolarity::doUninitialize() {
    // Do nothing
}