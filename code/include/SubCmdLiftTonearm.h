#include <CommandResult.h>
#include <BaseLiftSubCommand.h>
#include <SubCommandId.h>

#ifndef SubCmdLiftTonearm_h
#define SubCmdLiftTonearm_h
class TurntableState;

class SubCmdLiftTonearm : public BaseLiftSubCommand {
    public:
        SubCmdLiftTonearm(TurntableState* state, uint8_t speed);
        SubCommandId getSubCommandId() override;

    private:
        bool reachedLimit = false;
        unsigned long timeLimitReached = 0;

        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif