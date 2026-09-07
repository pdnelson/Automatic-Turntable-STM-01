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

CmdCalibration::CmdCalibration(TurntableState* state) : BaseTurntableCommand(state) {
    subCommands = std::make_shared<SubCmdCalibrateAzimuth>(state, this, SubCommandId::Calibrate7In, StmShiftPin::Led7In, state->calibration.in7)
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, this, SubCommandId::Calibrate10In, StmShiftPin::Led10In, state->calibration.in10))
        ->next(std::make_shared<SubCmdCalibrateAzimuth>(state, this, SubCommandId::Calibrate12In, StmShiftPin::Led12In, state->calibration.in12));

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
    // 2. Tonearm automatically lowers or raises until it "finds" where the mount is - Saves mount location
    //   * "Play" light illuminated during this step
    // 3. User sets tonearm on the stationary platter with a piece of paper on it (NO RECORD!) - Saves record location IF it is lower than the mount location
    //   * 78 light flashes during this step
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

    // TO DO: Save values to EEPROM
}