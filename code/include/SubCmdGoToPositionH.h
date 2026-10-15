#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>
#include <BaseHorizontalMovementSubCommand.h>

#ifndef SubCmdGoToPosition_h
#define SubCmdGoToPosition_h
class TurntableState;

// The minimum travel distance in order for this routine to be used. Otherwise, we can just rely on the error correction routines.
#define MIN_TRAVEL_DISTANCE 20

class SubCmdGoToPositionH : public BaseHorizontalMovementSubCommand {
    public:
        SubCmdGoToPositionH(TurntableState* state, uint16_t position, uint8_t tolerance, uint8_t speed);
        SubCommandId getSubCommandId() override;

    private:
        AzimuthDirection direction = AzimuthDirection::Clockwise;
        uint16_t destinationEncoderPosition = 0;
        uint8_t encoderTolerance = 0;
        uint16_t stepCount = 0;
        uint16_t stepsToTake = 0;

        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif