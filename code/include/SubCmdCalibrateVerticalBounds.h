#include <SubCommandId.h>
#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>

#ifndef SubCmdCalibrateVerticalBounds_h
#define SubCmdCalibrateVerticalBounds_h
class TurntableState;

class SubCmdCalibrateVerticalBounds : public BaseTurntableSubCommand {
    public:
        SubCmdCalibrateVerticalBounds(TurntableState* state, uint16_t &homeMountHeight, uint16_t &platterHeight);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        uint16_t &homeMountHeight;
        uint16_t &platterHeight;
};

#endif