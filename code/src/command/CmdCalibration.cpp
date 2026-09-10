#include <CmdCalibration.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <TurntableState.h>
#include <Constants.h>
#include <memory>
#include <SubCmdCalibrateAzimuth.h>
#include <RecordSize.h>
#include <SubCmdDelay.h>
#include <SubCommandId.h>
#include <SubCmdCalibrateVerticalPoint.h>
#include <SubCmdCalibrateHorizontalPolarity.h>
#include <SubCmdMoveNStepsV.h>
#include <SubCmdMoveNStepsH.h>
#include <SubCmdToggleLight.h>
#include <SubCmdSetPolarity.h>
#include <SubCmdMoveUpUntilLifted.h>
#include <SubCmdCalibrateVerticalBounds.h>
#include <MovementAxis.h>
#include <SubCmdGoToPositionV.h>
#include <StmEncoderPolarity.h>
#include <SubCmdEngageAzClutch.h>
#include <SubCmdDisengageAzClutch.h>

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
        ->next(std::make_shared<SubCmdMoveNStepsV>(state, -200, 14, true))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateVerticalPoint>(state, referencePoint1))
        ->next(std::make_shared<SubCmdMoveNStepsV>(state, 200, 14, true))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateVerticalPoint>(state, referencePoint2))
        ->next(std::make_shared<SubCmdSetPolarity>(state, MovementAxis::Vertical, referencePoint1, referencePoint2, state->calibration.polarityV))

        // Exercise the full range of the vertical movement. This should NOT fail out at this point. Consider this a "test" of the polarity check above.
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, 14))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 1023, 14))

        // Calibrate the horizontal polarity by going counterclockwise, logging a point, then clockwise, and logging another point.
        ->next(std::make_shared<SubCmdEngageAzClutch>(state))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, -200, 14, true))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateHorizontalPolarity>(state, referencePoint1))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, 200, 14, true))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateHorizontalPolarity>(state, referencePoint2))
        ->next(std::make_shared<SubCmdSetPolarity>(state, MovementAxis::Horizontal, referencePoint1, referencePoint2, state->calibration.polarityH))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, -250, 14, true)) // Move back to the home mount
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, 14))
        ->next(std::make_shared<SubCmdDisengageAzClutch>(state))

        // Calibrate the home mount position, then height
        // TO DO: Normalize sensor output RIGHT HERE
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, false, SubCommandId::CalibrateHome, StmShiftPin::Led33Rpm, state->calibration.home))
        ->next(std::make_shared<SubCmdMoveUpUntilLifted>(state, SubCommandId::CalibrateHomeHeight, 1))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateVerticalPoint>(state, referencePoint1))

        // Go to the middle of the platter
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 1023, 14))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, 150, 14, true))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, SET_DOWN_SLOWLY))

        // Calibrate the platter height
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, false, SubCommandId::CalibratePlatterHeight, StmShiftPin::Led45Rpm, dummyValue))
        ->next(std::make_shared<SubCmdMoveUpUntilLifted>(state, SubCommandId::CalibratePlatterHeight, 1))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateVerticalPoint>(state, referencePoint2))
        ->next(std::make_shared<SubCmdCalibrateVerticalBounds>(state, referencePoint1, referencePoint2))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, SET_DOWN_SLOWLY))
        ->next(std::make_shared<SubCmdToggleLight>(state, StmShiftPin::LedPlayStatus, false))

        // Now the user can calibrate the remaining record size positions
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, true, SubCommandId::Calibrate7In, StmShiftPin::Led7In, state->calibration.in7))
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, true, SubCommandId::Calibrate10In, StmShiftPin::Led10In, state->calibration.in10))
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, true, SubCommandId::Calibrate12In, StmShiftPin::Led12In, state->calibration.in12));
}

CommandId CmdCalibration::getCommandId() {
    return CommandId::Calibration;
}

void CmdCalibration::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, false);
    outputShiftValues = state->outputShift.getValues();
    state->calibration.polarityH = StmEncoderPolarity::NORMAL;
    state->calibration.polarityV = StmEncoderPolarity::NORMAL;
    state->azEncoder.setPolarity(state->calibration.polarityH);

    state->outputShift.setValues(0);
}

void CmdCalibration::doUninitialize() {
    state->outputShift.setValues(outputShiftValues);
    state->azEncoder.setPolarity(state->calibration.polarityH);
    state->calibration.persist();
}