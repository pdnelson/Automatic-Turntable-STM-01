#include <CommandResult.h>
#include <CommandId.h>
#include <BaseTurntableCommand.h>
#include <SubCmdLiftTonearm.h>

#ifndef CmdProtoPlay_h
#define CmdProtoPlay_h
class TurntableState;

class CmdProtoPlay : public BaseTurntableCommand {
    public:
        CmdProtoPlay(TurntableState* state, int16_t steps, uint8_t azimuthSpeed);

        CommandId getCommandId() override;
        void doInitialize() override;
        void doUninitialize() override;
};

#endif