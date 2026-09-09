#include <StmShiftPin.h>
#include <SubCommandId.h>
#include <CommandResult.h>
#include <BaseTurntableSubCommand.h>

#ifndef SubCmdToggleLight_h
#define SubCmdToggleLight_h
class TurntableState;

class SubCmdToggleLight : public BaseTurntableSubCommand {
    public:
        SubCmdToggleLight(TurntableState* state, StmShiftPin lightToToggle, bool value);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;

        StmShiftPin lightToToggle;
        bool value;
};

#endif