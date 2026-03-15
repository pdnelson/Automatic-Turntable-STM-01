#ifndef EXTERNALCOMMAND_H
#define EXTERNALCOMMAND_H

enum ExternalCommand : uint8_t {
    NoOpCommand = 0,

    // Action commands (1-30)
    ActionPauseUnPause = 1,

    // Data/setting set commands (31-100)
    SetClearActionCommand = 35,

    // Data retrieval commands (100+)
    GetVerticalEncoderPos = 100,
    GetHorizontalEncoderPos = 101,
    GetLiftStatus = 102,
    GetHomeStatus = 103,
    GetCurrentCommand = 104,
    GetErrorCode = 105
};

#endif