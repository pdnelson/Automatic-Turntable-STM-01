#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <AzimuthDirection.h>

#ifndef SubCmdMoveNSteps_h
#define SubCmdMoveNSteps_h
class TurntableState;

class SubCmdMoveNSteps : public BaseTurntableSubCommand {
    public:
        SubCmdMoveNSteps(TurntableState* state, uint8_t speed, int16_t steps);

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