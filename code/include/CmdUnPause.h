#include <Arduino.h>
#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdUnPause_h
#define CmdUnPause_h
class TurntableState;

class CmdUnPause : public BaseTurntableCommand {
    public:
        CmdUnPause(TurntableState* state);
        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif