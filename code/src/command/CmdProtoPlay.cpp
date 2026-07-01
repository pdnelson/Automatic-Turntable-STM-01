#include <CmdProtoPlay.h>
#include <memory>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <BaseTurntableSubCommand.h>
#include <SubCmdLiftTonearm.h>
#include <SubCmdSetDownTonearm.h>
#include <SubCmdEngageAzClutch.h>
#include <SubCmdDisengageAzClutch.h>
#include <SubCmdMoveNSteps.h>
#include <TurntableState.h>
#include <Constants.h>

CmdProtoPlay::CmdProtoPlay(TurntableState* state, int16_t steps, uint8_t azimuthSpeed) : BaseTurntableCommand(state) {

    // Lift up
    subCommands = std::make_shared<SubCmdLiftTonearm>(state, LIFT_UP_SPEED)

        // Engage the clutch
        ->next(std::make_shared<SubCmdEngageAzClutch>(state))
        
        // Move CW or CCW
        ->next(std::make_unique<SubCmdMoveNSteps>(state, steps, azimuthSpeed))
        
        // Disengage the clutch
        ->next(std::make_unique<SubCmdDisengageAzClutch>(state))
        
        // Set down
        ->next(std::make_unique<SubCmdSetDownTonearm>(state, SET_DOWN_SPEED));
}

CommandId CmdProtoPlay::getCommandId() {
    return CommandId::Pause;
}

void CmdProtoPlay::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, true);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, true);
}

void CmdProtoPlay::doUninitialize() {
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, false);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, false);
}