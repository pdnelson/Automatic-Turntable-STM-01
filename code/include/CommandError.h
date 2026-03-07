#ifndef COMMANDERROR_H
#define COMMANDERROR_H

enum CommandError : uint8_t {
    LiftStalledMovingUp = 1,
    LiftStalledMovingDown = 2,

    // The tonearm "pause" routine finished, though the tonearm's status is not lifted.
    NotLifted = 3,
};

#endif