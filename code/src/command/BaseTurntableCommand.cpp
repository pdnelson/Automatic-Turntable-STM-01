#include <BaseTurntableCommand.h>

BaseTurntableCommand::BaseTurntableCommand(TurntableState* state) {
    this->state = state;
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