#include <CommandResult.h>
#include <memory>
#include <SubCommandId.h>

#ifndef BaseTurntableSubCommand_h
#define BaseTurntableSubCommand_h
class TurntableState;

class BaseTurntableSubCommand : public std::enable_shared_from_this<BaseTurntableSubCommand> {
    public:
        BaseTurntableSubCommand(TurntableState* state);

        virtual SubCommandId getSubCommandId() = 0;

        TurntableState* state;
        std::shared_ptr<BaseTurntableSubCommand> nextSubCommand;

        CommandResult execute();

        void setCommandResult(CommandResult result);

        std::shared_ptr<BaseTurntableSubCommand> next(std::shared_ptr<BaseTurntableSubCommand> nextSubCommand);

        bool uninitialized = false;
    private:
        std::shared_ptr<BaseTurntableSubCommand>& nextEmptySubCommand(std::shared_ptr<BaseTurntableSubCommand>& nextSubCommand);
        CommandResult result = CommandResult::Running;
        virtual CommandResult doExecute() = 0;
        virtual void doInitialize() = 0;
        virtual void doUninitialize() = 0;
        bool initialized = false;

        void initialize();

        void uninitialize();
};

#endif