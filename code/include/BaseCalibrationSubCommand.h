#include <Arduino.h>
#include <CommandResult.h>
#include <BaseTurntableCommand.h>
#include <StmShiftPin.h>

#ifndef BaseCalibrationSubCommand_h
#define BaseCalibrationSubCommand_h
class TurntableState;
class CmdCalibration;

class BaseCalibrationSubCommand : public BaseTurntableSubCommand {
    public:
        BaseCalibrationSubCommand(TurntableState* state, CmdCalibration* calCommand);

        CmdCalibration* calCommand;

        CommandResult baseExecute(StmShiftPin pin, uint16_t &destination, uint16_t content);

    private:
        unsigned long lightBlinkIndicator = 0;
};

#endif