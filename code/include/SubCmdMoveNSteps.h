#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <AzimuthDirection.h>
#include <SubCommandId.h>

#ifndef SubCmdMoveNSteps_h
#define SubCmdMoveNSteps_h
class TurntableState;

class SubCmdMoveNSteps : public BaseTurntableSubCommand {
    public:
        SubCmdMoveNSteps(TurntableState* state, int16_t steps, uint8_t speed);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        uint8_t speed;
        uint16_t steps;
        uint16_t stepCount;
        AzimuthDirection direction;
};

#endif