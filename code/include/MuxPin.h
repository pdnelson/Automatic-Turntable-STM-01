#ifndef MUXPIN_H
#define MUXPIN_H

enum MuxPin : uint8_t {
    BtnSizeSelect = 0,
    BtnSpeedSelect = 1,
    BtnPlay = 2,
    BtnPause = 3,
    BtnTestMode = 4,
    BtnResetSettings = 6,
    BtnCalibration = 7,

    Unused5 = 5
};

#endif