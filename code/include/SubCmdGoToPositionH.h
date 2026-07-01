#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>

#ifndef SubCmdGoToPosition_h
#define SubCmdGoToPosition_h
class TurntableState;

class SubCmdGoToPositionH : public BaseTurntableSubCommand {
    public:
        SubCmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t speed);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif