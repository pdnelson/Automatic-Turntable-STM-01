#include <BaseTurntableSubCommand.h>
#include <TurntableState.h>
#include <CommandResult.h>

BaseTurntableSubCommand::BaseTurntableSubCommand(TurntableState* state) {
    this->state = state;
}

CommandResult BaseTurntableSubCommand::execute() {
    this->initialize();

    if(this->result.error != CommandError::NoError) {
        this->uninitialize();
        return this->result;
    } else if(this->result.complete) {
        this->uninitialize();
        if(this->nextSubCommand == nullptr) {
            return { true, CommandError::NoError };
        } else {
            return this->nextSubCommand->execute();
        }
    } else {
        this->result = doExecute();
        return { false, this->result.error };
    }
}

void BaseTurntableSubCommand::initialize() {
    if(!this->initialized) {
        this->doInitialize();
        this->initialized = true;
    }
}

void BaseTurntableSubCommand::uninitialize() {
    if(!this->uninitialized) {
        this->doUninitialize();
        this->uninitialized = true;
    }
}