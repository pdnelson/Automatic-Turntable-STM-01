#ifndef STMSSHIFTPIN_H
#define STMSSHIFTPIN_H

enum StmShiftPin : uint8_t {

    // Bank A
    AudioCutOff = 3,
    LedHeadshellIR = 4,
    LedPower = 5,
    LedPlayStatus = 6,
    LedPauseStatus = 7,

    // Bank B
    Led33Rpm = 8,
    Led10In = 9,
    Led12In = 10,
    LedAutoSize = 11,
    Led7In = 12,
    Led45Rpm = 13,
    Led78Rpm = 14,
    LedAutoSpeed = 15
};

#endif