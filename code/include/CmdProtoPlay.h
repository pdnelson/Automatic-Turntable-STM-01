#include <CommandResult.h>
#include <ActionCommand.h>
#include <BaseTurntableCommand.h>
#include <SubCmdLiftTonearm.h>

#ifndef CmdProtoPlay_h
#define CmdProtoPlay_h
class TurntableState;

class CmdProtoPlay : public BaseTurntableCommand {
    public:
        CmdProtoPlay(TurntableState* state);

        ActionCommand getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif