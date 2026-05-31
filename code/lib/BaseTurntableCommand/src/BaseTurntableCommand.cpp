#include <BaseTurntableCommand.h>
#include <ActionCommand.h>
#include <CommandResult.h>
#include <TurntableState.h>

BaseTurntableCommand::BaseTurntableCommand(TurntableState* state) {
    this->state = state;
}

CommandResult BaseTurntableCommand::execute() {
    initialize();

    CommandResult result = doExecute();

    if(result.complete || result.error != CommandError::NoError) {
        uninitialize();
    }

    return result;
}

void BaseTurntableCommand::initialize() {
    if(!this->initialized) {
        this->doInitialize();
        this->initialized = true;
    }
}

void BaseTurntableCommand::uninitialize() {
    if(!this->uninitialized) {
        this->doUninitialize();
        this->uninitialized = true;
    }
}