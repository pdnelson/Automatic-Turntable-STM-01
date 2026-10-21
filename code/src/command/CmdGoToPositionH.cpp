#include <CmdGoToPositionH.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdGoToPositionH.h>
#include <SubCmdGoToPositionV.h>
#include <SubCmdSetAzClutch.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>
#include <SubCmdDelay.h>
#include <SubCmdErrorCorrectHPosition.h>
#include <SubCmdSetEncoderPollSpeed.h>
#include <EncoderPollSpeed.h>
#include <ClutchStatus.h>
#include <SubCmdGoToPositionVAndSetClutch.h>

CmdGoToPositionH::CmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t tolerance, uint8_t speed) : BaseTurntableCommand(state) {
    this->state = state;

    uint16_t setDownSpeed = SET_DOWN_SLOWLY;
    uint16_t difference = abs((int16_t)position - (int16_t)state->calibration.home);
    
    // If we're near the home position, go down quickly.
    if(difference <= HOME_THRESHOLD) {
        setDownSpeed = SET_DOWN_QUICKLY;
    }

    // Engage the clutch and lift
    subCommands = std::make_shared<SubCmdGoToPositionVAndSetClutch>(state, state->calibration.verticalUpperLimit, LIFT_UP_SPEED, false, ClutchStatus::Engaged, true)
        
        // Delay, just in case the clutch moved the tonearm at all
        ->next(std::make_shared<SubCmdDelay>(state, 50))
        
        // Move to specific position
        ->next(std::make_shared<SubCmdSetEncoderPollSpeed>(state, EncoderPollSpeed::PollHMovementStandard))
        ->next(std::make_shared<SubCmdGoToPositionH>(state, position, tolerance, speed))

        // Error correction. During this step, we update the poll rate of the encoder to be insanely fast, so that
        // we can be sure it's giving us the most accurate, up-to-date information.
        ->next(std::make_shared<SubCmdSetEncoderPollSpeed>(state, EncoderPollSpeed::PollHErrorCorrect))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdErrorCorrectHPosition>(state, position, tolerance, 1))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdErrorCorrectHPosition>(state, position, tolerance, 1))
        ->next(std::make_shared<SubCmdSetEncoderPollSpeed>(state, EncoderPollSpeed::PollNormal))
        ->next(std::make_shared<SubCmdDelay>(state, 25))

        // Set down and disengage the clutch
        ->next(std::make_shared<SubCmdGoToPositionVAndSetClutch>(state, state->calibration.verticalLowerLimit, setDownSpeed, false, ClutchStatus::Disengaged, true));
}

CommandId CmdGoToPositionH::getCommandId() {
    return CommandId::GoToPositionH;
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