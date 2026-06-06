#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdPause_h
#define CmdPause_h
class TurntableState;

class CmdPause : public BaseTurntableCommand {
    public:
        CmdPause(TurntableState* state);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif