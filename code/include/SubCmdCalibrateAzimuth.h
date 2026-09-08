#include <CommandResult.h>
#include <SubCommandId.h>
#include <CmdCalibration.h>
#include <RecordSize.h>
#include <StmShiftPin.h>

#ifndef SubCmdCalibrateAzimuth_h
#define SubCmdCalibrateAzimuth_h
class TurntableState;

class SubCmdCalibrateAzimuth : public BaseTurntableSubCommand {
    public:
        SubCmdCalibrateAzimuth(TurntableState* state, CmdCalibration* calCommand, SubCommandId subCommandId, StmShiftPin pinToFlash, uint16_t &destination);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        SubCommandId subCommandId;
        StmShiftPin pinToFlash;
        uint16_t &destination;
        unsigned long lightBlinkIndicator = 0;
};

#endif