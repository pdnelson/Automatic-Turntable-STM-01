#include <SubCmdCalibrate7In.h>
#include <CmdCalibration.h>
#include <SubCommandId.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <ButtonResult.h>
#include <MuxPin.h>
#include <Constants.h>

SubCmdCalibrate7In::SubCmdCalibrate7In(TurntableState* state, CmdCalibration* calCommand) : BaseCalibrationSubCommand(state, calCommand) {
    // do nothing
}

SubCommandId SubCmdCalibrate7In::getSubCommandId() {
    return SubCommandId::Calibrate7In;
}

void SubCmdCalibrate7In::doInitialize() {
    state->outputShift.setValues(0);
}

CommandResult SubCmdCalibrate7In::doExecute() {
    return baseSizeExecute(StmShiftPin::Led7In, state->calibration.in7, state->azEncoder.getNormalizedPosition());
}

void SubCmdCalibrate7In::doUninitialize() {
    // do nothing
}