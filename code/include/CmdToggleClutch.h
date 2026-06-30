#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdToggleClutch_h
#define CmdToggleClutch_h
class TurntableState;

class CmdToggleClutch : public BaseTurntableCommand {
    public:
        CmdToggleClutch(TurntableState* state);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
        CommandResult error;
};

#endif