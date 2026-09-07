#include <BaseCalibrationSubCommand.h>

BaseCalibrationSubCommand::BaseCalibrationSubCommand(TurntableState* state, CmdCalibration* calCommand) : BaseTurntableSubCommand(state) {
    this->calCommand = calCommand;
}