#include <CommandResult.h>
#include <SubCommandId.h>
#include <StmPolarity.h>
#include <BaseTurntableSubCommand.h>
#include <Arduino.h>

#ifndef SubCmdSetPolarity_h
#define SubCmdSetPolarity_h
class TurntableState;
class CmdCalibration;

// The number of encoder ticks that the reference points must differ by to allow successful calibration
#define DIFFERENCE_BUFFER 25

class SubCmdSetPolarity : public BaseTurntableSubCommand {
    public:
        SubCmdSetPolarity(TurntableState* state, uint16_t &lowerReferencePoint, uint16_t &upperReferencePoint, bool isVertical, StmPolarity &destination);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        uint16_t &lowerReferencePoint;
        uint16_t &upperReferencePoint;
        StmPolarity &destination;
        bool isVertical = false;
};

#endif