#include <CommandResult.h>
#include <BaseLiftSubCommand.h>
#include <SubCommandId.h>
#include <VerticalDirection.h>

#ifndef SubCmdMoveUpUntilLifted_h
#define SubCmdMoveUpUntilLifted_h
class TurntableState;

class SubCmdMoveUpUntilLifted : public BaseLiftSubCommand {
    public:
        SubCmdMoveUpUntilLifted(TurntableState* state, SubCommandId subCommandId, uint8_t speed);
        SubCommandId getSubCommandId() override;

    private:
        void doInitialize() override;
        CommandResult doExecute() override;
        void doUninitialize() override;
        
        SubCommandId subCommandId;
};

#endif