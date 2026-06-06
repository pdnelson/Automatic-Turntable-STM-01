#include <ActionCommand.h>
#include <CommandResult.h>
#include <memory>
#include <BaseTurntableSubCommand.h>

#ifndef BaseTurntableCommand_h
#define BaseTurntableCommand_h

class TurntableState;

class BaseTurntableCommand {
    public:
        BaseTurntableCommand(TurntableState* state);

        virtual ~BaseTurntableCommand() = default;
        virtual ActionCommand getCommandId() = 0;
        TurntableState* state;
        std::shared_ptr<BaseTurntableSubCommand> subCommands;

        CommandResult execute();

    private:
        virtual void doInitialize() = 0;
        virtual void doUninitialize() = 0;
        bool initialized = false;
        bool uninitialized = false;

        void initialize();
        void uninitialize();
};

#endif