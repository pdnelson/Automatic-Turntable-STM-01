#include <VerticalDirection.h>
#include <BaseTurntableSubCommand.h>

#ifndef BaseLiftSubCommand_h
#define BaseLiftSubCommand_h
class TurntableState;

class BaseLiftSubCommand : public BaseTurntableSubCommand {
    public:
        BaseLiftSubCommand(TurntableState* state, uint16_t position, uint8_t speed, bool skipLiftCheck);

        uint8_t speed = 0;

        bool checkVerticalStall(VerticalDirection direction, int currentPosition);
        void baseInitialize();
        CommandResult executeLift();
        
        VerticalDirection direction;

    private:
        int16_t verticalStallCounter = 0;
        int16_t verticalStallPosition = 0;
        uint16_t destinationEncoderPosition = 0;
        bool reachedLimit = false;
        unsigned long timeLimitReached = 0;
        bool skipLiftCheck = false;
};

#endif