#include <Arduino.h>
#include <CommandResult.h>
#include <SubCommandId.h>
#include <BaseTurntableSubCommand.h>

#ifndef SubCmdCollectVerticalPosition_h
#define SubCmdCollectVerticalPosition_h
class TurntableState;

class SubCmdCollectVerticalPosition : public BaseTurntableSubCommand {
    public:
        SubCmdCollectVerticalPosition(TurntableState* state, uint16_t &destination);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        uint16_t &destination;
};

#endif