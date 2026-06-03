#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>

#ifndef SubCmdDisengageAzClutch_h
#define SubCmdDisengageAzClutch_h
class TurntableState;

class SubCmdDisengageAzClutch : public BaseTurntableSubCommand {
    public:
        SubCmdDisengageAzClutch(TurntableState* state);

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        uint16_t totalSteps;
};

#endif