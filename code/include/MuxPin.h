#ifndef MUXPIN_H
#define MUXPIN_H

enum MuxPin : uint8_t {
    // Action Buttons
    BtnPlay = 2,
    BtnPause = 3,
    BtnTestMode = 4,
    BtnCalibration = 7,

    // Settings Buttons
    BtnSizeSelect = 0,
    BtnSpeedSelect = 1,
    BtnResetSettings = 6,

    Unused5 = 5
};

#endif