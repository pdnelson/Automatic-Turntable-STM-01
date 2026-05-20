#ifndef PLAYSTEP_H
#define PLAYSTEP_H

enum PlayStep : uint8_t {
    LiftToCalibratedPositionPlay = 0,
    WaitForLiftStatusPlay = 1,
    MoveHorizontallyNSteps = 2,
    LowerUntilToneArmReleasedPlay = 3,
    LowerBelowRecordPlay = 4
};

#endif