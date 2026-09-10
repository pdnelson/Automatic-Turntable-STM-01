#include <SubCmdCalibrateVerticalBounds.h>
#include <CommandResult.h>
#include <SubCommandId.h>
#include <TurntableState.h>
#include <Constants.h>

SubCmdCalibrateVerticalBounds::SubCmdCalibrateVerticalBounds(TurntableState* state, uint16_t &homeMountHeight, uint16_t &platterHeight) : BaseTurntableSubCommand(state), homeMountHeight(homeMountHeight), platterHeight(platterHeight) {
    // Do nothing
}

SubCommandId SubCmdCalibrateVerticalBounds::getSubCommandId() {
    return SubCommandId::CalibrateVerticalBounds;
}

void SubCmdCalibrateVerticalBounds::doInitialize() {
    // Do nothing
}

CommandResult SubCmdCalibrateVerticalBounds::doExecute() {
    // Upper bound
    uint16_t upperBound = homeMountHeight;

    if(platterHeight > homeMountHeight) {
        upperBound = platterHeight;
    }

    state->calibration.verticalUpperLimit = upperBound + VERTICAL_BOUNDARY_UPPER_BUFFER;

    // Lower bound
    uint16_t lowerBound = homeMountHeight;

    if(platterHeight < homeMountHeight) {
        lowerBound = platterHeight;
    }

    state->calibration.verticalLowerLimit = lowerBound - VERTICAL_BOUNDARY_LOWER_BUFFER;

    return CommandResult::Success;
}

void SubCmdCalibrateVerticalBounds::doUninitialize() {
    // Dew nuthin'
}