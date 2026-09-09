#include <SubCmdToggleLight.h>
#include <TurntableState.h>

SubCmdToggleLight::SubCmdToggleLight(TurntableState* state, StmShiftPin lightToToggle, bool value) : BaseTurntableSubCommand(state) {
    this->lightToToggle = lightToToggle;
    this->value = value;
}

SubCommandId SubCmdToggleLight::getSubCommandId() {
    return SubCommandId::ToggleLight;
}

void SubCmdToggleLight::doInitialize() {
    // do nothing
}

CommandResult SubCmdToggleLight::doExecute() {
    state->outputShift.setValue(lightToToggle, value);
    return CommandResult::Success;
}

void SubCmdToggleLight::doUninitialize() {
    // do nothing
}
