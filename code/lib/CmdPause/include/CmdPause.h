#include <TurntableState.h>
#include <CommandResult.h>
#include <ActionCommand.h>
#include <BaseTurntableCommand.h>
#include <SubCmdLiftTonearm.h>

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