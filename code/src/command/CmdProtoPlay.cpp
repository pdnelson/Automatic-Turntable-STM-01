#include <CmdProtoPlay.h>
#include <memory>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <ActionCommand.h>
#include <BaseTurntableCommand.h>
#include <SubCmdLiftTonearm.h>
#include <SubCmdSetDownTonearm.h>
#include <TurntableState.h>
#include <Constants.h>

CmdProtoPlay::CmdProtoPlay(TurntableState* state) : BaseTurntableCommand(state) {
    subcommands = std::make_unique<SubCmdLiftTonearm>(state, LIFT_UP_SPEED);
    subcommands->nextSubCommand = std::make_unique<SubCmdSetDownTonearm>(state, SET_DOWN_SPEED);
}

ActionCommand CmdProtoPlay::getCommandId() {
    return ActionCommand::Pause;
}

void CmdProtoPlay::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, true);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, true);
}

void CmdProtoPlay::doUninitialize() {
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, false);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, false);
}