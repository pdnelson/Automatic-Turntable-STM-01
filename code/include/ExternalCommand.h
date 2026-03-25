#ifndef EXTERNALCOMMAND_H
#define EXTERNALCOMMAND_H

enum ExternalCommand : uint8_t {
    ConnectionTest = 0,

    // Action commands (1-30)
    ActionPauseUnPause = 1,

    // Data/setting set commands (31-100)
    SetSpeed = 31,
    SetSize = 32,
    SetCustomSpeed = 33,
    SetRotateSpeed = 34,
    SetClearActionCommand = 35,
    SetRotateSize = 36,

    // Data retrieval commands (100+)
    GetVerticalEncoderPos = 100,
    GetHorizontalEncoderPos = 101,
    GetLiftStatus = 102,
    GetHomeStatus = 103,
    GetCurrentCommand = 104,
    GetErrorCode = 105,
    GetUpTime = 106,
    GetSpeedSetting = 107,
    GetSpeedTarget = 108,
    GetSizeSetting = 109
};

#endif