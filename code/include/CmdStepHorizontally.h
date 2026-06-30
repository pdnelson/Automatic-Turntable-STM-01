#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <AzimuthDirection.h>

#ifndef CmdStepHorizontally_h
#define CmdStepHorizontally_h
class TurntableState;

class CmdStepHorizontally : public BaseTurntableCommand {
    public:
        CmdStepHorizontally(TurntableState* state, int16_t steps, uint8_t azimuthSpeed);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
        CommandResult error;
        uint8_t speed;
        uint16_t steps;
        uint16_t stepCount;
        AzimuthDirection direction;
};

#endif