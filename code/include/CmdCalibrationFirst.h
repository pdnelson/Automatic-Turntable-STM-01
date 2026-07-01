#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>

#ifndef CmdCalibrationFirst_h
#define CmdCalibrationFirst_h
class TurntableState;

//class CmdCalibrationFirst : public BaseTurntableCommand {
//    public:
//        CmdCalibrationFirst(TurntableState* state, CommandResult error);
//
//        CommandId getCommandId() override;
//        void doInitialize() override;
//        void doUninitialize() override;
//};

/**
 * To do: Implement. This routine will run the first time the turntable needs set up. It should only be run while connected to a computer.
 * The following things will need calibrated:
 * - The home position
 * - The horizontal encoder polarity
 * - The vertical encoder direction
 * - The vertical upper and lower bounds
 * - 7" start position
 * - 10" start position
 * - 12" start position
 * - Automatic IR reference calculation (will calculate between two points)
 */

#endif