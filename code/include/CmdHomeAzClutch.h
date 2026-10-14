#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdHomeAzClutch_h
#define CmdHomeAzClutch_h
class TurntableState;

class CmdHomeAzClutch : public BaseTurntableCommand {
    public:
        CmdHomeAzClutch(TurntableState* state);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif