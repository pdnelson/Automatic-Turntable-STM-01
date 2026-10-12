#include <CmdCalibration.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>
#include <SubCmdCollectHorizontalPosition.h>
#include <RecordSize.h>
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
#include <SubCmdEngageAzClutch.h>
#include <SubCmdDisengageAzClutch.h>
#include <SubCmdZeroAzEncoder.h>
#include <SubCmdSetHMotorPolarity.h>

CmdCalibration::CmdCalibration(TurntableState* state) : BaseTurntableCommand(state) {

    // Basic controls:
    // "Play" advances to the next calibration step, saving the calibration value
    // "Pause" skips the calibration step, saving nothing

    // Dummy value used for when we don't want to capture a position for calibration, but we want to utilize every other
    // feature of a subcommand.
    uint16_t dummyValue = 0;

    // Calibrate the vertical polarity by going down, logging a point, then going up, and logging another point.
    // If the second point is greater than the first, then polarity is correct. Otherwise, we need to reverse it.
    subCommands = std::make_shared<SubCmdToggleLight>(state, StmShiftPin::LedPlayStatus, true)
        ->next(std::make_shared<SubCmdZeroAzEncoder>(state)) // First zero out the az encoder so it doesn't cross over 0 when determining polarity
        ->next(std::make_shared<SubCmdDelay>(state, DELAY_BETWEEN_STEPS_MS))
        ->next(std::make_shared<SubCmdMoveNStepsV>(state, -200, 14, true))
        ->next(std::make_shared<SubCmdDelay>(state, DELAY_BETWEEN_STEPS_MS))
        ->next(std::make_shared<SubCmdCollectVerticalPosition>(state, referencePoint1))
        ->next(std::make_shared<SubCmdMoveNStepsV>(state, 200, 14, true))
        ->next(std::make_shared<SubCmdDelay>(state, DELAY_BETWEEN_STEPS_MS))
        ->next(std::make_shared<SubCmdCollectVerticalPosition>(state, referencePoint2))
        ->next(std::make_shared<SubCmdSetPolarity>(state, referencePoint1, referencePoint2, true, state->calibration.polarityV))

        // Exercise the full range of the vertical movement. This should NOT fail out at this point. Consider this a "test" of the polarity check above.
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, 14, false))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 1023, 14, false))

        // Calibrate horizontal motor polarity
        ->next(std::make_shared<SubCmdDisengageAzClutch>(state)) // First disengage the clutch (even if it's already disengaged) just in case it was in an unusual position before
        ->next(std::make_shared<SubCmdEngageAzClutch>(state))
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, false, SubCommandId::SetHMotorPolarity, StmShiftPin::Led33Rpm, referencePoint1))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, 1000, 30, true))
        ->next(std::make_shared<SubCmdDelay>(state, DELAY_BETWEEN_STEPS_MS))
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, false, SubCommandId::SetHMotorPolarity, StmShiftPin::Led33Rpm, referencePoint2))
        ->next(std::make_shared<SubCmdSetHMotorPolarity>(state, referencePoint1, referencePoint2, state->calibration.hStepperPolarity))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, -1050, 30, true))

        // Calibrate the horizontal polarity by going counterclockwise, logging a point, then clockwise, and logging another point.
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, -1000, 30, true))
        ->next(std::make_shared<SubCmdDelay>(state, DELAY_BETWEEN_STEPS_MS))
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, false, SubCommandId::SetPolarity, StmShiftPin::Led33Rpm, referencePoint1))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, 1000, 30, true))
        ->next(std::make_shared<SubCmdDelay>(state, DELAY_BETWEEN_STEPS_MS))
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, false, SubCommandId::SetPolarity, StmShiftPin::Led33Rpm, referencePoint2))
        ->next(std::make_shared<SubCmdSetPolarity>(state, referencePoint1, referencePoint2, false, state->calibration.polarityH))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, -1050, 14, true))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, 14, false))

        // Calibrate the home mount position, then height
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, false, SubCommandId::CalibrateHome, StmShiftPin::Led33Rpm, state->calibration.home))
        ->next(std::make_shared<SubCmdMoveUpUntilLifted>(state, SubCommandId::CalibrateHomeHeight, 3))
        ->next(std::make_shared<SubCmdDelay>(state, DELAY_BETWEEN_STEPS_MS))
        ->next(std::make_shared<SubCmdCollectVerticalPosition>(state, referencePoint1))

        // Go to the middle of the platter
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 1023, 14, false))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, 2500, 14, true))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, SET_DOWN_SLOWLY, false))

        // Calibrate the platter height
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, false, SubCommandId::CalibratePlatterHeight, StmShiftPin::Led45Rpm, dummyValue))
        ->next(std::make_shared<SubCmdMoveUpUntilLifted>(state, SubCommandId::CalibratePlatterHeight, 3))
        ->next(std::make_shared<SubCmdDelay>(state, DELAY_BETWEEN_STEPS_MS))
        ->next(std::make_shared<SubCmdCollectVerticalPosition>(state, referencePoint2))
        ->next(std::make_shared<SubCmdCalibrateVerticalBounds>(state, referencePoint1, referencePoint2))

        // Go back to the home position
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 1023, 14, false))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, -2550, 14, true))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, SET_DOWN_QUICKLY, false))
        ->next(std::make_shared<SubCmdDisengageAzClutch>(state))
        ->next(std::make_shared<SubCmdToggleLight>(state, StmShiftPin::LedPlayStatus, false))

        // Now the user can calibrate the remaining record size positions
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, true, SubCommandId::Calibrate7In, StmShiftPin::Led7In, state->calibration.in7))
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, true, SubCommandId::Calibrate10In, StmShiftPin::Led10In, state->calibration.in10))
        ->next(std::make_shared<SubCmdCollectHorizontalPosition>(state, true, SubCommandId::Calibrate12In, StmShiftPin::Led12In, state->calibration.in12));
}

CommandId CmdCalibration::getCommandId() {
    return CommandId::Calibration;
}

void CmdCalibration::doInitialize() {
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

void CmdCalibration::doUninitialize() {
    state->outputShift.setValues(outputShiftValues);
    state->azEncoder.setPolarity(state->calibration.polarityH);
    state->horizontalStepper.setPolarity(state->calibration.hStepperPolarity);
    state->calibration.persist();
}