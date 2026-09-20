#include <CommandResult.h>
#include <SubCommandId.h>
#include <StmPolarity.h>
#include <BaseTurntableSubCommand.h>
#include <Arduino.h>

#ifndef SubCmdSetVerticalPolarity_h
#define SubCmdSetVerticalPolarity_h
class TurntableState;
class CmdCalibration;

class SubCmdSetVerticalPolarity : public BaseTurntableSubCommand {
    public:
        SubCmdSetVerticalPolarity(TurntableState* state, uint16_t &lowerReferencePoint, uint16_t &upperReferencePoint, StmPolarity &destination);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        uint16_t &lowerReferencePoint;
        uint16_t &upperReferencePoint;
        StmPolarity &destination;
};

#endif