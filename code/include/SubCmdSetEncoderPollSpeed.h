#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>
#include <EncoderPollSpeed.h>

#ifndef SubCmdSetEncoderPollSpeed_h
#define SubCmdSetEncoderPollSpeed_h
class TurntableState;

class SubCmdSetEncoderPollSpeed : public BaseTurntableSubCommand {
    public:
        SubCmdSetEncoderPollSpeed(TurntableState* state, EncoderPollSpeed speed);
        SubCommandId getSubCommandId() override;

    private:
        EncoderPollSpeed speed = EncoderPollSpeed::PollNormal;

        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif