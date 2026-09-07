#include <SubCmdCalibrateSize.h>
#include <CmdCalibration.h>
#include <SubCommandId.h>
#include <CommandResult.h>
#include <TurntableState.h>
#include <ButtonResult.h>
#include <MuxPin.h>
#include <Constants.h>
#include <RecordSize.h>
#include <StmShiftPin.h>
#include <SubCommandId.h>

SubCmdCalibrateSize::SubCmdCalibrateSize(TurntableState* state, CmdCalibration* calCommand, RecordSize sizeToCalibrate) : BaseCalibrationSubCommand(state, calCommand) {
    this->sizeToCalibrate = sizeToCalibrate;
}

SubCommandId SubCmdCalibrateSize::getSubCommandId() {
    switch(sizeToCalibrate) {
        case RecordSize::In7: return SubCommandId::Calibrate7In; break;
        case RecordSize::In10: return SubCommandId::Calibrate10In; break;
        case RecordSize::In12: return SubCommandId::Calibrate12In; break;
        default: return SubCommandId::Calibrate7In;
    }
}

void SubCmdCalibrateSize::doInitialize() {
    state->outputShift.setValues(0);
}

CommandResult SubCmdCalibrateSize::doExecute() {
    switch(sizeToCalibrate) {
        case RecordSize::In7: 
            return baseSizeExecute(StmShiftPin::Led7In, state->calibration.in7, state->azEncoder.getNormalizedPosition()); 
            break;
        case RecordSize::In10: 
            return baseSizeExecute(StmShiftPin::Led10In, state->calibration.in10, state->azEncoder.getNormalizedPosition());
            break;
        case RecordSize::In12:
            return baseSizeExecute(StmShiftPin::Led12In, state->calibration.in12, state->azEncoder.getNormalizedPosition()); 
            break;
        default: return CommandResult::Success; break;
    }
}

void SubCmdCalibrateSize::doUninitialize() {
    // do nothing
}