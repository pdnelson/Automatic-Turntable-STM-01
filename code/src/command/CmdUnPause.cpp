#include <CmdUnPause.h>
#include <StmShiftPin.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <Constants.h>
#include <BaseTurntableCommand.h>
#include <SubCmdGoToPositionV.h>
#include <TurntableState.h>
#include <Constants.h>

CmdUnPause::CmdUnPause(TurntableState* state) : BaseTurntableCommand(state) {
    uint16_t currentPosition = state->azEncoder.getNormalizedPosition();
    uint16_t speed = SET_DOWN_SLOWLY;
    uint16_t difference = abs((int16_t)currentPosition - (int16_t)state->calibration.home);
    
    // If we're near the home position, go down quickly.
    if(difference <= VERTICAL_HOME_THRESHOLD) {
        speed = SET_DOWN_QUICKLY;
    }
    
    subCommands = std::make_unique<SubCmdGoToPositionV>(state, state->calibration.verticalLowerLimit, speed);
}

CommandId CmdUnPause::getCommandId() {
    return CommandId::UnPause;
}

void CmdUnPause::doInitialize() {
    // do nothing
}

void CmdUnPause::doUninitialize() {
    state->outputShift.setValue(StmShiftPin::LedPauseStatus, false);
    state->outputShift.setValue(StmShiftPin::AudioCutOff, false);
}