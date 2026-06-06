#include <CommandId.h>
#include <CommandResult.h>
#include <memory>
#include <BaseTurntableSubCommand.h>
#include <SubCommandId.h>

#ifndef BaseTurntableCommand_h
#define BaseTurntableCommand_h

class TurntableState;

class BaseTurntableCommand {
    public:
        BaseTurntableCommand(TurntableState* state);

        virtual ~BaseTurntableCommand() = default;
        virtual CommandId getCommandId() = 0;
        TurntableState* state;
        std::shared_ptr<BaseTurntableSubCommand> subCommands;

        SubCommandId currentSubCommandId();

        CommandResult execute();
        

    private:
        std::shared_ptr<BaseTurntableSubCommand>& activeSubCommand(std::shared_ptr<BaseTurntableSubCommand>& nextSubCommand);
        virtual void doInitialize() = 0;
        virtual void doUninitialize() = 0;
        bool initialized = false;
        bool uninitialized = false;

        void initialize();
        void uninitialize();
};

#endif