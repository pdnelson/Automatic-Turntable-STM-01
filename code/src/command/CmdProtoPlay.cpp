#include <CmdProtoPlay.h>
#include <memory>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <BaseTurntableSubCommand.h>
#include <SubCmdGoToPositionV.h>
#include <SubCmdEngageAzClutch.h>
#include <SubCmdDisengageAzClutch.h>
#include <SubCmdMoveNSteps.h>
#include <TurntableState.h>
#include <Constants.h>

CmdProtoPlay::CmdProtoPlay(TurntableState* state, int16_t steps, uint8_t azimuthSpeed) : BaseTurntableCommand(state) {

    // Lift up
    subCommands = std::make_shared<SubCmdGoToPositionV>(state, state->calibration.verticalUpperLimit, LIFT_UP_SPEED)

        // Engage the clutch
        ->next(std::make_shared<SubCmdEngageAzClutch>(state))
        
        // Move CW or CCW
        ->next(std::make_shared<SubCmdMoveNSteps>(state, steps, azimuthSpeed, true))
        
        // Disengage the clutch
        ->next(std::make_shared<SubCmdDisengageAzClutch>(state))
        
        // Set down
        ->next(std::make_shared<SubCmdGoToPositionV>(state, state->calibration.verticalLowerLimit, SET_DOWN_SLOWLY));
}

CommandId CmdProtoPlay::getCommandId() {
    return CommandId::MoveNStepsH;
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