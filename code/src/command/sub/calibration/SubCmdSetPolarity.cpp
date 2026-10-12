#include <SubCmdSetPolarity.h>
#include <SubCommandId.h>
#include <CommandResult.h>
#include <CmdCalibration.h>
#include <StmPolarity.h>
#include <TurntableState.h>

SubCmdSetPolarity::SubCmdSetPolarity(TurntableState* state, uint16_t &lowerReferencePoint, uint16_t &upperReferencePoint, bool isVertical, StmPolarity &destination) : BaseTurntableSubCommand(state), lowerReferencePoint(lowerReferencePoint), upperReferencePoint(upperReferencePoint), destination(destination) {
    this->isVertical = isVertical;
}

SubCommandId SubCmdSetPolarity::getSubCommandId() {
    return SubCommandId::SetPolarity;
}


void SubCmdSetPolarity::doInitialize() {
    // Do nothing
}

CommandResult SubCmdSetPolarity::doExecute() {
    if(lowerReferencePoint < upperReferencePoint) {
        destination = StmPolarity::Normal;
    } else if(lowerReferencePoint > upperReferencePoint) {
        destination = StmPolarity::Reversed;
    } else if(isVertical) {
        return CommandResult::FailedToSetVerticalPolarity;
    } else {
        return CommandResult::FailedToSetHorizontalPolarity;
    }

    // If horizontal, also set the encoder's polarity
    if(!isVertical) {
        state->azEncoder.setPolarity(destination);
    }
    
    return CommandResult::Success;
}

void SubCmdSetPolarity::doUninitialize() {
    // Do nothing
}