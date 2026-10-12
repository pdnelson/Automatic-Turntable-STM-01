#include <CommandResult.h>
#include <BaseLiftSubCommand.h>
#include <SubCommandId.h>
#include <VerticalDirection.h>

#ifndef SubCmdGoToPositionV_h
#define SubCmdGoToPositionV_h
class TurntableState;

class SubCmdGoToPositionV : public BaseLiftSubCommand {
    public:
        SubCmdGoToPositionV(TurntableState* state, uint16_t position, uint8_t speed, bool skipLiftCheck);
        SubCommandId getSubCommandId() override;

    private:
        uint16_t destinationEncoderPosition = 0;
        bool reachedLimit = false;
        unsigned long timeLimitReached = 0;
        VerticalDirection direction;
        bool skipLiftCheck = false;

        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif