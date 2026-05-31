#include <state/TurntableState.h>
#include <command/base/CommandResult.h>
#include <command/sub/lift/base/BaseLiftSubCommand.h>

#ifndef SubCmdLiftTonearm_h
#define SubCmdLiftTonearm_h

class SubCmdLiftTonearm : public BaseLiftSubCommand {
    public:
        SubCmdLiftTonearm(TurntableState* state, uint8_t speed);

    private:
        bool reachedLimit = false;
        unsigned long timeLimitReached = 0;

        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif