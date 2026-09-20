#include <SubCmdSetVerticalPolarity.h>
#include <SubCommandId.h>
#include <CommandResult.h>
#include <CmdCalibration.h>
#include <StmPolarity.h>
#include <TurntableState.h>

SubCmdSetVerticalPolarity::SubCmdSetVerticalPolarity(TurntableState* state, uint16_t &lowerReferencePoint, uint16_t &upperReferencePoint, StmPolarity &destination) : BaseTurntableSubCommand(state), lowerReferencePoint(lowerReferencePoint), upperReferencePoint(upperReferencePoint), destination(destination) {

}

SubCommandId SubCmdSetVerticalPolarity::getSubCommandId() {
    return SubCommandId::SetPolarity;
}


void SubCmdSetVerticalPolarity::doInitialize() {
    // Do nothing
}

CommandResult SubCmdSetVerticalPolarity::doExecute() {
    if(lowerReferencePoint < upperReferencePoint) {
        destination = StmPolarity::Normal;
    } else if(lowerReferencePoint > upperReferencePoint) {
        destination = StmPolarity::Reversed;
    } else {
        return CommandResult::FailedToSetVerticalPolarity;
    }
    
    return CommandResult::Success;
}

void SubCmdSetVerticalPolarity::doUninitialize() {
    // Do nothing
}