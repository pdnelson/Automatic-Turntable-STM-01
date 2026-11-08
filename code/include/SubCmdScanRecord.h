#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>
#include <BaseHorizontalMovementSubCommand.h>

#ifndef SubCmdScanRecord_h
#define SubCmdScanRecord_h
class TurntableState;

class SubCmdScanRecord : public BaseHorizontalMovementSubCommand {
    public:
        SubCmdScanRecord(TurntableState* state, float speed);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif