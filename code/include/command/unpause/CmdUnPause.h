#include <state/TurntableState.h>
#include <command/base/CommandResult.h>
#include <command/base/ActionCommand.h>
#include <command/base/BaseTurntableCommand.h>
#include <command/sub/lift/down/SubCmdSetDownTonearm.h>

#ifndef CmdUnPause_h
#define CmdUnPause_h

class CmdUnPause : public BaseTurntableCommand {
    public:
        CmdUnPause(TurntableState* state);
        ActionCommand getCommandId() override;
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

    private:
        SubCmdSetDownTonearm setDownTonearm;
};

#endif