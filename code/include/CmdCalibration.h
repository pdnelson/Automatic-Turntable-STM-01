#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdCalibration_h
#define CmdCalibration_h
class TurntableState;

class CmdCalibration : public BaseTurntableCommand {
    public:
        CmdCalibration(TurntableState* state);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;

    private:
        uint16_t outputShiftValues = 0;
};

#endif