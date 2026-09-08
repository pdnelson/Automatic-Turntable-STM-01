#include <CmdGoToPositionH.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdGoToPositionH.h>
#include <SubCmdGoToPositionV.h>
#include <SubCmdEngageAzClutch.h>
#include <SubCmdDisengageAzClutch.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>
#include <SubCmdDelay.h>
#include <SubCmdSetMovementVertical.h>

CmdGoToPositionH::CmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t tolerance, uint8_t speed) : BaseTurntableCommand(state) {
    this->state = state;

    uint16_t setDownSpeed = SET_DOWN_SLOWLY;
    uint16_t difference = (int16_t)position - (int16_t)state->calibration.home;
    
    // If we're near the home position, go down quickly.
    if(difference <= VERTICAL_HOME_THRESHOLD) {
        setDownSpeed = SET_DOWN_QUICKLY;
    }

    // Lift up
    subCommands = std::make_shared<SubCmdGoToPositionV>(state, state->calibration.verticalUpperLimit, LIFT_UP_SPEED)

        // Engage the clutch
        ->next(std::make_shared<SubCmdEngageAzClutch>(state))
        
        // Move to specific position
        ->next(std::make_shared<SubCmdGoToPositionH>(state, position, tolerance, speed, 800))

        // Error correction
        ->next(std::make_shared<SubCmdDelay>(state, 100))
        ->next(std::make_shared<SubCmdGoToPositionH>(state, position, tolerance, 0.5, 0))
        ->next(std::make_shared<SubCmdDelay>(state, 100))
        ->next(std::make_shared<SubCmdGoToPositionH>(state, position, tolerance, 0.5, 0))
        ->next(std::make_shared<SubCmdDelay>(state, 100))
        ->next(std::make_shared<SubCmdGoToPositionH>(state, position, tolerance, 0.5, 0))
        ->next(std::make_shared<SubCmdDelay>(state, 100))
        ->next(std::make_shared<SubCmdGoToPositionH>(state, position, tolerance, 0.5, 0))
        ->next(std::make_shared<SubCmdDelay>(state, 100))
        ->next(std::make_shared<SubCmdGoToPositionH>(state, position, tolerance, 0.5, 0))
        
        // Set to vertical, with a delay, so we don't accidentally send any steps to the H stepper
        ->next(std::make_shared<SubCmdDelay>(state, 25))
        ->next(std::make_shared<SubCmdSetMovementVertical>(state))
        ->next(std::make_shared<SubCmdDelay>(state, 1000))

        // Set down
        ->next(std::make_shared<SubCmdGoToPositionV>(state, state->calibration.verticalLowerLimit, setDownSpeed))

        // Disengage the clutch
        ->next(std::make_shared<SubCmdDisengageAzClutch>(state));
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