#include <SubCmdDelay.h>
#include <CommandResult.h>
#include <SubCommandId.h>
#include <TurntableState.h>

SubCmdDelay::SubCmdDelay(TurntableState* state, uint16_t delayMillis) : BaseTurntableSubCommand(state) {
    this->state = state;
    delayMicros = delayMillis * 1000;
}

SubCommandId SubCmdDelay::getSubCommandId() {
    return SubCommandId::SubDelay;
}

void SubCmdDelay::doInitialize() {
    startTimeMicros = state->clockMicros;
}

CommandResult SubCmdDelay::doExecute() {
    if(state->clockMicros - startTimeMicros >= delayMicros) {
        return CommandResult::Success;
    } else {
        return CommandResult::Running;
    }
}

void SubCmdDelay::doUninitialize() {
    /* Do nothing!!! */
}