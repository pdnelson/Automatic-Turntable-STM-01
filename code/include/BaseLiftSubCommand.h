#include <VerticalDirection.h>
#include <BaseTurntableSubCommand.h>

#ifndef BaseLiftSubCommand_h
#define BaseLiftSubCommand_h
class TurntableState;

class BaseLiftSubCommand : public BaseTurntableSubCommand {
    public:
        BaseLiftSubCommand(TurntableState* state, uint8_t speed);

        uint8_t speed = 0;

        bool checkVerticalStall(VerticalDirection direction, int currentPosition);
        void baseInitialize();

    private:
        int16_t verticalStallCounter = 0;
        int16_t verticalStallPosition = 0;
};

#endif