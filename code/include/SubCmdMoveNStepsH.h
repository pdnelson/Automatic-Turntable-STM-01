#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <AzimuthDirection.h>
#include <SubCommandId.h>
#include <BaseHorizontalMovementSubCommand.h>

#ifndef SubCmdMoveNStepsH_h
#define SubCmdMoveNStepsH_h
class TurntableState;

class SubCmdMoveNStepsH : public BaseHorizontalMovementSubCommand {
    public:
        SubCmdMoveNStepsH(TurntableState* state, int16_t steps, uint8_t speed, bool releaseCurrentAfterMovement);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        bool releaseCurrentAfterMovement;
        uint16_t steps;
        uint16_t stepCount;
        AzimuthDirection direction;
};

#endif