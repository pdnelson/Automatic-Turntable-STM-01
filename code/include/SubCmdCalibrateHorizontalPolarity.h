#include <Arduino.h>
#include <CommandResult.h>
#include <SubCommandId.h>
#include <CmdCalibration.h>
#include <RecordSize.h>
#include <BaseTurntableSubCommand.h>

#ifndef SubCmdCalibrateHorizontalPolarity_h
#define SubCmdCalibrateHorizontalPolarity_h
class TurntableState;

class SubCmdCalibrateHorizontalPolarity : public BaseTurntableSubCommand {
    public:
        SubCmdCalibrateHorizontalPolarity(TurntableState* state, uint16_t &destination);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        uint16_t &destination;
};

#endif