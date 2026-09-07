#include <CommandResult.h>
#include <BaseCalibrationSubCommand.h>
#include <SubCommandId.h>
#include <CmdCalibration.h>

#ifndef SubCmdCalibrate7In_h
#define SubCmdCalibrate7In_h
class TurntableState;

class SubCmdCalibrate7In : public BaseCalibrationSubCommand {
    public:
        SubCmdCalibrate7In(TurntableState* state, CmdCalibration* calCommand);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override; 
        unsigned long lightBlinkIndicator = 0;
};

#endif