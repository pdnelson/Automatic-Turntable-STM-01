#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdGoToPositionH_h
#define CmdGoToPositionH_h
class TurntableState;

class CmdGoToPositionH : public BaseTurntableCommand {
    public:
        CmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t delta, uint8_t speed);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif