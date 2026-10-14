#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>
#include <ClutchStatus.h>

#ifndef SubCmdSetAzClutch_h
#define SubCmdSetAzClutch_h
class TurntableState;

class SubCmdSetAzClutch : public BaseTurntableSubCommand {
    public:
        SubCmdSetAzClutch(TurntableState* state, ClutchStatus clutchStatus);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        uint16_t stepCount = 0;
        ClutchStatus clutchStatus;
};

#endif