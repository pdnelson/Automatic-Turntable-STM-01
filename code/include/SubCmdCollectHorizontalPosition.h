#include <CommandResult.h>
#include <SubCommandId.h>
#include <CmdCalibration.h>
#include <RecordSize.h>
#include <StmShiftPin.h>

#ifndef SubCmdCollectHorizontalPosition_h
#define SubCmdCollectHorizontalPosition_h
class TurntableState;

class SubCmdCollectHorizontalPosition : public BaseTurntableSubCommand {
    public:
        SubCmdCollectHorizontalPosition(TurntableState* state, bool waitForUserInput, SubCommandId subCommandId, StmShiftPin pinToFlash, uint16_t &destination);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        void updateDestination();

        bool waitForUserInput;
        SubCommandId subCommandId;
        StmShiftPin pinToFlash;
        uint16_t &destination;
        unsigned long lightBlinkIndicator = 0;
};

#endif