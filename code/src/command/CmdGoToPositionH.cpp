#include <CmdGoToPositionH.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdGoToPositionH.h>
#include <SubCmdLiftTonearm.h>
#include <SubCmdSetDownTonearm.h>
#include <SubCmdEngageAzClutch.h>
#include <SubCmdDisengageAzClutch.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>
#include <SubCmdDelay.h>

CmdGoToPositionH::CmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t tolerance, uint8_t speed) : BaseTurntableCommand(state) {
    this->state = state;

    // Lift up
    subCommands = std::make_shared<SubCmdLiftTonearm>(state, LIFT_UP_SPEED)

        // Engage the clutch
        ->next(std::make_shared<SubCmdEngageAzClutch>(state))
        
        // Move to specific position
        ->next(std::make_unique<SubCmdGoToPositionH>(state, position, tolerance, speed, 500))

        // Delay 100 ms
        ->next(std::make_unique<SubCmdDelay>(state, 100))

        // Correct any overshoot
        ->next(std::make_unique<SubCmdGoToPositionH>(state, position, tolerance, 0.5, 0))

        // Delay 100 ms
        ->next(std::make_unique<SubCmdDelay>(state, 100))

        // Correct any overshoot
        ->next(std::make_unique<SubCmdGoToPositionH>(state, position, tolerance, 0.5, 0))

        // Delay 100 ms
        ->next(std::make_unique<SubCmdDelay>(state, 100))

        // Correct any overshoot
        ->next(std::make_unique<SubCmdGoToPositionH>(state, position, tolerance, 0.5, 0))

        // Delay 100 ms
        ->next(std::make_unique<SubCmdDelay>(state, 100))

        // Correct any overshoot
        ->next(std::make_unique<SubCmdGoToPositionH>(state, position, tolerance, 0.5, 0))
        
        // Disengage the clutch
        ->next(std::make_unique<SubCmdDisengageAzClutch>(state))
        
        // Set down
        ->next(std::make_unique<SubCmdSetDownTonearm>(state, SET_DOWN_SPEED));
}

CommandId CmdGoToPositionH::getCommandId() {
    return CommandId::GoToPosition;
}

void CmdGoToPositionH::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, true);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, true);
}

void CmdGoToPositionH::doUninitialize() {
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, false);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, false);
}