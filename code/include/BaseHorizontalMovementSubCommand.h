#include <AzimuthDirection.h>
#include <BaseTurntableSubCommand.h>

#ifndef BaseHorizontalMovementSubCommand_h
#define BaseHorizontalMovementSubCommand_h
class TurntableState;

class BaseHorizontalMovementSubCommand : public BaseTurntableSubCommand {
    public:
        BaseHorizontalMovementSubCommand(TurntableState* state, uint8_t speed);

        uint8_t speed = 0;

        bool checkStall(AzimuthDirection direction);
        AzimuthDirection calculateDirection(uint16_t destination);
        uint16_t calculateTravelDistance(uint16_t destination);
        CommandResult stallResult(AzimuthDirection direction);
        void baseUninitialize();

    private:
        int16_t stallCounter = 0;
        int16_t stallPosition = 0;
};

#endif