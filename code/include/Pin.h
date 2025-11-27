#ifndef PIN_H
#define PIN_H

enum Pin : uint8_t {
    Serial1Rx = 0,
    Serial1Tx = 1,
    InputMuxA = 2,
    InputMuxB = 3,
    InputMuxC = 4,
    Serial2Rx = 5,
    Serial2Tx = 6,
    InputMuxResult = 7,
    MovementStep4 = 8,
    MovementStep3 = 9,
    MovementStep2 = 10,
    MovementStep1 = 11,
    MovementSelect = 12,
    PowerOnStatusIn = 13,
    Serial5Tx = 14,
    Serial5Rx = 15,
    SpeedEncoderB = 16,
    SpeedEncoderA = 17,
    HeadshellPhotoDiode = 18,
    TurntableMotorPhase3 = 19,
    TurntableMotorPhase2 = 20,
    TurntableMotorPhase1 = 21,
    HorizontalClutchPosition = 22,
    VerticalPosition = 23,
    Serial3Tx = 24,
    Serial3Rx = 25,
    OutputShiftSda = 26,
    OutputShiftScl = 27,
    Serial4Rx = 28,
    Serial4Tx = 29,
    LiftStatus = 30,
    HorizontalPositionEncoderA = 31,
    HorizontalPositionEncoderB = 32,
    HomeStatus = 37,
    HorizontalClutchStep4 = 38,
    HorizontalClutchStep3 = 39,
    HorizontalClutchStep2 = 40,
    HorizontalClutchStep1 = 41,

    // Unused pins
    Unused33 = 33,

    Unused34 = 34,
    Unused35 = 35,
    Unused36 = 36,
};

#endif