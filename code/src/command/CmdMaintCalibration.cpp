#include <CmdMaintCalibration.h>
#include <StmShiftPin.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <TurntableState.h>
#include <Constants.h>
#include <SubCmdCollectHorizontalPosition.h>
#include <SubCmdDelay.h>
#include <SubCommandId.h>
#include <SubCmdCollectVerticalPosition.h>
#include <SubCmdMoveNStepsV.h>
#include <SubCmdMoveNStepsH.h>
#include <SubCmdToggleLight.h>
#include <SubCmdSetPolarity.h>
#include <SubCmdMoveUpUntilLifted.h>
#include <SubCmdCalibrateVerticalBounds.h>
#include <SubCmdGoToPositionV.h>
#include <StmPolarity.h>
#include <SubCmdSetAzClutch.h>
#include <ClutchStatus.h>
#include <SubCmdZeroAzEncoder.h>
#include <SubCmdSetHMotorPolarity.h>
#include <SubCmdSetEncoderPollSpeed.h>

CmdMaintCalibration::CmdMaintCalibration(TurntableState* state) : BaseTurntableCommand(state) {

    // Basic controls:
    // "Play" advances to the next calibration step, saving the calibration value
    // "Pause" skips the calibration step, saving nothing

    // Calibrate the vertical polarity by going down, logging a point, then going up, and logging another point.
    // If the second point is greater than the first, then polarity is correct. Otherwise, we need to reverse it.
    subCommands = std::make_shared<SubCmdToggleLight>(state, StmShiftPin::LedPlayStatus, true)
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, 14, false))

        // Calibrate the home mount height
        ->next(std::make_shared<SubCmdMoveUpUntilLifted>(state, SubCommandId::CalibrateHomeHeight, 3))
        ->next(std::make_shared<SubCmdDelay>(state, DELAY_BETWEEN_STEPS_MS))
        ->next(std::make_shared<SubCmdCollectVerticalPosition>(state, referencePoint1))

        // Go to the middle of the platter
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 1023, 14, false))
        ->next(std::make_shared<SubCmdSetAzClutch>(state, ClutchStatus::Engaged))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, 1500, 14, true))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, SET_DOWN_SLOWLY, false))

        // Calibrate the platter height
        ->next(std::make_shared<SubCmdMoveUpUntilLifted>(state, SubCommandId::CalibratePlatterHeight, 3))
        ->next(std::make_shared<SubCmdDelay>(state, DELAY_BETWEEN_STEPS_MS))
        ->next(std::make_shared<SubCmdCollectVerticalPosition>(state, referencePoint2))
        ->next(std::make_shared<SubCmdCalibrateVerticalBounds>(state, referencePoint1, referencePoint2))

        ->next(std::make_shared<SubCmdSetEncoderPollSpeed>(state, EncoderPollSpeed::PollHMovementStandard))

        // Go back to the home position
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 1023, 14, false))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, -1550, 14, true))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, SET_DOWN_QUICKLY, false))
        ->next(std::make_shared<SubCmdSetAzClutch>(state, ClutchStatus::Disengaged))
        ->next(std::make_shared<SubCmdToggleLight>(state, StmShiftPin::LedPlayStatus, false))
        ->next(std::make_shared<SubCmdSetEncoderPollSpeed>(state, EncoderPollSpeed::PollNormal))

        // Now the user can calibrate the remaining record size positions
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, true, SubCommandId::CalibrateRecordScanEndPosition, StmShiftPin::LedAutoSize, state->calibration.scanEndPosition))
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, true, SubCommandId::Calibrate7In, StmShiftPin::Led7In, state->calibration.in7))
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, true, SubCommandId::Calibrate10In, StmShiftPin::Led10In, state->calibration.in10))
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, true, SubCommandId::Calibrate12In, StmShiftPin::Led12In, state->calibration.in12));
}

CommandId CmdMaintCalibration::getCommandId() {
    return CommandId::CalibrationMaint;
}

void CmdMaintCalibration::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, false);
    outputShiftValues = state->outputShift.getValues();
    state->calibration.hStepperPolarity = StmPolarity::Normal;
    state->calibration.polarityH = StmPolarity::Normal;
    state->calibration.polarityV = StmPolarity::Normal;
    state->horizontalStepper.setPolarity(state->calibration.hStepperPolarity);
    state->azEncoder.setPolarity(state->calibration.polarityH);

    state->outputShift.setValues(0);
}

void CmdMaintCalibration::doUninitialize() {
    state->outputShift.setValues(outputShiftValues);
    state->azEncoder.setPolarity(state->calibration.polarityH);
    state->horizontalStepper.setPolarity(state->calibration.hStepperPolarity);
    state->calibration.persist();
}