#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <ClutchStatus.h>

#ifndef CmdSetAzClutch_h
#define CmdSetAzClutch_h
class TurntableState;

class CmdSetAzClutch : public BaseTurntableCommand {
    public:
        CmdSetAzClutch(TurntableState* state, ClutchStatus clutchStatus);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif