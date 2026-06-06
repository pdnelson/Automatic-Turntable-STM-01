#include <CommandResult.h>
#include <BaseLiftSubCommand.h>
#include <SubCommandId.h>

#ifndef SubCmdSetDownTonearm_h
#define SubCmdSetDownTonearm_h
class TurntableState;

class SubCmdSetDownTonearm : public BaseLiftSubCommand {
    public:
        SubCmdSetDownTonearm(TurntableState* state, uint8_t speed);
        SubCommandId getSubCommandId() override;

    private:
        bool isSetDown = false;
        int setDownPosition = 0;

        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif