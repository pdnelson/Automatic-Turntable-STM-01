#include <CommandResult.h>
#include <ActionCommand.h>
#include <BaseTurntableCommand.h>
#include <CommandResult.h>

#ifndef CmdError_h
#define CmdError_h
class TurntableState;

class CmdError : public BaseTurntableCommand {
    public:
        CmdError(TurntableState* state, CommandResult error);

        ActionCommand getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
        CommandResult error;

    private:
        uint16_t outputShiftValues = 0;
};

#endif