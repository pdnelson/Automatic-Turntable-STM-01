#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>

#ifndef SubCmdGoToPosition_h
#define SubCmdGoToPosition_h
class TurntableState;

class SubCmdGoToPositionH : public BaseTurntableSubCommand {
    public:
        SubCmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t tolerance, uint8_t speed);
        SubCommandId getSubCommandId() override;

    private:
        uint8_t speed = 0;
        uint16_t destinationEncoderPosition = 0;
        uint8_t encoderTolerance = 0;

        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif