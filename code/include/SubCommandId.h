#ifndef SUBCOMMANDID_H
#define SUBCOMMANDID_H

enum SubCommandId {
    NoSubCommand = 0,
    ErrorSubCommand = 1,
    DisengageAzClutch = 2,
    EngageAzClutch = 3,
    SubMoveNStepsH = 6,
    SubGoToPosition = 7,
    SubDelay = 8,
    SubSetMovementVertical = 9,
    Calibrate7In = 10,
    Calibrate10In = 11,
    Calibrate12In = 12,
    CalibrateHome = 13,
    SubGoToPositionV = 14,
    SubMoveNStepsV = 15,
    CalibrateVerticalPoint = 16,
    ToggleLight = 17,
    SetPolarity = 18,
    CalibrateHorizontalPolarity = 16,
    CalibratePlatterHeight = 17,
    CalibrateHomeHeight = 18,
    CalibrateVerticalBounds = 19
};

#endif