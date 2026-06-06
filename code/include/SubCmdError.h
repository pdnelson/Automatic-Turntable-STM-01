#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>

#ifndef SubCmdError_h
#define SubCmdError_h
class TurntableState;

class SubCmdError : public BaseTurntableSubCommand {
    public:
        SubCmdError(TurntableState* state);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        unsigned long powerBlinkCounter = 0;
        unsigned long timeInitialized = 0;
};

#endif