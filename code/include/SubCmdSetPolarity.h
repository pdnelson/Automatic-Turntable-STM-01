#include <CommandResult.h>
#include <SubCommandId.h>
#include <StmEncoderPolarity.h>
#include <BaseTurntableSubCommand.h>
#include <Arduino.h>
#include <MovementAxis.h>

#ifndef SubCmdSetPolarity_h
#define SubCmdSetPolarity_h
class TurntableState;
class CmdCalibration;

class SubCmdSetPolarity : public BaseTurntableSubCommand {
    public:
        SubCmdSetPolarity(TurntableState* state, MovementAxis axis, uint16_t lowerReferencePoint, uint16_t upperReferencePoint, StmEncoderPolarity &destination);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        MovementAxis axis;
        uint16_t lowerReferencePoint = 0;
        uint16_t upperReferencePoint = 0;
        StmEncoderPolarity &destination;
};

#endif