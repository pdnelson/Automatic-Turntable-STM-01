#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdScanRecord_h
#define CmdScanRecord_h

class TurntableState;

class CmdScanRecord : public BaseTurntableCommand {
    public:
        CmdScanRecord(TurntableState* state);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif