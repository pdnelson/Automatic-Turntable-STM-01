#include <CommandResult.h>
#include <SubCommandId.h>
#include <BaseTurntableSubCommand.h>

#ifndef SubCmdZeroAzEncoder_h
#define SubCmdZeroAzEncoder_h
class TurntableState;

class SubCmdZeroAzEncoder : public BaseTurntableSubCommand {
    public:
        SubCmdZeroAzEncoder(TurntableState* state);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif