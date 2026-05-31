#include <BaseTurntableSubCommand.h>
#include <CommandResult.h>
#include <TurntableState.h>

BaseTurntableSubCommand::BaseTurntableSubCommand(TurntableState* state) {
    this->state = state;
}

CommandResult BaseTurntableSubCommand::execute() {
    initialize();

    // The command is finished
    if(result != CommandResult::Running) {
        uninitialize();
    
        // The command failed, or succeeded without a next command
        if(result > CommandResult::Success || nextSubCommand == nullptr) {
            return result;
        }
        
        // The command succeeded, and there is another to execute
        else {
            return nextSubCommand->execute();
        }
    }
    
    // The command is still running
    else {
        result = doExecute();
        return CommandResult::Running;
    }
}

void BaseTurntableSubCommand::initialize() {
    if(!initialized) {
        doInitialize();
        initialized = true;
    }
}

void BaseTurntableSubCommand::uninitialize() {
    if(!uninitialized) {
        doUninitialize();
        uninitialized = true;
    }
}