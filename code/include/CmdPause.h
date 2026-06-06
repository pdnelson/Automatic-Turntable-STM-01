#include <CommandResult.h>
#include <ActionCommand.h>
#include <BaseTurntableCommand.h>

#ifndef CmdPause_h
#define CmdPause_h
class TurntableState;

class CmdPause : public BaseTurntableCommand {
    public:
        CmdPause(TurntableState* state);

        ActionCommand getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif