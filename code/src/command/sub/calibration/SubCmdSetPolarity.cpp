#include <SubCmdSetPolarity.h>
#include <SubCommandId.h>
#include <CommandResult.h>
#include <CmdCalibration.h>
#include <StmEncoderPolarity.h>
#include <MovementAxis.h>
#include <TurntableState.h>

SubCmdSetPolarity::SubCmdSetPolarity(TurntableState* state, MovementAxis axis, uint16_t &lowerReferencePoint, uint16_t &upperReferencePoint, StmEncoderPolarity &destination) : BaseTurntableSubCommand(state), lowerReferencePoint(lowerReferencePoint), upperReferencePoint(upperReferencePoint), destination(destination) {
    this->axis = axis;
}

SubCommandId SubCmdSetPolarity::getSubCommandId() {
    return SubCommandId::SetPolarity;
}


void SubCmdSetPolarity::doInitialize() {
    // Do nothing
}

CommandResult SubCmdSetPolarity::doExecute() {
    if(lowerReferencePoint < upperReferencePoint) {
        destination = StmEncoderPolarity::NORMAL;
    } else if(lowerReferencePoint > upperReferencePoint) {
        destination = StmEncoderPolarity::REVERSED;
        if(axis == MovementAxis::Horizontal) {
            state->azEncoder.setPolarity(destination);
        }
    } else if(axis == MovementAxis::Vertical) {
        return CommandResult::FailedToSetVerticalPolarity;
    } else {
        return CommandResult::FailedToSetHorizontalPolarity;
    }
    
    return CommandResult::Success;
}

void SubCmdSetPolarity::doUninitialize() {
    // Do nothing
}