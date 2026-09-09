#include <Arduino.h>
#include <CommandResult.h>
#include <SubCommandId.h>
#include <CmdCalibration.h>
#include <RecordSize.h>
#include <StmShiftPin.h>
#include <BaseTurntableSubCommand.h>

#ifndef SubCmdCalibrateVerticalPolarity_h
#define SubCmdCalibrateVerticalPolarity_h
class TurntableState;

class SubCmdCalibrateVerticalPolarity : public BaseTurntableSubCommand {
    public:
        SubCmdCalibrateVerticalPolarity(TurntableState* state, uint16_t &destination);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        uint16_t &destination;
};

#endif