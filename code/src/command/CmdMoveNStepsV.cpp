#include <CmdMoveNStepsV.h>
#include <SubCmdMoveNStepsV.h>
#include <memory>

CmdMoveNStepsV::CmdMoveNStepsV(TurntableState* state, int16_t steps, uint8_t speed, bool releaseCurrentAfterMovement) : BaseTurntableCommand(state) {
    subCommands = std::make_shared<SubCmdMoveNStepsV>(state, steps, speed, releaseCurrentAfterMovement);
}

CommandId CmdMoveNStepsV::getCommandId() {
    return CommandId::MoveNStepsV;
}

void CmdMoveNStepsV::doInitialize() {
    // do nothing
}

void CmdMoveNStepsV::doUninitialize() {
    // do nothing
}