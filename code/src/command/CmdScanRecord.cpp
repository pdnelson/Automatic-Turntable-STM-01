#include <CmdScanRecord.h>
#include <StmShiftPin.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <TurntableState.h>
#include <SubCmdGoToPositionVAndSetClutch.h>
#include <SubCmdSetEncoderPollSpeed.h>
#include <SubCmdGoToPositionH.h>
#include <SubCmdScanRecord.h>
#include <SubCmdErrorCorrectHPosition.h>
#include <SubCmdDelay.h>
#include <Constants.h>

CmdScanRecord::CmdScanRecord(TurntableState* state) : BaseTurntableCommand(state) {

    // If it's homed, we can scan immediately
    if(state->getHomeStatus() == HomeStatus::Homed) {
        subCommands = std::make_shared<SubCmdGoToPositionVAndSetClutch>(state, state->calibration.verticalUpperLimit, LIFT_UP_SPEED, false, ClutchStatus::Engaged, true)
            ->next(std::make_shared<SubCmdSetEncoderPollSpeed>(state, EncoderPollSpeed::PollHErrorCorrect))
            ->next(std::make_shared<SubCmdScanRecord>(state, 1))
            ->next(std::make_shared<SubCmdSetEncoderPollSpeed>(state, EncoderPollSpeed::PollNormal));
    } 
    
    // Otherwise, we have to home the tonearm first
    else {
        // homing
        subCommands = std::make_shared<SubCmdGoToPositionVAndSetClutch>(state, state->calibration.verticalUpperLimit, LIFT_UP_SPEED, false, ClutchStatus::Engaged, true)
            ->next(std::make_shared<SubCmdSetEncoderPollSpeed>(state, EncoderPollSpeed::PollHMovementStandard))
            ->next(std::make_shared<SubCmdGoToPositionH>(state, state->calibration.home, 0, SPEED_AUTOMATIC))
            ->next(std::make_shared<SubCmdSetEncoderPollSpeed>(state, EncoderPollSpeed::PollHErrorCorrect))
            ->next(std::make_shared<SubCmdErrorCorrectHPosition>(state, state->calibration.home, 0, 2))

            // scanning
            ->next(std::make_shared<SubCmdScanRecord>(state, 1))
            ->next(std::make_shared<SubCmdSetEncoderPollSpeed>(state, EncoderPollSpeed::PollNormal));
    }

}

CommandId CmdScanRecord::getCommandId() {
    return CommandId::GoToPositionH;
}

void CmdScanRecord::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, true);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, true);
    state->recordProfile.clearProfile();
}

void CmdScanRecord::doUninitialize() {
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, false);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, false);
}