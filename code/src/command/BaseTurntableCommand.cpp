#include <BaseTurntableCommand.h>

BaseTurntableCommand::BaseTurntableCommand(TurntableState* state) {
    this->state = state;
}

SubCommandId BaseTurntableCommand::currentSubCommandId() {
    if(this->subCommands == nullptr) {
        return SubCommandId::NoSubCommand;
    } else {
        return activeSubCommand(this->subCommands)->getSubCommandId();
    }
}

std::shared_ptr<BaseTurntableSubCommand>& BaseTurntableCommand::activeSubCommand(std::shared_ptr<BaseTurntableSubCommand>& nextSubCommand) {
    
    // Sub command has concluded
    if(nextSubCommand->uninitialized) {

        // If there is no subcommand next, then this is the last one
        if(nextSubCommand->nextSubCommand == nullptr) {
            return nextSubCommand;
        }

        // Otherwise, drill down deeper
        else {
            return activeSubCommand(nextSubCommand->nextSubCommand);
        }
    } 

    // Sub command is still running
    else {
        return nextSubCommand;
    }
}

CommandResult BaseTurntableCommand::execute() {
    initialize();

    CommandResult result = subCommands->execute();

    if(result != CommandResult::Running) {
        uninitialize();
    }

    return result;
}

void BaseTurntableCommand::initialize() {
    if(!initialized) {
        doInitialize();
        initialized = true;
    }
}

void BaseTurntableCommand::uninitialize() {
    if(!uninitialized) {
        doUninitialize();
        uninitialized = true;
    }
}