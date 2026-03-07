#ifndef COMMANDERROR_H
#define COMMANDERROR_H

enum CommandError : uint8_t {
    LiftStalledMovingUp = 0,
    LiftStalledMovingDown = 1,
    LiftTooHigh = 2,
    LiftTooLow = 3
};

#endif