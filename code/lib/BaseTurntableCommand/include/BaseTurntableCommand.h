#include <ActionCommand.h>
#include <CommandResult.h>
#include <TurntableState.h>

#ifndef BaseTurntableCommand_h
#define BaseTurntableCommand_h

class BaseTurntableCommand {
    public:
        BaseTurntableCommand(TurntableState* state);

        virtual ~BaseTurntableCommand() = default;
        virtual ActionCommand getCommandId() = 0;
        TurntableState* state;

        CommandResult execute();

    private:
        virtual CommandResult doExecute() = 0;
        virtual void doInitialize() = 0;
        virtual void doUninitialize() = 0;
        bool initialized = false;
        bool uninitialized = false;

        void initialize();
        void uninitialize();
};

#endif