#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>

#ifndef SubCmdMoveClutchToHome_h
#define SubCmdMoveClutchToHome_h
class TurntableState;

class SubCmdMoveClutchToHome : public BaseTurntableSubCommand {
    public:
        SubCmdMoveClutchToHome(TurntableState* state);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        uint16_t stepsBeforeStatusChange;
        uint16_t stepsSinceStatusChange;
};

#endif