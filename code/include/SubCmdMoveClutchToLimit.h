#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>

#ifndef SubCmdMoveClutchToLimit_h
#define SubCmdMoveClutchToLimit_h
class TurntableState;

class SubCmdMoveClutchToLimit : public BaseTurntableSubCommand {
    public:
        SubCmdMoveClutchToLimit(TurntableState* state);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        uint16_t totalSteps;
};

#endif