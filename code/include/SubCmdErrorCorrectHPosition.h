#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>
#include <AzimuthDirection.h>
#include <BaseHorizontalMovementSubCommand.h>

#ifndef SubCmdErrorCorrectHPosition_h
#define SubCmdErrorCorrectHPosition_h
class TurntableState;

class SubCmdErrorCorrectHPosition : public BaseHorizontalMovementSubCommand {
    public:
        SubCmdErrorCorrectHPosition(TurntableState* state, uint16_t position, uint8_t tolerance, float speed);
        SubCommandId getSubCommandId() override;

    private:
        uint16_t destinationEncoderPosition = 0;
        uint8_t encoderTolerance = 0;
        uint16_t stepCount = 0;
        uint16_t stepsToTake = 0;
        AzimuthDirection direction;

        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif