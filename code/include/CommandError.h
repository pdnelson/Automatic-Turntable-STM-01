#ifndef COMMANDERROR_H
#define COMMANDERROR_H

enum CommandError : uint8_t {
    LiftStalledMovingUp = 0,
    LiftStalledMovingDown = 1,

    // The tonearm "pause" routine finished, though the tonearm's status is not lifted.
    NotLifted = 2,
};

#endif