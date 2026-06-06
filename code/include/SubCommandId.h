#ifndef SUBCOMMANDID_H
#define SUBCOMMANDID_H

enum SubCommandId {
    NoSubCommand = 0,
    ErrorSubCommand = 1,
    DisengageAzClutch = 2,
    EngageAzClutch = 3,
    LiftTonearm = 4,
    SetDownTonearm = 5,
    MoveNSteps = 6
};

#endif