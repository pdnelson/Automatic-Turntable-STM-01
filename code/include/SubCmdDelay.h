#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>

#ifndef SubCmdDelay_h
#define SubCmdDelay_h
class TurntableState;

class SubCmdDelay : public BaseTurntableSubCommand {
    public:
        SubCmdDelay(TurntableState* state, uint16_t delayMillis);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        uint16_t totalSteps;
        unsigned long delayMicros;
        unsigned long startTimeMicros;
};

#endif