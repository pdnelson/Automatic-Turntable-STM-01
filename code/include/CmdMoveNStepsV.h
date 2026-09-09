#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdMoveNStepsV_h
#define CmdMoveNStepsV_h
class TurntableState;

class CmdMoveNStepsV : public BaseTurntableCommand {
    public:
        CmdMoveNStepsV(TurntableState* state, int16_t steps, uint8_t speed, bool releaseCurrentAfterMovement);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif