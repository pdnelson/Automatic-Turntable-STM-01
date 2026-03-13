#ifndef EXTERNALCOMMAND_H
#define EXTERNALCOMMAND_H

enum ExternalCommand : uint8_t {
    NoOpCommand = 0,

    // Action commands (1-30)
    ActionPauseUnPause = 1,

    // Data/setting set commands (31-100)

    // Data retrieval commands (100+)
    DataVerticalEncoderPos = 100,
    DataHorizontalEncoderPos = 101,
    DataLiftStatus = 102,
    DataHomeStatus = 103,
    DataCurrentCommand = 104
};

#endif