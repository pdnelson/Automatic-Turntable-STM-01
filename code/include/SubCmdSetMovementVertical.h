#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>

#ifndef SubCmdSetMovementVertical_h
#define SubCmdSetMovementVertical_h
class TurntableState;

class SubCmdSetMovementVertical : public BaseTurntableSubCommand {
    public:
        SubCmdSetMovementVertical(TurntableState* state);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
};

#endif