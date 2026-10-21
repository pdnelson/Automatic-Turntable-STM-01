#include <CommandResult.h>
#include <BaseLiftSubCommand.h>
#include <SubCommandId.h>
#include <VerticalDirection.h>
#include <ClutchStatus.h>

#ifndef SubCmdGoToPositionVAndSetClutch_h
#define SubCmdGoToPositionVAndSetClutch_h
class TurntableState;

class SubCmdGoToPositionVAndSetClutch : public BaseLiftSubCommand {
    public:
        SubCmdGoToPositionVAndSetClutch(TurntableState* state, uint16_t position, uint8_t speed, bool skipLiftCheck, ClutchStatus clutchStatus, bool waitUntilLiftedBeforeSettingClutch);
        SubCommandId getSubCommandId() override;

    private:
        CommandResult executeClutch();

        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        uint16_t stepCount = 0;
        ClutchStatus clutchStatus;
        CommandResult clutchResult = CommandResult::Running;
        CommandResult liftResult = CommandResult::Running;
        bool liftHasChanged = false;
        bool waitUntilLiftedBeforeSettingClutch = false;
};

#endif