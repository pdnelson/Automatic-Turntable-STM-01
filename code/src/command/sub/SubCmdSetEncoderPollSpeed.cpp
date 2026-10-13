#include <SubCmdSetEncoderPollSpeed.h>
#include <Arduino.h>
#include <Pin.h>
#include <CommandResult.h>
#include <Constants.h>
#include <TurntableState.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <CommandResult.h>
#include <EncoderPollSpeed.h>

SubCmdSetEncoderPollSpeed::SubCmdSetEncoderPollSpeed(TurntableState* state, EncoderPollSpeed speed) : BaseTurntableSubCommand(state) {
    this->speed = speed;
}

void SubCmdSetEncoderPollSpeed::doInitialize() {
    state->azEncoder.setPollSpeed(speed);
}

CommandResult SubCmdSetEncoderPollSpeed::doExecute() {
    return CommandResult::Success;
}

void SubCmdSetEncoderPollSpeed::doUninitialize() {
    // Do nothing.
}

SubCommandId SubCmdSetEncoderPollSpeed::getSubCommandId() {
    return SubCommandId::SetPollSpeed;
}