#ifndef SUBCOMMANDID_H
#define SUBCOMMANDID_H

enum SubCommandId {
    NoSubCommand = 0,
    ErrorSubCommand = 1,
    DisengageAzClutch = 2,
    EngageAzClutch = 3,
    LiftTonearm = 4,
    SetDownTonearm = 5,
    MoveNSteps = 6,
    SubGoToPosition = 7,
    SubDelay = 8,
    SubSetMovementVertical = 9,
    Calibrate7In = 10,
    Calibrate10In = 11,
    Calibrate12In = 12,
    CalibrateHome = 13,
};

#endif