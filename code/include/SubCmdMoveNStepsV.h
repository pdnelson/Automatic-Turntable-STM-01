#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <VerticalDirection.h>
#include <SubCommandId.h>

#ifndef SubCmdMoveNStepsV_h
#define SubCmdMoveNStepsV_h
class TurntableState;

class SubCmdMoveNStepsV : public BaseTurntableSubCommand {
    public:
        SubCmdMoveNStepsV(TurntableState* state, int16_t steps, uint8_t speed, bool releaseCurrentAfterMovement);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        bool releaseCurrentAfterMovement;
        uint8_t speed;
        uint16_t steps;
        uint16_t stepCount;
        VerticalDirection direction;
};

#endif