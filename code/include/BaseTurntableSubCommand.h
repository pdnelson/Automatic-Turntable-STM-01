#include <CommandResult.h>
#include <memory>

#ifndef BaseTurntableSubCommand_h
#define BaseTurntableSubCommand_h
class TurntableState;

class BaseTurntableSubCommand : public std::enable_shared_from_this<BaseTurntableSubCommand> {
    public:
        BaseTurntableSubCommand(TurntableState* state);

        TurntableState* state;
        std::shared_ptr<BaseTurntableSubCommand> nextSubCommand;

        CommandResult execute();

        std::shared_ptr<BaseTurntableSubCommand> next(std::shared_ptr<BaseTurntableSubCommand> nextSubCommand);

    private:
        std::shared_ptr<BaseTurntableSubCommand>& nextEmptySubCommand(std::shared_ptr<BaseTurntableSubCommand>& nextSubCommand);
        CommandResult result = CommandResult::Running;
        virtual CommandResult doExecute() = 0;
        virtual void doInitialize() = 0;
        virtual void doUninitialize() = 0;
        bool initialized = false;
        bool uninitialized = false;

        void initialize();

        void uninitialize();
};

#endif