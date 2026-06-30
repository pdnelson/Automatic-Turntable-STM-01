#include <CmdStepHorizontally.h>
#include <memory>
#include <CommandResult.h>
#include <CommandId.h>
#include <TurntableState.h>
#include <BaseTurntableCommand.h>
#include <ClutchStatus.h>
#include <SubCmdMoveNSteps.h>

CmdStepHorizontally::CmdStepHorizontally(TurntableState* state, int16_t steps, uint8_t azimuthSpeed) : BaseTurntableCommand(state) {
    subCommands = std::make_unique<SubCmdMoveNSteps>(state, azimuthSpeed, steps);
}

CommandId CmdStepHorizontally::getCommandId() {
    return CommandId::StepHMotor;
}

void CmdStepHorizontally::doInitialize() {
    // Do nothing.
}

void CmdStepHorizontally::doUninitialize() {
    // Do nothing.
}