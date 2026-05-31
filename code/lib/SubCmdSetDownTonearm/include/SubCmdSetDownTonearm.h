#include <TurntableState.h>
#include <CommandResult.h>
#include <BaseLiftSubCommand.h>

#ifndef SubCmdSetDownTonearm_h
#define SubCmdSetDownTonearm_h

class SubCmdSetDownTonearm : public BaseLiftSubCommand {
    public:
        SubCmdSetDownTonearm(TurntableState* state, uint8_t speed);

    private:
        bool isSetDown = false;
        int setDownPosition = 0;

        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif