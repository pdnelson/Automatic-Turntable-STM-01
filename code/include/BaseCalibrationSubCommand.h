#include <CommandResult.h>
#include <BaseTurntableCommand.h>

#ifndef BaseCalibrationSubCommand_h
#define BaseCalibrationSubCommand_h
class TurntableState;
class CmdCalibration;

class BaseCalibrationSubCommand : public BaseTurntableSubCommand {
    public:
        BaseCalibrationSubCommand(TurntableState* state, CmdCalibration* calCommand);

        CmdCalibration* calCommand;
};

#endif