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
#include <MovementAxis.h>
#include <SubCmdGoToPositionV.h>

CmdCalibration::CmdCalibration(TurntableState* state) : BaseTurntableCommand(state) {

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
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, -200, 14, true))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateHorizontalPolarity>(state, referencePoint1))
        ->next(std::make_shared<SubCmdMoveNStepsH>(state, 200, 14, true))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateHorizontalPolarity>(state, referencePoint2))
        ->next(std::make_shared<SubCmdSetPolarity>(state, MovementAxis::Horizontal, referencePoint1, referencePoint2, state->calibration.polarityH))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, 14))

        // Shut the "Play" light off, because the automated part of calibration is finished for now
        ->next(std::make_shared<SubCmdToggleLight>(state, StmShiftPin::LedPlayStatus, false))

        // Wait for the user to press "Play" (or "Pause," to skip) with the tonearm over the home position. 
        // This will also begin calibrating the home mount height.
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, SubCommandId::CalibrateHome, StmShiftPin::Led33Rpm, state->calibration.home))
        ->next(std::make_shared<SubCmdToggleLight>(state, StmShiftPin::LedPlayStatus, true))
        ->next(std::make_shared<SubCmdMoveUpUntilLifted>(state, SubCommandId::CalibrateHomeHeight, 1))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateVerticalPoint>(state, referencePoint1))
        ->next(std::make_shared<SubCmdGoToPositionV>(state, 0, 14))
        ->next(std::make_shared<SubCmdToggleLight>(state, StmShiftPin::LedPlayStatus, false))

        // Wait for the user to put the tonearm on the platter (WITHOUT A RECORD) and proceed
        // This will begin calibrating the platter height
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, SubCommandId::CalibratePlatterHeight, StmShiftPin::Led45Rpm, dummyValue))
        ->next(std::make_shared<SubCmdToggleLight>(state, StmShiftPin::LedPlayStatus, true))
        ->next(std::make_shared<SubCmdMoveUpUntilLifted>(state, SubCommandId::CalibratePlatterHeight, 1))
        ->next(std::make_shared<SubCmdDelay>(state, 200))
        ->next(std::make_shared<SubCmdCalibrateVerticalPoint>(state, referencePoint2))
        // to do: calibrate vertical limit
        ->next(std::make_shared<SubCmdGoToPositionV>(state, state->calibration.verticalLowerLimit, SET_DOWN_SLOWLY))
        ->next(std::make_shared<SubCmdToggleLight>(state, StmShiftPin::LedPlayStatus, false))

        // Now the user can calibrate the remaining record size positions
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, SubCommandId::Calibrate7In, StmShiftPin::Led7In, state->calibration.in7))
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, SubCommandId::Calibrate10In, StmShiftPin::Led10In, state->calibration.in10))
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, SubCommandId::Calibrate12In, StmShiftPin::Led12In, state->calibration.in12));

    // Basic controls:
    // "Play" advances to the next calibration step, saving the calibration value
    // "Pause" skips the calibration step, saving nothing

    // What's being saved?
    // 
    // - home az position
    // - upper vertical limit
    // - lower vertical limit
    // - az encoder polarity
    // - 7in az position
    // - 10in az position
    // - 12in az position

    // Routine:
    //
    // Calibrate basic movements
    // 1. User sets tonearm over home mount - saves home position + offset
    //   * 33 light flashes during this step
    // 2.1 Tonearm lowers to the lowest position
    //   * "Play" light illuminated during this step
    // 2.2 Tonearm automatically raises until it "finds" where the mount is - Saves mount location
    //   * "Play" light illuminated during this step
    // 3.1 Tonearm lowers to the lowest position
    //   * "Play" light illuminated during this step
    // 3.2 User sets tonearm on the stationary platter with a piece of paper on it (NO RECORD!) - Saves record location IF it is lower than the mount location
    //   * 45 light flashes during this step
    //   * This is to calibrate the tonearm set-down position such that a flexidisc could be played
    //
    // Calibrate standard sizes
    // 4. User sets tonearm on 7" record start - saves position
    //   * 7" light flashes during this step
    // 5. User sets tonearm on 10" record start - saves position
    //   * 10" light flashes during this step
    // 6. User sets tonearm on 12" record start - saves position
    //   * 12" light flashes during this step
    //
    // Calibrate IR sensor (cannot implement yet)
    // 7. User sets tonearm on test disc point 1 - logs position
    //   * Auto size light flashes during this step
    // 8. User sets tonearm on test disc point 2 - logs position
    //   * Auto size light flashes more quickly during this step
    // 9. Tonearm automatically scans test disc to detect the test points
    //   * "Play" light illuminated during this step
    //   * Now we have enough data to calculate tonearm movement radius and offset from the sensor
}

CommandId CmdCalibration::getCommandId() {
    return CommandId::Calibration;
}

void CmdCalibration::doInitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::LedPlayStatus, false);
    outputShiftValues = state->outputShift.getValues();

    state->outputShift.setValues(0);
}

void CmdCalibration::doUninitialize() {
    state->outputShift.setValues(outputShiftValues);
    state->azEncoder.setPolarity(state->calibration.polarityH);
    state->calibration.persist();
}