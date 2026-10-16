#include <AzimuthDirection.h>
#include <BaseTurntableSubCommand.h>

#ifndef BaseHorizontalMovementSubCommand_h
#define BaseHorizontalMovementSubCommand_h
class TurntableState;

#define STEPS_BETWEEN_POLLING_REF 850
#define TICKS_PER_POLL_REF 550
#define SPEED_REF 25

class BaseHorizontalMovementSubCommand : public BaseTurntableSubCommand {
    public:
        BaseHorizontalMovementSubCommand(TurntableState* state, float speed);

        float speed = 0;

        bool checkStall(AzimuthDirection direction);
        AzimuthDirection calculateDirection(uint16_t destination);
        uint16_t calculateTravelDistance(uint16_t destination);
        CommandResult stallResult(AzimuthDirection direction);
        void baseUninitialize();

    private:
        uint16_t stallCounter = 0;
        uint16_t lastPosition = 0;
        uint16_t stepsBetweenPolling = 0;
        uint16_t ticksPerPoll = 0;
};

#endif