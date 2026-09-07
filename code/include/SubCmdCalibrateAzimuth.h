#include <CommandResult.h>
#include <BaseCalibrationSubCommand.h>
#include <SubCommandId.h>
#include <CmdCalibration.h>
#include <RecordSize.h>

#ifndef SubCmdCalibrateAzimuth_h
#define SubCmdCalibrateAzimuth_h
class TurntableState;

class SubCmdCalibrateAzimuth : public BaseCalibrationSubCommand {
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
};

#endif