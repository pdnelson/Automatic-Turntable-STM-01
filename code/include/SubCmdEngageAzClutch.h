#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>

#ifndef SubCmdEngageAzClutch_h
#define SubCmdEngageAzClutch_h
class TurntableState;

class SubCmdEngageAzClutch : public BaseTurntableSubCommand {
    public:
        SubCmdEngageAzClutch(TurntableState* state);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        uint16_t stepsBeforeStatusChange;
        uint16_t stepsSinceStatusChange;
};

#endif