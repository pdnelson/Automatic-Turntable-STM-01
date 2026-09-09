#include <SubCmdMoveUpUntilLifted.h>
#include <BaseLiftSubCommand.h>
#include <TurntableState.h>
#include <Pin.h>
#include <LiftStatus.h>

SubCmdMoveUpUntilLifted::SubCmdMoveUpUntilLifted(TurntableState* state, SubCommandId subCommandId, uint8_t speed) : BaseLiftSubCommand(state, speed) {
    this->subCommandId = subCommandId;
}

SubCommandId SubCmdMoveUpUntilLifted::getSubCommandId() {
    return subCommandId;
}

void SubCmdMoveUpUntilLifted::doInitialize() {
    baseInitialize();
    state->movementStepper.setDirection(VerticalDirection::Up);
    state->movementStepper.calibrateDirection(VerticalDirection::Up, VerticalDirection::Down);
}

CommandResult SubCmdMoveUpUntilLifted::doExecute() {
    if(digitalRead(Pin::Lift) == LiftStatus::Lifted) {
        return CommandResult::Success;
    } else {
        if(state->movementStepper.stepBlind(state->clockMicros)) {
            bool stalled = checkVerticalStall(VerticalDirection::Up, state->getVerticalEncoderPos());

            if(stalled) {
                return CommandResult::LiftStalledMovingUp;
            }
        }

        return CommandResult::Running;
    }
}

void SubCmdMoveUpUntilLifted::doUninitialize() {
    // do nothing
}