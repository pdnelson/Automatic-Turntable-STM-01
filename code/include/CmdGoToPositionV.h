#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdGoToPositionV_h
#define CmdGoToPositionV_h
class TurntableState;

class CmdGoToPositionV : public BaseTurntableCommand {
    public:
        CmdGoToPositionV(TurntableState* state, uint16_t position, uint8_t speed);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif