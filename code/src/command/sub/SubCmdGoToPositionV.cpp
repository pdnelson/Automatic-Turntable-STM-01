#include <SubCmdGoToPositionV.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <CommandResult.h>

SubCmdGoToPositionV::SubCmdGoToPositionV(TurntableState* state, uint16_t position, uint8_t speed, bool skipLiftCheck) : BaseLiftSubCommand(state, position, speed, skipLiftCheck) {

}

void SubCmdGoToPositionV::doInitialize() {
    baseInitialize();
}

CommandResult SubCmdGoToPositionV::doExecute() {
    return executeLift();
}

void SubCmdGoToPositionV::doUninitialize() {
    state->verticalStepper.releaseMotorCurrent();
}

SubCommandId SubCmdGoToPositionV::getSubCommandId() {
    return SubCommandId::SubGoToPositionV;
}