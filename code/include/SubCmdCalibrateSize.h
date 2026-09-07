#include <CommandResult.h>
#include <BaseCalibrationSubCommand.h>
#include <SubCommandId.h>
#include <CmdCalibration.h>
#include <RecordSize.h>

#ifndef SubCmdCalibrateSize_h
#define SubCmdCalibrateSize_h
class TurntableState;

class SubCmdCalibrateSize : public BaseCalibrationSubCommand {
    public:
        SubCmdCalibrateSize(TurntableState* state, CmdCalibration* calCommand, RecordSize sizeToCalibrate);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        RecordSize sizeToCalibrate;
};

#endif