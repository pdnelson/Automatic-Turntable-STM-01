#ifndef COMMANDRESULT_H
#define COMMANDRESULT_H

enum CommandResult {
    Running = 0,
    Success = 1,
    LiftStalledMovingUp = 2,
    LiftStalledMovingDown = 3,

    // The tonearm "pause" routine finished, though the tonearm's status is not lifted.
    NotLifted = 4,

    ClutchFailedToEngage = 5,
    ClutchFailedToDisengage = 6
};

#endif