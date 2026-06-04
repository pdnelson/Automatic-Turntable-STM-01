#include <BaseTurntableSubCommand.h>
#include <CommandResult.h>
#include <memory>

BaseTurntableSubCommand::BaseTurntableSubCommand(TurntableState* state) : std::enable_shared_from_this<BaseTurntableSubCommand>() {
    this->state = state;
}

std::shared_ptr<BaseTurntableSubCommand> BaseTurntableSubCommand::next(std::shared_ptr<BaseTurntableSubCommand> nextSubCommand) {
    nextEmptySubCommand(this->nextSubCommand) = nextSubCommand;
    return shared_from_this();
}

std::shared_ptr<BaseTurntableSubCommand>& BaseTurntableSubCommand::nextEmptySubCommand(std::shared_ptr<BaseTurntableSubCommand>& nextSubCommand) {
    if(nextSubCommand == nullptr) {
        return nextSubCommand;
    } else {
        return nextEmptySubCommand(nextSubCommand->nextSubCommand);
    }
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