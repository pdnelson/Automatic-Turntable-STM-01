#include <CommandResult.h>
#include <ActionCommand.h>
#include <BaseTurntableCommand.h>
#include <SubCmdSetDownTonearm.h>

#ifndef CmdUnPause_h
#define CmdUnPause_h
class TurntableState;

class CmdUnPause : public BaseTurntableCommand {
    public:
        CmdUnPause(TurntableState* state);
        ActionCommand getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif