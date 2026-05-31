#include <state/TurntableState.h>
#include <command/base/CommandResult.h>
#include <command/base/ActionCommand.h>
#include <command/base/BaseTurntableCommand.h>
#include <command/sub/lift/up/SubCmdLiftTonearm.h>

#ifndef CmdPause_h
#define CmdPause_h

class CmdPause : public BaseTurntableCommand {
    public:
        CmdPause(TurntableState* state);

        ActionCommand getCommandId() override;
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

    private:
        SubCmdLiftTonearm liftTonearm;
};

#endif