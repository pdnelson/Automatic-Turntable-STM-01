#include <TurntableState.h>
#include <CommandResult.h>

#ifndef BaseTurntableSubCommand_h
#define BaseTurntableSubCommand_h

class BaseTurntableSubCommand {
    public:
        BaseTurntableSubCommand(TurntableState* state);

        TurntableState* state;
        std::unique_ptr<BaseTurntableSubCommand> nextSubCommand;

        CommandResult execute();

    private:
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