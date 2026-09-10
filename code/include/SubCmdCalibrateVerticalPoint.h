#include <Arduino.h>
#include <CommandResult.h>
#include <SubCommandId.h>
#include <BaseTurntableSubCommand.h>

#ifndef SubCmdCalibrateVerticalPoint_h
#define SubCmdCalibrateVerticalPoint_h
class TurntableState;

class SubCmdCalibrateVerticalPoint : public BaseTurntableSubCommand {
    public:
        SubCmdCalibrateVerticalPoint(TurntableState* state, uint16_t &destination);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        uint16_t &destination;
};

#endif