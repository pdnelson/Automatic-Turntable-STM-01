#include <CommandResult.h>
#include <SubCommandId.h>
#include <StmPolarity.h>
#include <BaseTurntableSubCommand.h>
#include <Arduino.h>

#ifndef SubCmdSetHMotorPolarity_h
#define SubCmdSetHMotorPolarity_h
class TurntableState;
class CmdCalibration;

class SubCmdSetHMotorPolarity : public BaseTurntableSubCommand {
    public:
        SubCmdSetHMotorPolarity(TurntableState* state, uint16_t &lowerReferencePoint, uint16_t &upperReferencePoint, StmPolarity &destination);
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