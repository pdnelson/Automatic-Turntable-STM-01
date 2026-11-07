#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdFirstCalibration_h
#define CmdFirstCalibration_h

#define DELAY_BETWEEN_STEPS_MS 100

class TurntableState;

class CmdFirstCalibration : public BaseTurntableCommand {
    public:
        CmdFirstCalibration(TurntableState* state);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;

    private:
        uint16_t outputShiftValues = 0;

        uint16_t referencePoint1 = 0;
        uint16_t referencePoint2 = 0;
};

#endif